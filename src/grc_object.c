
/*
 * Description: Functions to manipulate a 'struct grc_object' structure.
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Jul 28 14:10:38 2016
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

void destroy_grc_object(void *a)
{
    struct grc_object *p = (struct grc_object *)a;

    if (NULL == p)
        return;

    /* Destroy Allegro's DIALOG */
    if (p->dlg != NULL)
        free(p->dlg);

    /* Destroy object properties */
    if (p->prop != NULL)
        destroy_obj_properties(p->prop);

    /* Destroy object's callback data */
    if (p->cb_data != NULL)
        destroy_callback_data(p->cb_data);

    if (p->tag != NULL)
        free(p->tag);

    free(p);
}

struct grc_object *new_grc_object(enum grc_object_type type)
{
    struct grc_object *p = NULL;

    p = calloc(1, sizeof(struct grc_object));

    if (NULL == p) {
        al_set_errno(AL_ERROR_MEMORY);
        return NULL;
    }

    if ((type == GRC_OBJ_STANDARD) || (type == GRC_OBJ_KEY)) {
        p->dlg = calloc(1, sizeof(DIALOG));

        if (NULL == p->dlg)
            goto error_block;
    } else if (type == GRC_OBJ_MENU_ITEM) {
        p->menu = calloc(1, sizeof(MENU));

        if (NULL == p->menu)
            goto error_block;
    }

    p->type = type;

    return p;

error_block:
    destroy_grc_object(p);
    return NULL;
}

struct grc_obj_properties *grc_object_get_properties(struct grc_object *object)
{
    if (NULL == object)
        return NULL;

    return object->prop;
}

DIALOG *grc_object_get_DIALOG(struct grc_object *object)
{
    if (NULL == object)
        return NULL;

    return object->dlg;
}

MENU *grc_object_get_MENU(struct grc_object *object)
{
    if (NULL == object)
        return NULL;

    return object->menu;
}

void grc_object_set_MENU(struct grc_object *object, MENU *menu)
{
    if (NULL == object)
        return;

    object->menu = menu;
}

void grc_object_set_tag(struct grc_object *object, const char *tag)
{
    if ((NULL == object) || (NULL == tag))
        return;

    /* FIXME: Do we really need to dup this? */
    object->tag = strdup(tag);
}

static int search_object_by_tag(void *a, void *b)
{
    struct grc_object *o = (struct grc_object *)a;
    const char *tag = (const char *)b;

    if (strcmp(o->tag, tag) == 0)
        return 1;

    return 0;
}

DIALOG *grc_object_get_DIALOG_from_tag(struct grc_object *object,
    const char *tag)
{
    struct grc_object *o = NULL;

    if (NULL == object) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return NULL;
    }

    o = cdll_map(object, search_object_by_tag, (void *)tag);

    if (NULL == o) {
        al_set_errno(AL_ERROR_OBJECT_NOT_FOUND);
        return NULL;
    }

    return grc_object_get_DIALOG(o);
}

MENU *grc_object_get_MENU_from_tag(struct grc_object *object, const char *tag)
{
    struct grc_object *o = NULL;

    if (NULL == object) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return NULL;
    }

    o = cdll_map(object, search_object_by_tag, (void *)tag);

    if (NULL == o) {
        al_set_errno(AL_ERROR_OBJECT_NOT_FOUND);
        return NULL;
    }

    return grc_object_get_MENU(o);
}

/*
 * TODO: Add function to store a grc_generic_data node and remove the code
 *       that does this from parser.c
 */

