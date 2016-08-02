
/*
 * Description: Functions to manipulate a 'struct grc_obj_properties'
 *              structure.
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Jul 28 09:45:08 2016
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

#include <stdlib.h>

#include "libalex.h"

/* Structure to store an object "object" of a GRC file */
struct grc_obj_properties {
    enum al_grc_object  type;
    char                *name;
    char                *parent;
    char                *text;
    char                *fg;
    int                 x;
    int                 y;
    int                 w;
    int                 h;
    bool                hide;
    int                 line_break_mode;
    int                 data_length;
    int                 radio_group;
    int                 radio_type;
    int                 password_mode;
    int                 horizontal_position;
    int                 fps;
    int                 devices;
};

/*
 * Destroy a structure 'struct grc_obj_properties'.
 */
void destroy_obj_properties(struct grc_obj_properties *prop)
{
    if (prop->parent != NULL)
        free(prop->parent);

    if (prop->name != NULL)
        free(prop->name);

    if (prop->text != NULL)
        free(prop->text);

    if (prop->fg != NULL)
        free(prop->fg);

    free(prop);
}

/*
 * Create and return a structure 'struct grc_obj_properties'. It is created
 * and filled with an object content from 'objects' array inside a GRC file.
 */
struct grc_obj_properties *new_obj_properties(cjson_t *object)
{
    struct grc_obj_properties *p = NULL;
    struct grc_json_key *e;
    char *tmp;

    p = calloc(1, sizeof(struct grc_obj_properties));

    if (NULL == p) {
        al_set_errno(AL_ERROR_MEMORY);
        return NULL;
    }

    /* type (obrigatorio) */
    e = get_grc_json_key(AL_GRC_JOBJ_TYPE);

    if (NULL == e)
        goto undefined_grc_jkey_block;

    tmp = grc_get_object_str(object, e->name);
    p->type = tr_str_type_to_grc_type(tmp);
    free(tmp);

    /* name */
    e = get_grc_json_key(AL_GRC_JOBJ_NAME);

    if (NULL == e)
        goto undefined_grc_jkey_block;

    tmp = grc_get_object_str(object, e->name);

    if (tmp != NULL)
        p->name = tmp;
    else
        p->name = NULL;

    /* parent */
    e = get_grc_json_key(AL_GRC_JOBJ_PARENT);

    if (NULL == e)
        goto undefined_grc_jkey_block;

    tmp = grc_get_object_str(object, e->name);

    if (tmp != NULL)
        p->parent = tmp;
    else
        p->parent = NULL;

    /* text */
    e = get_grc_json_key(AL_GRC_JOBJ_TEXT);

    if (NULL == e)
        goto undefined_grc_jkey_block;

    tmp = grc_get_object_str(object, e->name);

    if (tmp != NULL)
        p->text = tmp;
    else
        p->text = NULL;

    /* foreground */
    e = get_grc_json_key(AL_GRC_JOBJ_FOREGROUND);

    if (NULL == e)
        goto undefined_grc_jkey_block;

    tmp = grc_get_object_str(object, e->name);

    if (tmp != NULL)
        p->fg = tmp;

    /* pos_x */
    e = get_grc_json_key(AL_GRC_JOBJ_POS_X);

    if (NULL == e)
        goto undefined_grc_jkey_block;

    p->x = grc_get_object_value(object, e->name, -1);

    /* pos_y */
    e = get_grc_json_key(AL_GRC_JOBJ_POS_Y);

    if (NULL == e)
        goto undefined_grc_jkey_block;

    p->y = grc_get_object_value(object, e->name, -1);

    /* width */
    e = get_grc_json_key(AL_GRC_JOBJ_WIDTH);

    if (NULL == e)
        goto undefined_grc_jkey_block;

    p->w = grc_get_object_value(object, e->name, -1);

    /* height */
    e = get_grc_json_key(AL_GRC_JOBJ_HEIGHT);

    if (NULL == e)
        goto undefined_grc_jkey_block;

    p->h = grc_get_object_value(object, e->name, -1);

    /* hide */
    e = get_grc_json_key(AL_GRC_JOBJ_HIDE);

    if (NULL == e)
        goto undefined_grc_jkey_block;

    p->hide = grc_get_object_value(object, e->name, false);

    /* line_break */
    e = get_grc_json_key(AL_GRC_JOBJ_LINE_BREAK);

    if (NULL == e)
        goto undefined_grc_jkey_block;

    tmp = grc_get_object_str(object, e->name);
    p->line_break_mode = tr_line_break(tmp);
    free(tmp);

    /* input_length */
    e = get_grc_json_key(AL_GRC_JOBJ_INPUT_LENGTH);

    if (NULL == e)
        goto undefined_grc_jkey_block;

    p->data_length = grc_get_object_value(object, e->name, 0);

    /* radio_group */
    e = get_grc_json_key(AL_GRC_JOBJ_RADIO_GROUP);

    if (NULL == e)
        goto undefined_grc_jkey_block;

    p->radio_group = grc_get_object_value(object, e->name, 0);

    /* radio_type */
    e = get_grc_json_key(AL_GRC_JOBJ_RADIO_TYPE);

    if (NULL == e)
        goto undefined_grc_jkey_block;

    tmp = grc_get_object_str(object, e->name);
    p->radio_type = tr_radio_type(tmp);
    free(tmp);

    /* password */
    e = get_grc_json_key(AL_GRC_JOBJ_PASSWORD_MODE);

    if (NULL == e)
        goto undefined_grc_jkey_block;

    p->password_mode = grc_get_object_value(object, e->name, false);

    /* horizontal position */
    e = get_grc_json_key(AL_GRC_JOBJ_H_POSITION);

    if (NULL == e)
        goto undefined_grc_jkey_block;

    tmp = grc_get_object_str(object, e->name);
    p->horizontal_position = tr_horizontal_position(tmp);
    free(tmp);

    /* fps */
    e = get_grc_json_key(AL_GRC_JOBJ_FPS);

    if (NULL == e)
        goto undefined_grc_jkey_block;

    p->fps = grc_get_object_value(object, e->name, 0);

    /* devices */
    e = get_grc_json_key(AL_GRC_JOBJ_DEVICES);

    if (NULL == e)
        goto undefined_grc_jkey_block;

    p->devices = grc_get_object_value(object, e->name, 1);

    return p;

undefined_grc_jkey_block:
    al_set_errno(AL_ERROR_UNDEFINED_GRC_KEY);
    destroy_obj_properties(p);

    return NULL;
}

bool grc_obj_properties_has_name(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return false;

    if (NULL == prop->name)
        return false;

    return true;
}

bool grc_obj_properties_has_parent(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return false;

    if (NULL == prop->parent)
        return false;

    return true;
}

bool grc_obj_properties_has_fg(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return false;

    if (NULL == prop->fg)
        return false;

    return true;
}

const char *grc_obj_properties_name(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return NULL;

    return prop->name;
}

const char *grc_obj_properties_parent(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return NULL;

    return prop->parent;
}

const char *grc_obj_properties_text(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return NULL;

    return prop->text;
}

const char *grc_obj_properties_fg(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return NULL;

    return prop->fg;
}

enum al_grc_object grc_obj_properties_type(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return -1;

    return prop->type;
}

int grc_obj_properties_x(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return -1;

    return prop->x;
}

int grc_obj_properties_y(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return -1;

    return prop->y;
}

int grc_obj_properties_w(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return -1;

    return prop->w;
}

int grc_obj_properties_h(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return -1;

    return prop->h;
}

bool grc_obj_properties_hide(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return false;

    return prop->hide;
}

int grc_obj_properties_line_break_mode(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return -1;

    return prop->line_break_mode;
}

int grc_obj_properties_data_length(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return -1;

    return prop->data_length;
}

int grc_obj_properties_radio_group(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return -1;

    return prop->radio_group;
}

int grc_obj_properties_radio_type(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return -1;

    return prop->radio_type;
}

int grc_obj_properties_password_mode(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return -1;

    return prop->password_mode;
}

int grc_obj_properties_horizontal_position(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return -1;

    return prop->horizontal_position;
}

