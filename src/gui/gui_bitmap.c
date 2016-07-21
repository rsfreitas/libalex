
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Dec 11 17:38:46 2014
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

#include "libalex.h"

int gui_d_bitmap_proc(int msg, DIALOG *d, int c __attribute__((unused)))
{
    BITMAP *b;

    switch (msg) {
        case MSG_LOAD_IMAGE:
            /* c = image_fmt */
            /* Convert image to Allegro format */
            /* Stretch_blit to BITMAP */
            return D_REDRAWME;

        case MSG_DRAW:
            if (d->dp != NULL) {
                b = (BITMAP *)d->dp;
                blit(b, gui_get_screen(), 0, 0, d->x, d->y, b->w, b->h);
                destroy_bitmap(b);
            }

            break;
    }

    return D_O_K;
}

