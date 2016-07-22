
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

#ifndef _LIBALEX_PARSER_H
#define _LIBALEX_PARSER_H               1

#ifndef LIBALEX_COMPILE
# ifndef _LIBALEX_H
#  error "Never use <parser.h> directly; include <libalex.h> instead."
# endif
#endif

#ifdef LIBALEX_COMPILE
struct grc_json_key *get_grc_json_key(enum al_grc_object_property prop);
int grc_get_object_value(cjson_t *object, const char *object_name,
                         int default_value);

char *grc_get_object_str(cjson_t *object, const char *object_name);
int grc_parse_file(struct al_grc *grc, const char *grc_filename);
int grc_parse_mem(struct al_grc *grc, const char *data);
void destroy_key_data(struct grc_key_data *kdata);
struct grc_key_data *new_key_data(cjson_t *key);
void destroy_obj_properties(struct grc_obj_properties *odata);
struct grc_obj_properties *new_obj_properties(cjson_t *object);
const char *str_radio_type(enum al_grc_radio_button_fmt radio);
const char *str_horizontal_position(enum al_grc_hpos hpos);
const char *str_line_break(enum al_grc_line_break lbreak);
const char *str_grc_obj_type(enum al_grc_object obj);

void destroy_menu(void *a);
struct grc_menu *new_menu(cjson_t *object);
#endif

#endif

