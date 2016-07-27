
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

/* Edit deafult height */
#define DEFAULT_EDIT_HEIGHT                 15

/* Space between buttons */
#define BUTTON_DEFAULT_SPACER               4

/* Button default height */
#define DEFAULT_BUTTON_HEIGHT               30

/* Space between checkboxes */
#define CHECKBOX_DEFAULT_SPACER             13

/* Checkbox default height */
#define DEFAULT_CHECKBOX_HEIGHT             15

/* Space between radio buttons */
#define RADIO_DEFAULT_SPACER                13

/* Radio button default height */
#define DEFAULT_RADIO_HEIGHT                15

struct rgb {
    int r;
    int g;
    int b;
};

struct color {
    char        name[128];
    struct rgb  rgb;
};

static struct color __colors[] = {
    { AL_BLACK,             { 0,    0,      0   }},
    { AL_WHITE,             { 255,  255,    255 }},
    { AL_LIGHT_RED,         { 255,  0,      0   }},
    { AL_LIGHT_GREEN,       { 0,    255,    0   }},
    { AL_LIGHT_BLUE,        { 0,    0,      255 }},
    { AL_YELLOW,            { 240,  240,    0   }},
    { AL_CYAN,              { 0,    255,    255 }},
    { AL_MAGENTA,           { 255,  0,      255 }},
    { AL_SILVER,            { 192,  192,    192 }},
    { AL_GRAY,              { 128,  128,    128 }},
    { AL_MAROON,            { 128,  0,      0   }},
    { AL_OLIVE,             { 128,  128,    0   }},
    { AL_PURPLE,            { 128,  0,      128 }},
    { AL_TEAL,              { 0,    128,    128 }},
    { AL_NAVY,              { 0,    0,      128 }},
    { AL_BLUE,              { 0,    0,      150 }},
    { AL_GREEN,             { 0,    150,    0   }},
    { AL_RED,               { 150,  0,      0   }},
    { AL_ORANGE_RED,        { 255,  69,     0   }},
    { AL_ORANGE,            { 255,  165,    0   }},
    { AL_KHAKI,             { 240,  230,    140 }},
    { AL_BROWN,             { 165,  42,     42  }},
    { AL_SADDLE_BROWN,      { 139,  69,     19  }},
    { AL_BAKERS_CHOCOLATE,  { 92,   51,     23  }},
    { AL_IVORY,             { 255,  255,    240 }},
    { AL_SNOW,              { 255,  250,    250 }},
    { AL_GHOST_WHITE,       { 248,  248,    255 }},
    { AL_INDIGO,            { 75,   0,      130 }},
    { AL_DARK_SLATE_BLUE,   { 72,   61,     139 }},
    { AL_SKY_BLUE,          { 135,  206,    235 }},
    { AL_SEA_GREEN,         { 46,   139,    87  }},
    { AL_DARK_GREEN,        { 0,    100,    0   }},
    { AL_FOREST_GREEN,      { 34,   139,    24  }},
    { AL_GOLD,              { 255,  215,    0   }},
    { AL_GOLDEN_ROD,        { 218,  165,    32  }},
    { AL_SALMON,            { 250,  128,    114 }},
    { AL_CORAL,             { 255,  127,    80  }},
    { AL_TOMATO,            { 255,  99,     71  }},
    { AL_DARK_SEA_GREEN,    { 143,  188,    143 }},
    { AL_DARK_CYAN,         { 0,    139,    139 }},
    { AL_TURQUOISE,         { 64,   224,    208 }},
    { AL_DARK_TURQUOISE,    { 0,    206,    209 }},
    { AL_CADET_BLUE,        { 95,   158,    160 }},
    { AL_OLIVE_DRAB,        { 107,  142,    35  }},
    { AL_STEEL_BLUE,        { 70,   130,    180 }},
    { AL_DODGER_BLUE,       { 30,   144,    255 }},
    { AL_MIDNIGHT_BLUE,     { 25,   25,     112 }},
    { AL_SLATE_BLUE,        { 106,  90,     205 }},
    { AL_SIENNA,            { 160,  82,     45  }},
    { AL_PERU,              { 205,  133,    63  }},
    { AL_TAN,               { 210,  180,    140 }},
    { AL_SLATE_GRAY,        { 112,  128,    144 }},
    { AL_DARK_KHAKI,        { 189,  183,    107 }},
    { AL_INDIAN_RED,        { 205,  92,     92  }}
};

#define MAX_COLORS              \
    (sizeof(__colors) / sizeof(__colors[0]))

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

/*
 * Translate a string into a color. Dealing correctly with the number of
 * colors from the GFX mode.
 */
int grc_tr_color_to_al_color(int color_depth, const char *color)
{
    struct color *c = NULL;
    unsigned int i;

    for (i = 0; i < MAX_COLORS; i++)
        if (!strcmp(__colors[i].name, color)) {
            c = &__colors[i];
            break;
        }

    if (NULL == c)
        return -1;

    return makecol_depth(color_depth, c->rgb.r, c->rgb.g, c->rgb.b);
}

static cjson_t *grc_get_object(struct al_grc *grc, const char *object)
{
    return cjson_get_object_item(grc->jgrc, object);
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

    return &grc->dlg[ref->dlg_index];
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
    if (install_allegro(SYSTEM_AUTODETECT, NULL, NULL)) {
        al_set_errno(AL_ERROR_LIB_INIT);
        return -1;
    }

    if (install_keyboard()) {
        al_set_errno(AL_ERROR_KEYBOARD_INIT);
        return -1;
    }

    install_timer();
    set_color_depth(grc->gfx.color_depth);

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
int gui_change_resolution(struct al_grc *grc)
{
    if (load_resolution_info(grc) < 0)
        return -1;

    return real_change_resolution(grc);
}

/*
 * Load main colors of the DIALOG.
 */
int gui_load_colors(struct al_grc *grc)
{
    cjson_t *jcolors;
    struct grc_json_key *e;
    char *fg, *bg;

    jcolors = grc_get_object(grc, OBJ_COLORS);

    if (NULL == jcolors) {
        al_set_errno(AL_ERROR_COLORS_BLOCK_NOT_FOUND);
        return -1;
    }

    /* foreground */
    e = get_grc_json_key(AL_GRC_JOBJ_FOREGROUND);

    if (NULL == e)
        goto unknown_grc_key_block;

    fg = grc_get_object_str(jcolors, e->name);

    /* background */
    e = get_grc_json_key(AL_GRC_JOBJ_BACKGROUND);

    if (NULL == e)
        goto unknown_grc_key_block;

    bg = grc_get_object_str(jcolors, e->name);

    /* Global colors */
    grc->fg = grc_tr_color_to_al_color(get_color_depth(), fg);
    grc->bg = grc_tr_color_to_al_color(get_color_depth(), bg);

    free(bg);
    free(fg);

    return 0;

unknown_grc_key_block:
    al_set_errno(AL_ERROR_UNDEFINED_GRC_KEY);
    return -1;
}

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

static void DIALOG_creation_start(struct al_grc *grc)
{
    DIALOG *d = &grc->dlg[0];

    d->proc = d_clear_proc;
}

static void DIALOG_creation_finish(struct al_grc *grc, int dlg_size)
{
    DIALOG *d = &grc->dlg[dlg_size];

    d->proc = d_yield_proc;
}

static void set_object_internal_data(DIALOG *d, struct al_grc *grc)
{
    struct al_callback_data *acd;

    acd = new_callback_data();

    if (NULL == acd) {
        al_set_errno(AL_ERROR_MEMORY);
        return;
    }

    acd->grc = (void *)grc;
    d->dp3 = acd;

    grc->callback_data = cdll_unshift(grc->callback_data, acd);
}

static int add_object(struct al_grc *grc, int dlg_index, cjson_t *object)
{
    struct grc_obj_properties *prop;
    DIALOG *d = &grc->dlg[dlg_index], *p;
    struct dlg_obj_ref *ref = NULL;
    struct grc_generic_data *g_data;
    int w = -1, h = -1;
    char tmp[MAX_EDIT_SIZE] = {0};

    prop = new_obj_properties(object);

    if (NULL == prop)
        return -1;

    /*
     * Objects that the user MUST define _width_ and _height_:
     *
     *  listbox
     *  box
     *  slider
     */

    /*
     * Add an object into the DIALOG. It is recommended that 'boxes' be the
     * first objects defined, so that no text be superimposed.
     */
    switch (prop->type) {
        case AL_GRC_OBJ_BOX:
            d->proc = d_box_proc;
            break;

        case AL_GRC_OBJ_DIGITAL_CLOCK:
            d->proc = gui_clock_proc;
            d->dp = grc->dlg_clock_str;
            break;

        case AL_GRC_OBJ_IMAGE:
        case AL_GRC_OBJ_MESSAGES_LOG_BOX:
            if (prop->type == AL_GRC_OBJ_MESSAGES_LOG_BOX) {
                d->proc = gui_messages_log_proc;
                d->d1 = prop->line_break_mode;
            } else {
                d->proc = gui_d_bitmap_proc;

                /*
                 * We set as NULL here so no error will be thrown when trying
                 * to draw the object and no image exists.
                 */
                d->dp = NULL;
            }

            /*
             * If we have a reference to a "father" object, the object position
             * will derived from his position. Worth mentioning that, for this
             * work correctly the "father" object must have been defined BEFORE
             * the object, otherwise we can't get his positions. ;-)
             */
            if (prop->parent != NULL) {
                p = get_DIALOG_from_grc(grc, prop->parent);
                d->x = p->x + 2;
                d->y = p->y + 2;
                d->w = p->w - 2;
                d->h = p->h - 2;
            }

            break;

        case AL_GRC_OBJ_CUSTOM:
            /*
             * Neste objeto, tanto a funcao de proprio objeto quanto a funcao
             * de callback, devem ser definidas em tempo de execucao, antes
             * da execucao do DIALOG.
             *
             * d->proc -> Funcao do objeto
             * d->dp3  -> callback
             */
            break;

        case AL_GRC_OBJ_VAR_TEXT:
        case AL_GRC_OBJ_FIXED_TEXT:
            d->proc = d_text_proc;

            if (prop->type == AL_GRC_OBJ_FIXED_TEXT) {
                /*
                 * Although 'prop' structure is freed right here, this only
                 * points to the original text, loaded from the GRC file.
                 * This way we can display it.
                 */
                d->dp = strdup(prop->text); /* TODO: Release this later */
            }

            break;

        case AL_GRC_OBJ_BUTTON:
            d->proc = gui_d_button_proc;
            d->flags = D_EXIT;
            d->dp = strdup(prop->text); /* TODO: Release this later */

            /* We compute width and height automatically, case is necessary */
            if (prop->w < 0)
                w = text_length(font, prop->text) + BUTTON_DEFAULT_SPACER;

            if (prop->h < 0)
                h = DEFAULT_BUTTON_HEIGHT;

            break;

        case AL_GRC_OBJ_EDIT:
            if (prop->data_length >= MAX_EDIT_SIZE) {
                al_set_errno(AL_ERROR_UNSUPPORTED_EDIT_INPUT_LENGTH);
                return -1;
            }

            if (prop->password_mode == false)
                d->proc = gui_d_edit_proc;
            else {
                d->proc = gui_d_password_proc;

                /*
                 * Creates a temporary buffer to store the asterisks strings.
                 */
                g_data = new_grc_generic_data();
                grc->g_data = cdll_unshift(grc->g_data, g_data);
                d->dp2 = g_data->data;
            }

            /* Creates the buffer to store the typed string in the object. */
            g_data = new_grc_generic_data();
            grc->g_data = cdll_unshift(grc->g_data, g_data);
            d->dp = g_data->data;

            d->flags = D_EXIT;
            d->d1 = prop->data_length;

            /*
             * Adjusts a default size allowing that the mouse may set the
             * focus on this object.
             */
            h = DEFAULT_EDIT_HEIGHT;

            /* We compute width automatically, case is necessary */
            if (prop->parent != NULL) {
                p = get_DIALOG_from_grc(grc, prop->parent);
                d->x = p->x + 3;
                d->y = p->y + 3;
                d->w = p->w - 4;
                d->h = h;
            } else {
                if (prop->w < 0) {
                    memset(tmp, '0', prop->data_length + 1);
                    w = text_length(font, tmp);
                }
            }

            break;

        case AL_GRC_OBJ_LIST:
            d->proc = gui_d_list_proc;
            d->flags = D_EXIT;
            break;

        case AL_GRC_OBJ_CHECK:
            d->proc = gui_d_check_proc;
            d->dp = strdup(prop->text); /* TODO: Release this later */

            if (prop->horizontal_position == AL_GRC_H_POS_RIGHT)
                d->d1 = 1;
            else
                d->d1 = 0;

            /* We compute width and height automatically, case is necessary */
            if (prop->w < 0)
                w = text_length(font, prop->text) + CHECKBOX_DEFAULT_SPACER;

            if (prop->h < 0)
                h = DEFAULT_CHECKBOX_HEIGHT;

            break;

        case AL_GRC_OBJ_RADIO:
            d->proc = gui_d_radio_proc;
            d->dp = strdup(prop->text); /* TODO: Release this later */
            d->d1 = prop->radio_group;
            d->d2 = prop->radio_type;

            /* We compute width and height automatically, case is necessary */
            if (prop->w < 0)
                w = text_length(font, prop->text) + RADIO_DEFAULT_SPACER;

            if (prop->h < 0)
                h = DEFAULT_RADIO_HEIGHT;

            break;

        case AL_GRC_OBJ_SLIDER:
            d->proc = gui_d_slider_proc;
            d->d1 = prop->data_length;
            break;

        /* Not implemented yet */
/*        case AL_GRC_OBJ_LIVE_IMAGE:
        case AL_GRC_OBJ_MULTLIVE_IMAGE:
            if (prop->type == AL_GRC_OBJ_LIVE_IMAGE)
                d->proc = gui_d_live_image;
            else {
                d->proc = gui_d_multlive_image;
                d->d2 = prop->devices;
            }

            d->d1 = prop->fps;

            if (prop->parent != NULL) {
                p = get_DIALOG_from_grc(grc, prop->parent);
                d->x = p->x + 2;
                d->y = p->y + 2;
                d->w = p->w - 4;
                d->h = p->h - 4;
            }

            break;*/

        case AL_GRC_OBJ_VT_KEYBOARD:
            d->proc = gui_d_vt_keyboard_proc;
            d->d1 = GRC_KLAYOUT_LETTERS;
            d->dp = grc;
            grc->virtual_keyboard = true;
            break;

        case AL_GRC_OBJ_ICON:
            d->proc = gui_d_icon_proc;
            d->flags = D_EXIT;
            break;

        case AL_GRC_OBJ_TEXTBOX:
            d->proc = d_textbox_proc;
            break;

        default:
            al_set_errno(AL_ERROR_UNKNOWN_OBJECT_TYPE);
            return -1;
    }

    /* Object position into the screen */
    if (prop->parent == NULL) {
        d->x = prop->x;
        d->y = prop->y;
        d->w = (w == -1) ? prop->w : w;
        d->h = (h == -1) ? prop->h : h;
    }

    /* Specific object colors */
    if (prop->fg != NULL)
        d->fg = grc_tr_color_to_al_color(get_color_depth(), prop->fg);
    else
        d->fg = grc->fg;

    d->bg = grc->bg;

    /* Hides the object or not */
    if (prop->hide == false)
        d->flags &= ~D_HIDDEN;
    else
        d->flags |= D_HIDDEN;

    /*
     * Every object already has access to internal library information,
     * even if they do not have callback functions.
     */
    set_object_internal_data(d, grc);

    /* Creates a reference the be used by the user later */
    if (prop->name != NULL) {
        ref = new_obj_ref(prop->name, dlg_index, prop->type);

        if (NULL == ref)
            return -1;

        grc->ref = cdll_unshift(grc->ref, ref);
    }

    destroy_obj_properties(prop);

    return 0;
}

/*
 * Traverses the object array that will compose the DIALOG from the GRC file.
 * The function returns the last object index, allowing that the added keys may
 * be in sequence.
 */
static int DIALOG_add_objects(struct al_grc *grc, cjson_t *objects,
    int dlg_index)
{
    int i, t = 0, p;
    cjson_t *o;

    t = cjson_get_array_size(objects);

    if (t <= 0) {
        al_set_errno(AL_ERROR_NO_OBJECTS);
        return -1;
    }

    for (i = 0, p = dlg_index; i < t; i++, p++) {
        o = cjson_get_array_item(objects, i);

        if (add_object(grc, p, o) < 0)
            return -1;
    }

    return p;
}

static int add_key(struct al_grc *grc, int dlg_index, cjson_t *key)
{
    DIALOG *d = &grc->dlg[dlg_index];
    struct dlg_obj_ref *ref = NULL;
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
    ref = new_obj_ref(kdata->name, dlg_index, AL_GRC_OBJ_KEY);

    if (NULL == ref)
        return -1;

    grc->ref = cdll_unshift(grc->ref, ref);
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

static void add_default_esc_key(struct al_grc *grc, int dlg_index)
{
    DIALOG *d = &grc->dlg[dlg_index];

    /* Was the key defined by the user? */
    if (grc->esc_key_user_defined == true)
        return;

    /* Did the user ask to ignore the ESC key? */
    if (grc->ignore_esc_key == false)
        return;

    /*
     * Uses the Allegro key object to avoid create an unecessary
     * 'al_callback_data' structure.
     */
    d->proc = d_keyboard_proc;
    d->d1 = KEY_ESC;
    d->dp = __disable_key;
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

    al_menu = calloc(1, sizeof(struct al_menu));
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

int create_DIALOG(struct al_grc *grc)
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

    /*
     * The total size of the DIALOG will be the amount of keys + the amount of
     * objects + menu + ignored ESC key + d_yield_proc + d_clear_proc + 1.
     */
    total = cjson_get_array_size(objects);
    total += 1; /* d_yield_proc */

    if (grc->use_gfx == true)
        total += 1; /* d_clear_proc */

    if (keys != NULL)
        total += cjson_get_array_size(keys);

    /*
     * Case a menu has been defined, it also needs a space inside the DIALOG
     */
    if (menu != NULL)
        total += 1;

    /*
     * Even that the user has defined the ESC key, we allocate an extra space.
     * It will not be used...
     */
    if (grc->ignore_esc_key)
        total += 1;

    grc->dlg = calloc(total + 1, sizeof(DIALOG));

    if (NULL == grc->dlg) {
        al_set_errno(AL_ERROR_MEMORY);
        return -1;
    }

    /* Initializes the DIALOG */
    if (grc->use_gfx == true)
        DIALOG_creation_start(grc);

    /* Add user defined objects */
    p = DIALOG_add_objects(grc, objects, (grc->use_gfx == true) ? 1 : 0);

    if (p < 0)
        return -1;

    /* Add a menu */
    if (menu != NULL) {
        DIALOG_add_menu(grc, menu, p);
        p += 1;

        /*
         * Changes the main color so that the menu uses the same that the user
         * defined.
         */
        gui_fg_color = grc->fg;
        gui_bg_color = grc->bg;
    }

    /* Add keys */
    if (keys != NULL)
        if (DIALOG_add_keys(grc, keys, p) < 0)
            return -1;

    /* We ignore the ESC key (if needed) */
    add_default_esc_key(grc, total - 2);

    /* Ends the DIALOG */
    DIALOG_creation_finish(grc, total - 1);

    return 0;
}

void run_DIALOG(struct al_grc *grc)
{
    if (grc->use_gfx == false)
        centre_dialog(grc->dlg);

    do_dialog(grc->dlg, -1);
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

