
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Jul 21 15:57:15 2016
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

#ifndef _LIBALEX_OBJECTS_H
#define _LIBALEX_OBJECTS_H              1

/* gui_bitmap.c */
int gui_d_bitmap_proc(int msg, DIALOG *d, int c);

/* gui_button.c */
int gui_d_button_proc(int msg, DIALOG *d, int c);

/* gui_check.c */
int gui_d_check_proc(int msg, DIALOG *d, int c);

/* gui_clock.c */
char *gui_clock_proc_str(void);
int gui_clock_proc(int msg, DIALOG *d, int c);

/* gui_edit.c */
int gui_d_edit_proc(int msg, DIALOG *d, int c);
int gui_d_password_proc(int msg, DIALOG *d, int c);

/* gui_icon.c */
int gui_d_icon_proc(int msg, DIALOG *d, int c);

/* gui_keyboard.c */
int gui_d_keyboard_proc(int msg, DIALOG *d, int c);

/* gui_list.c */
int gui_d_list_proc(int msg, DIALOG *d, int c);

/* gui_messages_log_box.c */
int gui_messages_log_proc(int msg, DIALOG *d, int c);
void gui_messages_set(enum grc_line_break lbreak, int fg_color,
                      const char *msg, const char *color);

/* gui_radio.c */
int gui_d_radio_proc(int msg, DIALOG *d, int c);

/* gui_slider.c */
int gui_d_slider_proc(int msg, DIALOG *d, int c);

/* gui_textbox.c */
int gui_d_textbox_proc(int msg, DIALOG *d, int c);

/* gui_vt_keyboard.c */
int gui_d_vt_keyboard_proc(int msg, DIALOG *d, int c);

#endif

