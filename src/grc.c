
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
    if (grc->ui_objects != NULL)
        cdll_free(grc->ui_objects, destroy_grc_object);

    if (grc->tmp_objects != NULL)
        cdll_free(grc->tmp_objects, destroy_grc_object);

    if (grc->ui_keys != NULL)
        cdll_free(grc->ui_keys, destroy_grc_object);

    if (grc->al_dlg != NULL)
        free(grc->al_dlg);

    if (grc->jgrc != NULL)
        cjson_delete(grc->jgrc);

    if (grc->info != NULL)
        info_finish(grc->info);

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
    g->al_dlg = NULL;
    g->ui_objects = NULL;
    g->tmp_objects = NULL;
    g->ui_keys = NULL;
    g->ui_menu = NULL;

    g->info = info_start();

    if (NULL == g->info) {
        free(g);
        return NULL;
    }

    /*
     * Let the virtual keyboard disabled by now. If there is such an object
     * this flag will be enabled later.
     */
    info_set_value(g->info, AL_INFO_VIRTUAL_KEYBOARD, false, NULL);

    /*
     * We're not prepared to run de DIALOG yet cause we need to load all
     * objects from the file and translate them to Allegro.
     */
    info_set_value(g->info, AL_INFO_ARE_WE_PREPARED, false, NULL);

    return g;
}

DIALOG *grc_get_DIALOG_from_tag(struct al_grc *grc, const char *tag)
{
    struct tmp_list {
        struct grc_object *ui;
    };

    DIALOG *d = NULL;
    unsigned int i, t;
    struct grc_object *p;
    struct tmp_list ui_list[] = {
        { grc->ui_objects },
        { grc->ui_keys },
        { grc->ui_menu },
        { grc->tmp_objects }
    };

    t = sizeof(ui_list) / sizeof(ui_list[0]);

    for (i = 0; i < t; i++) {
        p = ui_list[i].ui;
        d = grc_object_get_DIALOG_from_tag(p, tag);

        if (d != NULL)
            return d;
    }

    return NULL;
}

MENU *grc_get_MENU_from_tag(struct al_grc *grc, const char *tag)
{
    struct tmp_list {
        struct grc_object *ui;
    };

    MENU *m = NULL;
    unsigned int i, t;
    struct grc_object *p;
    struct tmp_list ui_list[] = {
        { grc->ui_menu },
        { grc->tmp_objects }
    };

    t = sizeof(ui_list) / sizeof(ui_list[0]);

    for (i = 0; i < t; i++) {
        p = ui_list[i].ui;
        m = grc_object_get_MENU_from_tag(p, tag);

        if (m != NULL)
            return m;
    }

    return NULL;
}

