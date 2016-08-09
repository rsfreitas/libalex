
/*
 * Description: Functions to manipulate a 'struct grc_generic_data' structure.
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Jul 28 09:22:12 2016
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

struct grc_generic_data *new_grc_generic_data(void)
{
    struct grc_generic_data *d = NULL;

    d = calloc(1, sizeof(struct grc_generic_data));

    if (NULL == d)
        return NULL;

    return d;
}

void destroy_grc_generic_data(void *a)
{
    struct grc_generic_data *p = (struct grc_generic_data *)a;

    if (NULL == a)
        return;

    free(p);
}

