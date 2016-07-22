
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Sat Dec 13 21:05:43 2014
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
 * Function like a d_edit_proc from Allegro, @dp2 is the value displayed
 * with asterisks (*), @dp is the real value.
 */
static int d_password_proc(int msg, DIALOG *d, int c)
{
    static int ignore_next_uchar = FALSE;
    int f, l, p, w, x, fg, b = 0, scroll;
    char buf[16], *s, *dp2;

    ASSERT(d);

    s = d->dp2;
    l = ustrlen(s);

    if (d->d2 > l)
        d->d2 = l;

    /* Its size is the same as len(s) */
    dp2 = d->dp;

    /* calculate maximal number of displayable characters */
    if (d->d2 == l) {
        usetc(buf + usetc(buf, ' '), 0);
        x = text_length(font, buf);
    } else
        x = 0;

    for (p = d->d2; p >= 0; p--) {
        usetc(buf + usetc(buf, ugetat(s, p)), 0);
        x += text_length(font, buf);
        b++;

        if (x > d->w)
            break;
    }

    if (x <= d->w) {
        b = l;
        scroll = FALSE;
    } else {
        b--;
        scroll = TRUE;
    }

    switch (msg) {
        case MSG_START:
            d->d2 = l;
            break;

        case MSG_DRAW:
            fg = (d->flags & D_DISABLED) ? gui_mg_color : d->fg;
            x = 0;

            if (scroll) {
                p = d->d2-b + 1;
                b = d->d2;
            } else
                p = 0;

            for (; p <= b; p++) {
                f = ugetat(s, p);
                usetc(buf + usetc(buf, (f) ? f : ' '), 0);
                w = text_length(font, buf);

                if (x + w > d->w)
                    break;

                f = ((p == d->d2) && (d->flags & D_GOTFOCUS));

                textout_ex(screen, font, buf, d->x + x, d->y, (f) ? d->bg : fg,
                           (f) ? fg : d->bg);

                x += w;
            }

            if (x < d->w)
                rectfill(screen, d->x + x, d->y, d->x + d->w - 1,
                         d->y + text_height(font) - 1, d->bg);

            break;

        case MSG_WANTFOCUS:
        case MSG_LOSTFOCUS:
        case MSG_KEY:
            return D_WANTFOCUS;

        case MSG_CHAR:
            ignore_next_uchar = FALSE;

            if ((c >> 8) == KEY_LEFT) {
                if (d->d2 > 0)
                    d->d2--;
            } else if ((c >> 8) == KEY_RIGHT) {
                if (d->d2 < l)
                    d->d2++;
            } else if ((c >> 8) == KEY_HOME) {
                d->d2 = 0;
            } else if ((c >> 8) == KEY_END) {
                d->d2 = l;
            } else if ((c >> 8) == KEY_DEL) {
                if (d->d2 < l) {
                    uremove(s, d->d2);
                    uremove(dp2, d->d2);
                }
            } else if ((c >> 8) == KEY_BACKSPACE) {
                if (d->d2 > 0) {
                    d->d2--;
                    uremove(s, d->d2);
                    uremove(dp2, d->d2);
                }
            } else if ((c >> 8) == KEY_ENTER) {
                if (d->flags & D_EXIT) {
                    object_message(d, MSG_DRAW, 0);
                    return D_CLOSE;
                } else
                    return D_O_K;
            } else if ((c >> 8) == KEY_TAB) {
                ignore_next_uchar = TRUE;
                return D_O_K;
            } else {
                /* don't process regular keys here: MSG_UCHAR will do that */
                break;
            }

            object_message(d, MSG_DRAW, 0);
            return D_USED_CHAR;

        case MSG_UCHAR:
            if ((c >= ' ') && (uisok(c)) && (!ignore_next_uchar)) {
                if (l < d->d1) {
                    uinsert(s, d->d2, '*');
                    uinsert(dp2, d->d2, c);

                    d->d2++;

                    object_message(d, MSG_DRAW, 0);
                }

                return D_USED_CHAR;
            }
            break;
    }

    return D_O_K;
}

static void set_actual_edit_object(DIALOG *d, struct al_callback_data *acd)
{
    struct al_grc *grc;

    if (NULL == acd)
        return;

    grc = acd->grc;

    if (NULL == grc)
        return;

    if (grc->virtual_keyboard == AL_FALSE)
        return;

    /*
     * Saves a reference to the object so the virtual keyboard may access it.
     */
    grc->last_edit_object = d;
}

int gui_d_password_proc(int msg, DIALOG *d, int c)
{
    struct al_callback_data *acd = d->dp3;
    int ret;

    /* Received an update from the virtual keyboard? */
    if (msg == MSG_UPDATE_CURSOR_POSITION)
        d->d2 = c;

    ret = d_password_proc(msg, d, c);

    if (msg == MSG_LOSTFOCUS)
        set_actual_edit_object(d, acd);

    if (ret == D_CLOSE) {
        if (d->dp3 != NULL) {
            acd->value_string = (char *)d->dp;
            run_callback(acd);

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

int gui_d_edit_proc(int msg, DIALOG *d, int c)
{
    struct al_callback_data *acd = d->dp3;
    int ret;

    /* Received an update from the virtual keyboard? */
    if (msg == MSG_UPDATE_CURSOR_POSITION)
        d->d2 = c;

    ret = d_edit_proc(msg, d, c);

    /*
     * If the user passes over the object we let it selected so the
     * virtual keyboard edit it.
     */
    if ((msg == MSG_LOSTFOCUS) || (msg == MSG_GOTMOUSE))
        set_actual_edit_object(d, acd);

    if (ret == D_CLOSE) {
        if (d->dp3 != NULL) {
            acd->value_string = (char *)d->dp;
            run_callback(acd);

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

