
/*
 * Description: Functions to manipulate a 'struct al_callback_data' structure.
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Jul 28 09:21:58 2016
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

void destroy_callback_data(void *a)
{
    struct al_callback_data *p = (struct al_callback_data *)a;

    if (NULL == p)
        return;

    free(p);
}

