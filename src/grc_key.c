
/*
 * Description: Functions to manipulate a 'struct grc_key_data' structure.
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Jul 28 09:44:58 2016
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
 * Destroy a structure 'struct grc_key_data'.
 */
void destroy_key_data(struct grc_key_data *kdata)
{
    if (kdata->name != NULL)
        free(kdata->name);

    if (kdata->key != NULL)
        free(kdata->key);

    free(kdata);
}

/*
 * Create and return a structure 'grc_key_data'. It is created and filled with
 * content from an object from an array 'keys' inside a GRC file.
 */
struct grc_key_data *new_key_data(cjson_t *key)
{
    struct grc_key_data *k = NULL;
    struct grc_json_key *e;
    char *tmp;

    k = calloc(1, sizeof(struct grc_key_data));

    if (NULL == k) {
        al_set_errno(AL_ERROR_MEMORY);
        return NULL;
    }

    /* key */
    e = get_grc_json_key(AL_GRC_JOBJ_KEY);

    if (NULL == e)
        goto undefined_grc_jkey_block;

    tmp = grc_get_object_str(key, e->name);

    if (NULL == tmp)
        goto undefined_grc_jkey_block;

    k->key = tmp;

    /* name */
    e = get_grc_json_key(AL_GRC_JOBJ_NAME);

    if (NULL == e)
        goto undefined_grc_jkey_block;

    tmp = grc_get_object_str(key, e->name);
    k->name = tmp;

    return k;

undefined_grc_jkey_block:
    al_set_errno(AL_ERROR_UNDEFINED_GRC_KEY);
    destroy_key_data(k);

    return NULL;
}

