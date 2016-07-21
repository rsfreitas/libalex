
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Sun Aug 24 21:05:01 2014
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

#ifndef _LIBALEX_COMMON_H
#define _LIBALEX_COMMON_H                            1

#ifndef LIBALEX_COMPILE
# ifndef _LIBALEX_H
#  error "Nao inclua o arquivo <common.h> diretamente; use o arquivo <libalex.h>"
# endif
#endif

#ifdef LIBALEX_COMPILE
struct dlg_obj_ref *new_obj_ref(const char *name, int dlg_index,
                                enum al_grc_object type);

void destroy_obj_ref(void *a);
struct al_grc *new_grc(void);
void destroy_grc(struct al_grc *grc);
struct al_callback_data *new_callback_data(void);
struct grc_generic_data *new_grc_generic_data(void);
void dotted_rect(int x1, int y1, int x2, int y2, int fg, int bg);
#endif

#endif

