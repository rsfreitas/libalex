
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Mon Dec 29 11:41:01 2014
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

int gui_d_textbox_proc(int msg, DIALOG *d, int c)
{
    /*
     * In case the text is not passed yet we do nothing and avoid draw
     * something invalid into the screen.
     */
    if (NULL == d->dp)
        return D_O_K;

    return d_textbox_proc(msg, d, c);
}

