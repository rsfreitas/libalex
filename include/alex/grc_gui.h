
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Mon Dec 15 11:27:47 2014
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

#ifndef _LIBALEX_GRC_GUI_H
#define _LIBALEX_GRC_GUI_H			1

#ifndef LIBALEX_COMPILE
# ifndef _LIBALEX_H
#  error "Nao inclua o arquivo <grc_gui.h> diretamente; use o arquivo <libalex.h>"
# endif
#endif

#ifdef LIBALEX_COMPILE
int gui_change_resolution(struct al_grc *grc);
int gui_load_colors(struct al_grc *grc);
void gui_reset_resolution(void);
int create_DIALOG(struct al_grc *grc);
void run_DIALOG(struct al_grc *grc);
int run_callback(struct al_callback_data *acd);

int grc_tr_color_to_al_color(int color_depth, const char *color);
DIALOG *get_DIALOG_from_grc(struct al_grc *grc, const char *object_name);
struct grc_menu *get_grc_menu_from_grc(struct al_grc *grc,
                                       const char *object_name);

MENU *get_MENU_from_grc(struct al_grc *grc, const char *object_name);
#endif

#endif

