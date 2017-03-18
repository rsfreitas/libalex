
/*
 * Description: Functions to manipulate a 'struct grc_s' structure.
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Jul 28 09:01:37 2016
 * Project: libgrc
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

#include "libgrc.h"

/*
 * Destroy a structure of type 'struct grc_s'.
 */
void destroy_grc(struct grc_s *grc)
{
    if (grc->ui_objects != NULL)
        cl_dll_free(grc->ui_objects, destroy_grc_object);

    if (grc->tmp_objects != NULL)
        cl_dll_free(grc->tmp_objects, destroy_grc_object);

    if (grc->ui_keys != NULL)
        cl_dll_free(grc->ui_keys, destroy_grc_object);

    if (grc->dlg != NULL)
        free(grc->dlg);

    if (grc->jgrc != NULL)
        cl_json_delete(grc->jgrc);

    if (grc->info != NULL)
        info_finish(grc->info);

    if (grc->color != NULL)
        color_finish(grc->color);

    grc_release_internal_data(grc);
    free(grc);
}

/*
 * Create and return a structure of type 'struct grc_s'.
 */
struct grc_s *new_grc(void)
{
    struct grc_s *g;

    g = calloc(1, sizeof(struct grc_s));

    if (NULL == g) {
        grc_set_errno(GRC_ERROR_MEMORY);
        return NULL;
    }

    g->jgrc = NULL;
    g->dlg = NULL;
    g->ui_objects = NULL;
    g->tmp_objects = NULL;
    g->ui_keys = NULL;
    g->ui_menu = NULL;

    g->info = info_start();

    if (NULL == g->info) {
        free(g);
        return NULL;
    }

    g->color = color_start();

    if (NULL == g->color) {
        info_finish(g->info);
        free(g);
        return NULL;
    }

    /*
     * Let the virtual keyboard disabled by now. If there is such an object
     * this flag will be enabled later.
     */
    info_set_value(g->info, INFO_VIRTUAL_KEYBOARD, false, NULL);

    /*
     * We're not prepared to run de DIALOG yet cause we need to load all
     * objects from the file and translate them to Allegro.
     */
    info_set_value(g->info, INFO_ARE_WE_PREPARED, false, NULL);

    return g;
}

DIALOG *grc_get_DIALOG_from_tag(struct grc_s *grc, const char *tag)
{
    struct tmp_list {
        struct grc_object_s *ui;
    };

    DIALOG *d = NULL;
    unsigned int i, t;
    struct grc_object_s *p;
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

MENU *grc_get_MENU_from_tag(struct grc_s *grc, const char *tag)
{
    struct tmp_list {
        struct grc_object_s *ui;
    };

    MENU *m = NULL;
    unsigned int i, t;
    struct grc_object_s *p;
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

struct gfx_info_s *grc_get_info(struct grc_s *grc)
{
    if (NULL == grc)
        return NULL;

    return grc->info;
}

cl_json_t *grc_get_JSON(struct grc_s *grc)
{
    if (NULL == grc)
        return NULL;

    return grc->jgrc;
}

int grc_set_JSON(struct grc_s *grc, cl_json_t *json)
{
    if (NULL == grc)
        return -1;

    grc->jgrc = json;

    return 0;
}

void *grc_get_internal_data(struct grc_s *grc)
{
    if (NULL == grc)
        return NULL;

    return grc->internal;
}

int grc_set_internal_data(struct grc_s *grc, void *ptr,
    void (*free_internal)(void *))
{
    if (NULL == grc)
        return -1;

    grc->internal = ptr;
    grc->free_internal = free_internal;

    return 0;
}

void grc_release_internal_data(struct grc_s *grc)
{
    if (NULL == grc)
        return;

    if (grc->internal != NULL)
        if (grc->free_internal != NULL)
            (grc->free_internal)(grc->internal);
}

