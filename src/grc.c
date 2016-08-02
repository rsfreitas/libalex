
/*
 * Description: Functions to manipulate a 'struct al_grc' structure.
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Jul 28 09:01:37 2016
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

#include "libalex.h"

/*
 * Destroy a structure of type 'struct al_grc'.
 */
void destroy_grc(struct al_grc *grc)
{
    int i;

    if (grc->ui_objects != NULL)
        cdll_free(grc->ui_objects, destroy_grc_object);

    if (grc->al_dlg != NULL)
        free(grc->al_dlg);

    if (grc->jgrc != NULL)
        cjson_delete(grc->jgrc);

    /*
     * TODO: Here we need to destroy some objects info, like button text
     *       and other stuff. Some Allegro's DIALOG internal fields...
     */
//    if (grc->dlg != NULL)
//        free(grc->dlg);

    if (grc->ref != NULL)
        cdll_free(grc->ref, destroy_obj_ref);

//    if (grc->callback_data != NULL)
//        cdll_free(grc->callback_data, destroy_callback_data);

//    if (grc->g_data != NULL)
//        cdll_free(grc->g_data, destroy_grc_generic_data);

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
        cdll_free(grc->grc_menu, destroy_grc_menu);

    free(grc);
}

/*
 * Create and return a structure of type 'struct al_grc'.
 */
struct al_grc *new_grc(void)
{
    struct al_grc *g;

    g = calloc(1, sizeof(struct al_grc));

    if (NULL == g) {
        al_set_errno(AL_ERROR_MEMORY);
        return NULL;
    }

    g->jgrc = NULL;
    g->ref = NULL;
    g->al_dlg = NULL;
    g->ui_objects = NULL;
    g->ui_menu = NULL;
//    g->dlg = NULL;
//    g->callback_data = NULL;
//    g->g_data = NULL;
    g->grc_menu = NULL;
    g->menu = NULL;

    /*
     * Let the virtual keyboard disabled by now. If there is such an object
     * this flag will be enabled later.
     */
    g->virtual_keyboard = false;

    /*
     * We're not prepared to run de DIALOG yet cause we need to load all
     * objects from the file and translate them to Allegro.
     */
    g->are_we_prepared = false;

    return g;
}

void grc_creates_reference(struct al_grc *grc, struct grc_object *object)
{
    struct grc_obj_properties *prop;
    struct dlg_obj_ref *ref;

    if ((NULL == object) || (NULL == grc))
        return;

    prop = grc_object_get_properties(object);

    /*
     * If the object has no 'name' propertie it's not a parent, we don't need
     * to create a reference to it.
     */
    if (grc_obj_properties_has_name(prop) == false)
        return;

    ref = new_obj_ref(grc_obj_properties_name(prop),
                      grc_object_get_DIALOG(object),
                      grc_obj_properties_type(prop));

    if (NULL == ref)
        return;

    grc->ref = cdll_unshift(grc->ref, ref);
}

