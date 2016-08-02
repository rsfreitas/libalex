
/*
 * Description: Translate info loaded from an UI file to Allegro
 *              calls.
 *
 * Author: Rodrigo Freitas
 * Created at: Mon Dec 15 11:25:28 2014
 * Project: libalex
 *
 * Copyright (c) 2014 Rodrigo Freitas
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#include <ctype.h>

#include "libalex.h"
#include "gui/objects.h"

/*
 * Default DIALOG objects:
 *
 *  d_clear_proc
 *  d_yield_proc
 */
#define DLG_DEFAULT_NOBJS           2

/*
 * ------- Common functions -------
 */

static int special_keys(const char *key)
{
    int ret = KEY_F1 - 1, k = 0;
    char *tmp;

    if (!strcmp(key, "ESC"))
        return KEY_ESC;

    if (key[0] == 'F') {
        tmp = (char *)&key[1];
        k = strtol(tmp, NULL, 10);
        return ret + k;
    }

    return -1;
}

static int key_char(const char *key)
{
    int c = key[0];

    if (isalpha(c))
        return c - '@';

    if (isdigit(c))
        return c - 21;

    return -1;
}

/*
 * Translate a key string, since it is in the format 'KEY_X', to its
 * corresponding scancode.
 */
static int grc_tr_str_key_to_al_key(const char *skey)
{
    cstring_t *tmp = NULL, *p = NULL;
    cstring_list_t *list = NULL;
    int ret = -1;

    tmp = cstring_create("%s", skey);

    if (NULL == tmp)
        return -1;

    list = cstring_split(tmp, "_");

    if (NULL == list)
        goto end_block;

    if (cstring_list_size(list) != 2)
        goto end_block;

    /* ok */
    p = cstring_list_get(list, 1);

    if (cstring_length(p) == 1)
        ret = key_char(cstring_valueof(p));
    else
        ret = special_keys(cstring_valueof(p));

    cstring_unref(p);

end_block:
    if (tmp != NULL)
        cstring_unref(tmp);

    if (list != NULL)
        cstring_list_destroy(list);

    return ret;
}

static int search_object_ref(void *a, void *b)
{
    struct dlg_obj_ref *r = (struct dlg_obj_ref *)a;
    char *name = (char *)b;

    if (!strcmp(r->name, name))
        return 1;

    return 0;
}

/*
 * Search for an object through its "name" inside the objects reference list
 * and return a pointer to its DIALOG.
 */
DIALOG *get_DIALOG_from_grc(struct al_grc *grc, const char *object_name)
{
    struct dlg_obj_ref *ref = NULL;

    if (NULL == object_name) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return NULL;
    }

    ref = cdll_map(grc->ref, search_object_ref, (void *)object_name);

    if (NULL == ref) {
        al_set_errno(AL_ERROR_OBJECT_NOT_FOUND);
        return NULL;
    }

    return ref->dlg;
}

static MENU *search_menu(MENU *menu, int t_items, const char *object_name)
{
    int i;

    for (i = 0; i < t_items; i++) {
        if (menu[i].dp == NULL)
            continue;

        if (!strcmp(menu[i].dp, object_name))
            return &menu[i];
    }

    return NULL;
}

MENU *get_MENU_from_grc(struct al_grc *grc, const char *object_name)
{
    struct al_menu *p = NULL;
    MENU *it = NULL;

    if (NULL == object_name) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return NULL;
    }

    /* Search in the main menu */
    it = search_menu(grc->dlg_menu, grc->dlg_menu_t_items, object_name);

    if (it != NULL)
        return it;

    /* Search in the others */
    for (p = grc->menu; p; p = p->next) {
        it = search_menu(p->menu, p->t_items, object_name);

        if (it != NULL)
            return it;
    }

    if (NULL == it) {
        al_set_errno(AL_ERROR_OBJECT_NOT_FOUND);
        return NULL;
    }

    return NULL;
}

/*
 * ------- Graphical Interface handling functions -------
 */

static int real_change_resolution(struct al_grc *grc)
{
/*    if (install_allegro(SYSTEM_AUTODETECT, NULL, NULL)) {
        al_set_errno(AL_ERROR_LIB_INIT);
        return -1;
    }

    if (install_keyboard()) {
        al_set_errno(AL_ERROR_KEYBOARD_INIT);
        return -1;
    }

    install_timer();
    set_color_depth(grc->gfx.color_depth);
*/
    if (set_gfx_mode(GFX_XWINDOWS, grc->gfx.width, grc->gfx.height,
                     0, 0) != 0)
    {
        if (set_gfx_mode(GFX_FBCON, grc->gfx.width, grc->gfx.height,
                         0, 0) != 0)
        {
            remove_keyboard();
            allegro_exit();
            al_set_errno(AL_ERROR_SET_GFX_MODE);
            return -1;
        }
    } else {
        if (grc->gfx.use_mouse == true) {
            install_mouse();
            gui_mouse_focus = FALSE;
        }
    }

    /* Disable ctrl+alt+end */
    if (grc->gfx.block_keys == true)
        three_finger_flag = FALSE;

    return 0;
}

static int load_resolution_info(struct al_grc *grc)
{
    cjson_t *jinfo;
    struct grc_json_key *e;

    jinfo = grc_get_object(grc, OBJ_INFO);

    if (NULL == jinfo) {
        al_set_errno(AL_ERROR_INFO_BLOCK_NOT_FOUND);
        return -1;
    }

    /* width */
    e = get_grc_json_key(AL_GRC_JOBJ_WIDTH);

    if (NULL == e)
        goto unknown_grc_key_block;

    grc->gfx.width = grc_get_object_value(jinfo, e->name, AL_DEFAULT_WIDTH);

    /* height */
    e = get_grc_json_key(AL_GRC_JOBJ_HEIGHT);

    if (NULL == e)
        goto unknown_grc_key_block;

    grc->gfx.height = grc_get_object_value(jinfo, e->name, AL_DEFAULT_HEIGHT);

    /* color depth */
    e = get_grc_json_key(AL_GRC_JOBJ_COLOR_DEPTH);

    if (NULL == e)
        goto unknown_grc_key_block;

    grc->gfx.color_depth = grc_get_object_value(jinfo, e->name,
                                                AL_DEFAULT_COLOR_DEPTH);

    /* block exit keys */
    e = get_grc_json_key(AL_GRC_JOBJ_BLOCK_EXIT_KEYS);

    if (NULL == e)
        goto unknown_grc_key_block;

    grc->gfx.block_keys = grc_get_object_value(jinfo, e->name, true);

    /* mouse */
    e = get_grc_json_key(AL_GRC_JOBJ_MOUSE);

    if (NULL == e)
        goto unknown_grc_key_block;

    grc->gfx.use_mouse = grc_get_object_value(jinfo, e->name, false);

    /* ignore_esc_key */
    e = get_grc_json_key(AL_GRC_JOBJ_IGNORE_ESC_KEY);

    if (NULL == e)
        goto unknown_grc_key_block;

    grc->ignore_esc_key = grc_get_object_value(jinfo, e->name, false);

    return 0;

unknown_grc_key_block:
    al_set_errno(AL_ERROR_UNDEFINED_GRC_KEY);
    return -1;
}

/*
 * Load main information from a GRC file GFX related and sets the screen
 * resolution so we can display some nice UI.
 */
/*int gui_change_resolution(struct al_grc *grc)
{
//    if (load_resolution_info(grc) < 0)
//        return -1;

    return real_change_resolution(grc);
}*/

/*
 * Load main colors of the DIALOG.
 */
/*int gui_load_colors(struct al_grc *grc)
{
    cjson_t *jcolors;
    struct grc_json_key *e;
    char *fg, *bg;

    jcolors = grc_get_object(grc, OBJ_COLORS);

    if (NULL == jcolors) {
        al_set_errno(AL_ERROR_COLORS_BLOCK_NOT_FOUND);
        return -1;
    }
*/
    /* foreground */
/*    e = get_grc_json_key(AL_GRC_JOBJ_FOREGROUND);

    if (NULL == e)
        goto unknown_grc_key_block;

    fg = grc_get_object_str(jcolors, e->name);
*/
    /* background */
/*    e = get_grc_json_key(AL_GRC_JOBJ_BACKGROUND);

    if (NULL == e)
        goto unknown_grc_key_block;

    bg = grc_get_object_str(jcolors, e->name);
*/
    /* Global colors */
/*    grc->fg = color_grc_to_al(get_color_depth(), fg);
    grc->bg = color_grc_to_al(get_color_depth(), bg);

    free(bg);
    free(fg);

    return 0;

unknown_grc_key_block:
    al_set_errno(AL_ERROR_UNDEFINED_GRC_KEY);
    return -1;
}*/

/*
 * Turn back to text mode.
 */
void gui_reset_resolution(void)
{
    cmsleep(100);
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    remove_keyboard();
    allegro_exit();
}

/*
 * ------- DIALOG handling functions -------
 */

static void DIALOG_creation_start(DIALOG *dlg, struct al_grc *grc)
{
    DIALOG *p;
    struct grc_object *gobj = NULL;

    gobj = new_grc_object();

    if (NULL == gobj)
        return;

    /* Sets the DIALOG object */
    p = gobj->dlg;
    p->proc = d_clear_proc;
    dlg[0] = *p;

    /* Saves this new objects so we can free it later */
    grc->ui_objects = cdll_unshift(grc->ui_objects, gobj);
}

static void DIALOG_creation_finish(DIALOG *dlg, unsigned int dlg_size,
    struct al_grc *grc)
{
    DIALOG *p;
    struct grc_object *gobj = NULL;

    gobj = new_grc_object();

    if (NULL == gobj)
        return;

    /* Sets the DIALOG last known object */
    p = gobj->dlg;
    p->proc = d_yield_proc;
    dlg[dlg_size - 1] = *p;

    /* Saves this new objects so we can free it later */
    grc->ui_objects = cdll_unshift(grc->ui_objects, gobj);

    /* Create the real end of the DIALOG */
    gobj = new_grc_object();

    if (NULL == gobj)
        return;

    /* Sets the DIALOG end mark */
    p = gobj->dlg;
    p->proc = NULL;
    dlg[dlg_size] = *p;

    /* Saves this new objects so we can free it later */
    grc->ui_objects = cdll_unshift(grc->ui_objects, gobj);
}

static int add_key(struct al_grc *grc, int dlg_index, cjson_t *key)
{
    DIALOG *d = &grc->dlg[dlg_index];
//    struct dlg_obj_ref *ref = NULL;
    struct grc_key_data *kdata;

    kdata = new_key_data(key);

    if (NULL == kdata)
        return -1;

    /* Adds an object into the DIALOG */
    d->proc = gui_d_keyboard_proc;
    d->d1 = grc_tr_str_key_to_al_key(kdata->key);

    /*
     * Case we're adding the ESC key, we signal internally, so we don't
     * override it.
     */
    if (d->d1 == KEY_ESC)
        grc->esc_key_user_defined = true;

    /* Creates a reference the be used by the user later */
/*    ref = new_obj_ref(kdata->name, dlg_index, AL_GRC_OBJ_KEY);

    if (NULL == ref)
        return -1;

    grc->ref = cdll_unshift(grc->ref, ref);*/
    destroy_key_data(kdata);

    return 0;
}

/*
 * Simple callback function to block ESC key to the user.
 */
static int __disable_key(void *arg __attribute__((unused)))
{
    return D_O_K;
}

static void add_default_esc_key(struct al_grc *grc, DIALOG *dlg)
{
    /* Was the key defined by the user? */
    if (grc->esc_key_user_defined == true)
        return;

    /* Did the user ask to ignore the ESC key? */
    if (grc->ignore_esc_key == false)
        return;

    dlg = calloc(1, sizeof(DIALOG));

    /*
     * Uses the Allegro key object to avoid create an unecessary
     * 'al_callback_data' structure.
     */
    dlg->proc = d_keyboard_proc;
    dlg->d1 = KEY_ESC;
    dlg->dp = __disable_key;
}

/*
 * Traverses the key array objects defined in the GRC file and insert
 * them into the DIALOG.
 */
static int DIALOG_add_keys(struct al_grc *grc, cjson_t *keys, int dlg_index)
{
    int i, t;
    cjson_t *n;

    t = cjson_get_array_size(keys);

    if (t <= 0) {
        al_set_errno(AL_ERROR_NO_KEYS);
        return -1;
    }

    grc->esc_key_user_defined = false;

    for (i = 0; i < t; i++) {
        n = cjson_get_array_item(keys, i);

        if (add_key(grc, dlg_index + i, n) < 0)
            return -1;
    }

    return 0;
}

static int cvt_grc_menu(void *a, void *b)
{
    struct grc_menu *menu = (struct grc_menu *)a, *p;
    struct al_grc *grc = (struct al_grc *)b;
    int t_items = 0, i;
    MENU *m;
    struct al_menu *al_menu;

    if (menu->items != NULL)
        t_items = cdll_size(menu->items);

    m = calloc(t_items + 1, sizeof(MENU));

    if (NULL == m)
        return -1;

    for (i = 0, p = menu->items; p; p = p->next, i++) {
        if (strcmp(p->text, MENU_SEPARATOR)) {
            m[i].text = p->text;
            m[i].dp = p->name;
        } else
            m[i].text = "";
    }

    al_menu = new_al_menu();
    al_menu->name = menu->name;
    al_menu->parent = menu->parent;
    al_menu->menu = m;
    al_menu->t_items = t_items;

    grc->menu = cdll_unshift(grc->menu, al_menu);

    return 0;
}

static void grc_menu_to_MENU(struct al_grc *grc)
{
    cdll_map(grc->grc_menu, cvt_grc_menu, grc);
}

static int create_main_MENU(struct al_grc *grc, cjson_t *main_options)
{
    cjson_t *n;
    int i, t;
    char *tmp;
    struct grc_json_key *e;

    /* Creates the main menu */
    t = cjson_get_array_size(main_options);
    grc->dlg_menu = calloc(t + 1, sizeof(MENU));

    for (i = 0; i < t; i++) {
        n = cjson_get_array_item(main_options, i);

        /* text */
        e = get_grc_json_key(AL_GRC_JOBJ_TEXT);

        if (NULL == e)
            return -1;

        tmp = grc_get_object_str(n, e->name);
        grc->dlg_menu[i].text = tmp;

        /* name */
        e = get_grc_json_key(AL_GRC_JOBJ_NAME);

        if (NULL == e)
            return -1;

        tmp = grc_get_object_str(n, e->name);
        grc->dlg_menu[i].dp = tmp;
    }

    return t;
}

static int search_child(void *a, void *b)
{
    struct al_menu *al_menu = (struct al_menu *)a;
    char *name = (char *)b;

    if (!strcmp(al_menu->name, name))
        return 1;

    return 0;
}

static MENU *search_child_MENU(struct al_grc *grc, const char *child_name)
{
    struct al_menu *al_menu;

    al_menu = cdll_map(grc->menu, search_child, (void *)child_name);

    if (NULL == al_menu)
        return NULL;

    return al_menu->menu;
}

static MENU *search_MENU_item(struct al_grc *grc, const char *item_name)
{
    struct al_menu *p;
    int i;

    for (p = grc->menu; p; p = p->next)
        for (i = 0; i < p->t_items; i++)
            if (p->menu[i].dp != NULL)
                if (!strcmp(p->menu[i].dp, item_name))
                    return &p->menu[i];

    return NULL;
}

/*
 * Adjusts some menu item pointers so that they correctly point to their
 * submenus (when needed).
 */
static int adjust_submenus(void *a, void *b)
{
    struct al_menu *al_menu = (struct al_menu *)a;
    struct al_grc *grc = (struct al_grc *)b;
    MENU *it;

    if (al_menu->parent == NULL)
        return 0;

    /* Searches for the item and point it to this menu */
    it = search_MENU_item(grc, al_menu->parent);

    if (it != NULL)
        it->child = al_menu->menu;

    return 0;
}

/*
 * Creates a MENU structure that will be used by the 'd_menu_proc' object
 * and add all items and submenus defined by the user.
 */
static void create_MENU(struct al_grc *grc, cjson_t *main_options)
{
    int i;

    grc->dlg_menu_t_items = create_main_MENU(grc, main_options);

    /* Adjusts main menus */
    for (i = 0; i < grc->dlg_menu_t_items; i++) {
        if (grc->dlg_menu[i].text == NULL)
            continue;

        if (grc->dlg_menu[i].dp != NULL)
            grc->dlg_menu[i].child = search_child_MENU(grc, grc->dlg_menu[i].dp);
    }

    /* Adjusts submenus */
    cdll_map(grc->menu, adjust_submenus, grc);
}

/*
 * Adds a 'd_menu_proc' object into the DIALOG objects. As there is any
 * configuration (yet) its screen position is fixed into the upper right
 * corner.
 */
static void add_menu(struct al_grc *grc, int dlg_index)
{
    DIALOG *d = &grc->dlg[dlg_index];

    d->proc = d_menu_proc;
    d->dp = grc->dlg_menu;

    /* x and y fixe positions to the user (until now) */
    d->x = 0;
    d->y = 0;
}

/*
 * All menu items definied in the GRC file are stored in 'struct grc_menu'
 * types. They will be "manipulated" by the user when needed, such as when
 * we need to adjust an item callback function.
 *
 * The 'struct al_menu' list is used to store the menus in a structure
 * understood by Allegro, the MENU structure. And serve as a source to
 * build the menu which will be used by the 'd_menu_proc' object from the
 * DIALOG.
 */
static void DIALOG_add_menu(struct al_grc *grc, cjson_t *menu, int dlg_index)
{
    cjson_t *menu_options = NULL, *main_options = NULL, *n;
    int i, t;
    struct grc_menu *grcm;

    menu_options = cjson_get_object_item(menu, OBJ_MENU_OPTIONS);
    main_options = cjson_get_object_item(menu, OBJ_MAIN_OPTIONS);

    /* Extracts menus and items informations */
    if ((NULL == menu_options) || (NULL == main_options))
        return;

    t = cjson_get_array_size(menu_options);

    for (i = 0; i < t; i++) {
        n = cjson_get_array_item(menu_options, i);

        grcm = new_menu(n);
        grc->grc_menu = cdll_unshift(grc->grc_menu, grcm);
    }

    /* Converts GRC menus to Allegro MENUs */
    grc_menu_to_MENU(grc);

    /* Assembles the Allegro supported menu(s) */
    create_MENU(grc, main_options);

    /* Adds a 'd_menu_proc' object into the DIALOG */
    add_menu(grc, dlg_index);
}

int gui_init(struct al_grc *grc)
{
    if (install_allegro(SYSTEM_AUTODETECT, NULL, NULL)) {
        al_set_errno(AL_ERROR_LIB_INIT);
        return -1;
    }

    if (install_keyboard()) {
        al_set_errno(AL_ERROR_KEYBOARD_INIT);
        return -1;
    }

    install_timer();
    set_color_depth(info_color_depth(grc));

    if (set_gfx_mode(GFX_XWINDOWS, grc->gfx.width, grc->gfx.height,
                     0, 0) != 0)
    {
        if (set_gfx_mode(GFX_FBCON, grc->gfx.width, grc->gfx.height,
                         0, 0) != 0)
        {
            remove_keyboard();
            allegro_exit();
            al_set_errno(AL_ERROR_SET_GFX_MODE);
            return -1;
        }
    } else {
        if (grc->gfx.use_mouse == true) {
            install_mouse();
            gui_mouse_focus = FALSE;
        }
    }

    /* Disable ctrl+alt+end */
    if (grc->gfx.block_keys == true)
        three_finger_flag = FALSE;

    return 0;
}

/*
 * Here we create the Allegro DIALOG array pointing to every previously loaded
 * object.
 */
int DIALOG_create(struct al_grc *grc)
{
    int dlg_items = 0, index = 0;
    DIALOG *d, *q;
    struct grc_object *p = NULL;

    /* d_yield_proc */
    dlg_items += 1;

    /* d_clear_proc */
    if (grc->use_gfx == true)
        dlg_items += 1;

    /*
     * Even that the user has defined the ESC key, we allocate an extra space.
     * It will not be used...
     */
    if (grc->ignore_esc_key)
        dlg_items += 1;

    dlg_items += cdll_size(grc->ui_objects);

    /*
     * The total size of the DIALOG will be the amount of keys + the amount of
     * objects + menu + ignored ESC key + d_yield_proc + d_clear_proc +
     * 1 (NULL proc).
     */
    d = calloc(dlg_items + 1, sizeof(DIALOG));

    if (NULL == d) {
        al_set_errno(AL_ERROR_MEMORY);
        return -1;
    }

    /* Initializes the DIALOG */
    if (grc->use_gfx == true)
        DIALOG_creation_start(d, grc);

    /* Add user defined objects */
    for (p = grc->ui_objects, index = 1; p; p = p->next, index++) {
        q = grc_object_get_DIALOG(p);
        d[index] = *q;
    }
    
    /* TODO Add a menu */

    /* TODO Add keys */

    /* We ignore the ESC key (if needed) */
    add_default_esc_key(grc, &d[dlg_items - 2]);

    /* Ends the DIALOG */
    DIALOG_creation_finish(d, dlg_items, grc);

    /* Points to the new DIALOG */
    grc->al_dlg = d;

    return 0;
}

/*int create_DIALOG(struct al_grc *grc)
{
    cjson_t *objects, *keys = NULL, *menu = NULL;
    int total = 0, p = 0;

    objects = grc_get_object(grc, OBJ_OBJECTS);

    if (NULL == objects) {
        al_set_errno(AL_ERROR_OBJECTS_BLOCK_NOT_FOUND);
        return -1;
    }

    keys = grc_get_object(grc, OBJ_KEYS);
    menu = grc_get_object(grc, OBJ_MENU);
*/
    /*
     * The total size of the DIALOG will be the amount of keys + the amount of
     * objects + menu + ignored ESC key + d_yield_proc + d_clear_proc + 1.
     */
//    total = cjson_get_array_size(objects);
//    total += 1; /* d_yield_proc */

//    if (grc->use_gfx == true)
//        total += 1; /* d_clear_proc */

//    if (keys != NULL)
//        total += cjson_get_array_size(keys);

    /*
     * Case a menu has been defined, it also needs a space inside the DIALOG
     */
//    if (menu != NULL)
//        total += 1;

    /*
     * Even that the user has defined the ESC key, we allocate an extra space.
     * It will not be used...
     */
/*    if (grc->ignore_esc_key)
        total += 1;

    grc->dlg = calloc(total + 1, sizeof(DIALOG));

    if (NULL == grc->dlg) {
        al_set_errno(AL_ERROR_MEMORY);
        return -1;
    }
*/
    /* Initializes the DIALOG */
//    if (grc->use_gfx == true)
//        DIALOG_creation_start(grc);

    /* Add user defined objects */
/*    p = DIALOG_add_objects(grc, objects, (grc->use_gfx == true) ? 1 : 0);

    if (p < 0)
        return -1;
*/
    /* Add a menu */
/*    if (menu != NULL) {
        DIALOG_add_menu(grc, menu, p);
        p += 1;
*/
        /*
         * Changes the main color so that the menu uses the same that the user
         * defined.
         */
/*        gui_fg_color = grc->fg;
        gui_bg_color = grc->bg;
    }
*/
    /* Add keys */
/*    if (keys != NULL)
        if (DIALOG_add_keys(grc, keys, p) < 0)
            return -1;
*/
    /* We ignore the ESC key (if needed) */
//    add_default_esc_key(grc, total - 2);

    /* Ends the DIALOG */
/*    DIALOG_creation_finish(grc, total - 1);

    return 0;
}*/

void run_DIALOG(struct al_grc *grc)
{
    if (grc->use_gfx == false)
        centre_dialog(grc->al_dlg);

    do_dialog(grc->al_dlg, -1);
}

/*
 * Execute a callback function from an object. If there is no function we
 * return a @default_return value.
 */
int run_callback(struct al_callback_data *acd, unsigned int default_return)
{
    if (acd->callback != NULL)
        return (acd->callback)(acd);

    return default_return;
}

