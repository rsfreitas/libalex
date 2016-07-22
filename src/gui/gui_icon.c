
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Mon Dec 29 10:11:13 2014
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

/*
 * This is a copy from Allegro's 'd_icon_proc' without the capability of use
 * 2 different imagens to use when selected or not.
 */
static int simple_d_icon_proc(int msg, DIALOG *d, int c)
{
    BITMAP *butimage,*gui_bmp;
    int butx, buty, index, indent, depth;

    ASSERT(d);

    butimage = (BITMAP *)d->dp;
    gui_bmp = gui_get_screen();

    if ((msg == MSG_DRAW) && (!(d->flags & D_HIDDEN))) {
        depth = 0;

        if ((d->dp2 == NULL) && (d->flags & D_SELECTED)) {
            depth = d->d1;

            if (depth < 1)
                depth = 2;
        }

        indent = d->d2;

        if (indent == 0)
            indent = 2;

        /* put the graphic on screen, scaled as needed */
        butx = butimage->w;
        buty = butimage->h;
        stretch_blit(butimage, gui_bmp, 0, 0, butx-depth, buty-depth, 
                     d->x + depth, d->y + depth, d->w - depth, d->h - depth);

        if ((d->flags & D_GOTFOCUS) &&
            (!(d->flags & D_SELECTED) || !(d->flags & D_EXIT)))
        {
            /* draw focus lines */
            for (index = indent; index < d->w - (indent + 1); index += 2) {
                putpixel(gui_bmp, d->x + index + depth, d->y + indent + depth,
                         d->fg);

                putpixel(gui_bmp, d->x + index + depth,
                         d->y + d->h - (indent + 1) + depth, d->fg);
            }

            for (index = indent; index < d->h - (indent + 1); index += 2) {
                putpixel(gui_bmp, d->x + indent + depth, d->y + index + depth,
                         d->fg);

                putpixel(gui_bmp, d->x + d->w - (indent + 1) + depth,
                         d->y + index + depth, d->fg);
            }
        }

        /* draw shadowing */
        for (index = 0; index < depth; index++) {
            hline(gui_bmp, d->x, d->y + index, d->x + d->w - 1, d->bg);
            vline(gui_bmp, d->x + index, d->y, d->y + d->h - 1, d->bg);
        }

        return D_O_K;
    }

    return d_button_proc(msg, d, c);
}

int gui_d_icon_proc(int msg, DIALOG *d, int c)
{
    struct al_callback_data *acd = d->dp3;
    int ret;

    /*
     * If the image is not passed yet we do nothing and avoid some error
     * trying to draw something invalid into the screen.
     */
    if (NULL == d->dp)
        return D_O_K;

    ret = simple_d_icon_proc(msg, d, c);

    if (ret == D_CLOSE) {
        if (acd != NULL) {
            run_callback(acd);

            /*
             * We don't let the interface shutdown if the user forgets the
             * correct return value.
             */
            ret = D_REDRAWME;
        }
    }

    return ret;
}

