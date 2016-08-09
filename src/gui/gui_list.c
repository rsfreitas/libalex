
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Sun Dec 14 00:29:57 2014
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

#include "libgrc.h"

/*
 * The user must pass a function of following type to use this object:
 *
 * char *foo(int index, int *list_size);
 *
 * so, when @index >= 0 the function must return a pointer to the string
 * which will be displayed in the list in the @index position. Otherwise
 * it must return NULL and @list_size must contain the total size of the
 * list.
 *
 * This function must be passed in @d->dp.
 */
int gui_d_list_proc(int msg, DIALOG *d, int c)
{
    struct callback_data *acd = d->dp3;
    int ret;

    ret = d_list_proc(msg, d, c);

    if (ret == D_CLOSE) {
        if (d->dp3 != NULL) {
            callback_set_int(acd, d->d1);
            run_callback(acd, D_O_K);

            /*
             * We don't let the interface shutdown if the user forgets the
             * correct return value.
             */
            ret = D_O_K;
        } else
            return D_REDRAWME;
    }

    return ret;
}

