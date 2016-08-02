
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

    free(p);
}

struct grc_object *new_grc_object(void)
{
    struct grc_object *p = NULL;

    p = calloc(1, sizeof(struct grc_object));

    if (NULL == p) {
        al_set_errno(AL_ERROR_MEMORY);
        return NULL;
    }

    p->dlg = calloc(1, sizeof(DIALOG));

    if (NULL == p->dlg)
        goto error_block;

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

/*
 * TODO: Add function to store a grc_generic_data node and remove the code
 *       that does this from parser.c
 */

