
/*
 * Description: Functions to manipulate a 'struct dlg_obj_ref' structure.
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Jul 28 09:21:45 2016
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
