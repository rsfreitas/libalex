
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Sun Aug 24 21:05:01 2014
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

#include <stdlib.h>
#include <string.h>

#include "libalex.h"

/*
 * This is an original Allegro function. We put it here because there it
 * is declared as static and we need it.
 */
void dotted_rect(int x1, int y1, int x2, int y2, int fg, int bg)
{
    BITMAP *gui_bmp = gui_get_screen();
    int x = ((x1 + y1) & 1) ? 1 : 0;
    int c;

    /* two loops to avoid bank switches */
    for (c = x1; c <= x2; c++)
        putpixel(gui_bmp, c, y1, (((c + y1) & 1) == x) ? fg : bg);

    for (c = x1; c <= x2; c++)
        putpixel(gui_bmp, c, y2, (((c + y2) & 1) == x) ? fg : bg);

    for (c = y1 + 1; c < y2; c++) {
        putpixel(gui_bmp, x1, c, (((c + x1) & 1) == x) ? fg : bg);
        putpixel(gui_bmp, x2, c, (((c + x2) & 1) == x) ? fg : bg);
    }
}

/*
 * ------- internal objects -------
 */

/*
 * Create and return a structure of type 'struct al_callback_data'.
 */
struct al_callback_data *new_callback_data(void)
{
    struct al_callback_data *cd = NULL;

    cd = calloc(1, sizeof(struct al_callback_data));

    if (NULL == cd)
        return NULL;

    return cd;
}

/*
 * Create and return a structure of type 'struct dlg_obj_ref'.
 */
struct dlg_obj_ref *new_obj_ref(const char *name, int dlg_index,
    enum al_grc_object type)
{
    struct dlg_obj_ref *r = NULL;

    r = calloc(1, sizeof(struct dlg_obj_ref));

    if (NULL == r) {
        al_set_errno(AL_ERROR_NEW_REF);
        return NULL;
    }

    r->name = strdup(name);
    r->dlg_index = dlg_index;
    r->type = type;

    return r;
}

/*
 * Destroy a structure of type 'struct dlg_obj_ref'.
 */
void destroy_obj_ref(void *a)
{
    struct dlg_obj_ref *r = (struct dlg_obj_ref *)a;

    if (r->name != NULL)
        free(r->name);

    free(r);
}

/*
 * Create and return a structure of type 'struct al_grc'.
 */
struct al_grc *new_grc(void)
{
    struct al_grc *g;

    g = calloc(1, sizeof(struct al_grc));

    if (NULL == g)
        return NULL;

    g->jgrc = NULL;
    g->ref = NULL;
    g->dlg = NULL;
    g->callback_data = NULL;
    g->g_data = NULL;
    g->grc_menu = NULL;
    g->menu = NULL;

    /*
     * Let the virtual keyboard disabled by now. If there is such an object
     * this flag will be enabled.
     */
    g->virtual_keyboard = false;

    /* We're not prepared yet */
    g->are_we_prepared = false;

    return g;
}

static void destroy_al_menu(void *a)
{
    struct al_menu *menu = (struct al_menu *)a;

    free(menu->menu);
    free(menu);
}

/*
 * Destroy a structure of type 'struct al_grc'.
 */
void destroy_grc(struct al_grc *grc)
{
    int i;

    if (grc->jgrc != NULL)
        cjson_delete(grc->jgrc);

    /*
     * TODO: Here we need to destroy some objects info, like button text.
     */
    if (grc->dlg != NULL)
        free(grc->dlg);

    if (grc->ref != NULL)
        cdll_free(grc->ref, destroy_obj_ref);

    if (grc->callback_data != NULL)
        cdll_free(grc->callback_data, NULL);

    if (grc->g_data != NULL)
        cdll_free(grc->g_data, NULL);

    if (grc->dlg_menu != NULL) {
        for (i = 0; i < grc->dlg_menu_t_items; i++) {
            if (grc->dlg_menu[i].text != NULL)
                free(grc->dlg_menu[i].text);

            if (grc->dlg_menu[i].dp != NULL)
                free(grc->dlg_menu[i].dp);
        }

        free(grc->dlg_menu);
    }

    if (grc->menu != NULL)
        cdll_free(grc->menu, destroy_al_menu);

    if (grc->grc_menu != NULL)
        cdll_free(grc->grc_menu, destroy_menu);

    free(grc);
}

struct grc_generic_data *new_grc_generic_data(void)
{
    struct grc_generic_data *d = NULL;

    d = calloc(1, sizeof(struct grc_generic_data));

    if (NULL == d)
        return NULL;

    return d;
}

