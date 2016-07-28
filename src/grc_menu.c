
/*
 * Description: Functions to manipulate a 'struct grc_menu' structure.
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Jul 28 09:45:03 2016
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

void destroy_grc_menu(void *a)
{
    struct grc_menu *menu = (struct grc_menu *)a;

    if (menu->items != NULL)
        cdll_free(menu->items, destroy_grc_menu);

    if (menu->parent != NULL)
        free(menu->parent);

    if (menu->name != NULL)
        free(menu->name);

    if (menu->text != NULL)
        free(menu->text);

    free(menu);
}

struct grc_menu *new_menu(cjson_t *object)
{
    struct grc_json_key *e;
    struct grc_menu *m, *it;
    char *tmp;
    cjson_t *o, *n;
    int i, t;

    m = calloc(1, sizeof(struct grc_menu));

    if (NULL == m)
        return NULL;

    m->parent = NULL;
    m->items = NULL;

    /* name */
    e = get_grc_json_key(AL_GRC_JOBJ_NAME);

    if (e != NULL) {
        tmp = grc_get_object_str(object, e->name);

        if (tmp != NULL)
            m->name = tmp;
    }

    /* parent */
    e = get_grc_json_key(AL_GRC_JOBJ_PARENT);

    if (e != NULL) {
        tmp = grc_get_object_str(object, e->name);

        if (tmp != NULL)
            m->parent = tmp;
    }

    /* text */
    e = get_grc_json_key(AL_GRC_JOBJ_TEXT);

    if (e != NULL) {
        tmp = grc_get_object_str(object, e->name);

        if (tmp != NULL)
            m->text = tmp;
    }

    /* options (items) */
    o = cjson_get_object_item(object, OBJ_OPTIONS);

    if (o != NULL) {
        t = cjson_get_array_size(o);

        for (i = 0; i < t; i++) {
            n = cjson_get_array_item(o, i);

            it = new_menu(n);
            m->items = cdll_unshift(m->items, it);
        }
    }

    return m;
}

