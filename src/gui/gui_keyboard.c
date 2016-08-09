
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Dec 11 15:40:59 2014
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

int gui_d_keyboard_proc(int msg, DIALOG *d, int c)
{
    struct callback_data *acd = d->dp3;
    int ret = D_O_K;

    switch (msg) {
        case MSG_START:
            d->w = d->h = 0;
            break;

        case MSG_XCHAR:
            if (((c >> 8) != d->d1) && ((c >> 8) != d->d2))
                break;

            ret |= D_USED_CHAR;

            /* fall through */

        case MSG_KEY:
            if (d->dp3 != NULL) {
                callback_set_int(acd, d->d1);
                ret |= run_callback(acd, D_O_K);
            }

            break;
    }

    return ret;
}

