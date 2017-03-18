
/*
 * Description: Functions to manipulate a 'struct grc_obj_properties'
 *              structure that holds every known GRC object properties.
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Jul 28 09:45:08 2016
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

#include <stdlib.h>

#include "libgrc.h"

/* Structure to hold a property detail, such as its name, type, etc */
struct property_detail {
    char                        string[128];
    enum grc_object_property    prop;
    enum grc_entry_type_value   type;
};

/* Structure to store an object "object" of a GRC file */
struct grc_obj_properties {
    enum grc_object     type;
    cl_string_t         *name;
    cl_string_t         *parent;
    cl_string_t         *text;
    cl_string_t         *fg;
    cl_string_t         *key;
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

/* Supported properties from an object of a GRC file */
static struct property_detail __properties[] = {
    { OBJ_WIDTH,                GRC_PROPERTY_WIDTH,              GRC_NUMBER  },
    { OBJ_HEIGHT,               GRC_PROPERTY_HEIGHT,             GRC_NUMBER  },
    { OBJ_COLOR_DEPTH,          GRC_PROPERTY_COLOR_DEPTH,        GRC_NUMBER  },
    { OBJ_FOREGROUND,           GRC_PROPERTY_FOREGROUND,         GRC_STRING  },
    { OBJ_BACKGROUND,           GRC_PROPERTY_BACKGROUND,         GRC_STRING  },
    { OBJ_BLOCK_EXIT_KEYS,      GRC_PROPERTY_BLOCK_EXIT_KEYS,    GRC_BOOL    },
    { OBJ_MOUSE,                GRC_PROPERTY_MOUSE,              GRC_BOOL    },
    { OBJ_TYPE,                 GRC_PROPERTY_TYPE,               GRC_STRING  },
    { OBJ_POS_X,                GRC_PROPERTY_POS_X,              GRC_NUMBER  },
    { OBJ_POS_Y,                GRC_PROPERTY_POS_Y,              GRC_NUMBER  },
    { OBJ_TAG,                  GRC_PROPERTY_TAG,                GRC_STRING  },
    { OBJ_PARENT,               GRC_PROPERTY_PARENT,             GRC_STRING  },
    { OBJ_KEY,                  GRC_PROPERTY_KEY,                GRC_STRING  },
    { OBJ_TEXT,                 GRC_PROPERTY_TEXT,               GRC_STRING  },
    { OBJ_HIDE,                 GRC_PROPERTY_HIDE,               GRC_BOOL    },
    { OBJ_LINE_BREAK,           GRC_PROPERTY_LINE_BREAK,         GRC_STRING  },
    { OBJ_IGNORE_ESC_KEY,       GRC_PROPERTY_IGNORE_ESC_KEY,     GRC_BOOL    },
    { OBJ_INPUT_LENGTH,         GRC_PROPERTY_INPUT_LENGTH,       GRC_NUMBER  },
    { OBJ_RADIO_GROUP,          GRC_PROPERTY_RADIO_GROUP,        GRC_NUMBER  },
    { OBJ_RADIO_TYPE,           GRC_PROPERTY_RADIO_TYPE,         GRC_STRING  },
    { OBJ_PASSWORD,             GRC_PROPERTY_PASSWORD_MODE,      GRC_BOOL    },
    { OBJ_HORIZONTAL_POSITION,  GRC_PROPERTY_H_POSITION,         GRC_STRING  },
    { OBJ_FPS,                  GRC_PROPERTY_FPS,                GRC_NUMBER  },
    { OBJ_DEVICES,              GRC_PROPERTY_DEVICES,            GRC_NUMBER  }
};

#define MAX_PROPERTIES              \
    (sizeof(__properties) / sizeof(__properties[0]))

/*
 * Search for an information structure from a JSON object in the supported
 * objects list.
 */
struct property_detail *get_property_detail(enum grc_object_property prop)
{
    unsigned int i;
    struct property_detail *e = NULL;

    for (i = 0; i < MAX_PROPERTIES; i++)
        if (__properties[i].prop == prop) {
            e = &__properties[i];
            break;
        }

    return e;
}

const char *property_detail_string(struct property_detail *d)
{
    if (NULL == d)
        return NULL;

    return d->string;
}

enum grc_object_property property_detail(struct property_detail *d)
{
    if (NULL == d)
        return -1;

    return d->prop;
}

enum grc_entry_type_value propery_detail_type(struct property_detail *d)
{
    if (NULL == d)
        return -1;

    return d->type;
}

/*
 * Destroy a structure 'struct grc_obj_properties'.
 */
void destroy_obj_properties(struct grc_obj_properties *prop)
{
    if (prop->parent != NULL)
        cl_string_unref(prop->parent);

    if (prop->name != NULL)
        cl_string_unref(prop->name);

    if (prop->text != NULL)
        cl_string_unref(prop->text);

    if (prop->fg != NULL)
        cl_string_unref(prop->fg);

    free(prop);
}

/*
 * Create and return an empty structure 'struct grc_obj_properties'.
 */
static struct grc_obj_properties *new_empty_obj_properties(void)
{
    struct grc_obj_properties *p = NULL;

    p = calloc(1, sizeof(struct grc_obj_properties));

    if (NULL == p) {
        grc_set_errno(GRC_ERROR_MEMORY);
        return NULL;
    }

    return p;
}

/*
 * Create and return a structure 'struct grc_obj_properties'. It is created
 * and filled with an object content from 'objects' array inside a GRC file.
 */
struct grc_obj_properties *new_obj_properties(cl_json_t *object)
{
    struct grc_obj_properties *p = NULL;
    struct property_detail *dt;
    cl_string_t *tmp;

    p = new_empty_obj_properties();

    /* type */
    dt = get_property_detail(GRC_PROPERTY_TYPE);

    if (NULL == dt)
        goto undefined_grc_jkey_block;

    tmp = grc_get_object_str(object, property_detail_string(dt));
    p->type = tr_str_type_to_grc_type(cl_string_valueof(tmp));
    free(tmp);

    /* tag */
    dt = get_property_detail(GRC_PROPERTY_TAG);

    if (NULL == dt)
        goto undefined_grc_jkey_block;

    tmp = grc_get_object_str(object, property_detail_string(dt));

    if (tmp != NULL)
        p->name = tmp;
    else
        p->name = NULL;

    /* parent */
    dt = get_property_detail(GRC_PROPERTY_PARENT);

    if (NULL == dt)
        goto undefined_grc_jkey_block;

    tmp = grc_get_object_str(object, property_detail_string(dt));

    if (tmp != NULL)
        p->parent = tmp;
    else
        p->parent = NULL;

    /* text */
    dt = get_property_detail(GRC_PROPERTY_TEXT);

    if (NULL == dt)
        goto undefined_grc_jkey_block;

    tmp = grc_get_object_str(object, property_detail_string(dt));

    if (tmp != NULL)
        p->text = tmp;
    else
        p->text = NULL;

    /* key */
    dt = get_property_detail(GRC_PROPERTY_KEY);

    if (NULL == dt)
        goto undefined_grc_jkey_block;

    tmp = grc_get_object_str(object, property_detail_string(dt));

    if (tmp != NULL)
        p->key = tmp;
    else
        p->key = NULL;

    /* foreground */
    dt = get_property_detail(GRC_PROPERTY_FOREGROUND);

    if (NULL == dt)
        goto undefined_grc_jkey_block;

    tmp = grc_get_object_str(object, property_detail_string(dt));

    if (tmp != NULL)
        p->fg = tmp;

    /* pos_x */
    dt = get_property_detail(GRC_PROPERTY_POS_X);

    if (NULL == dt)
        goto undefined_grc_jkey_block;

    p->x = grc_get_object_value(object, property_detail_string(dt), -1);

    /* pos_y */
    dt = get_property_detail(GRC_PROPERTY_POS_Y);

    if (NULL == dt)
        goto undefined_grc_jkey_block;

    p->y = grc_get_object_value(object, property_detail_string(dt), -1);

    /* width */
    dt = get_property_detail(GRC_PROPERTY_WIDTH);

    if (NULL == dt)
        goto undefined_grc_jkey_block;

    p->w = grc_get_object_value(object, property_detail_string(dt), -1);

    /* height */
    dt = get_property_detail(GRC_PROPERTY_HEIGHT);

    if (NULL == dt)
        goto undefined_grc_jkey_block;

    p->h = grc_get_object_value(object, property_detail_string(dt), -1);

    /* hide */
    dt = get_property_detail(GRC_PROPERTY_HIDE);

    if (NULL == dt)
        goto undefined_grc_jkey_block;

    p->hide = grc_get_object_value(object, property_detail_string(dt), false);

    /* line_break */
    dt = get_property_detail(GRC_PROPERTY_LINE_BREAK);

    if (NULL == dt)
        goto undefined_grc_jkey_block;

    tmp = grc_get_object_str(object, property_detail_string(dt));
    p->line_break_mode = tr_line_break(cl_string_valueof(tmp));
    free(tmp);

    /* input_length */
    dt = get_property_detail(GRC_PROPERTY_INPUT_LENGTH);

    if (NULL == dt)
        goto undefined_grc_jkey_block;

    p->data_length = grc_get_object_value(object, property_detail_string(dt),
                                          0);

    /* radio_group */
    dt = get_property_detail(GRC_PROPERTY_RADIO_GROUP);

    if (NULL == dt)
        goto undefined_grc_jkey_block;

    p->radio_group = grc_get_object_value(object, property_detail_string(dt),
                                          0);

    /* radio_type */
    dt = get_property_detail(GRC_PROPERTY_RADIO_TYPE);

    if (NULL == dt)
        goto undefined_grc_jkey_block;

    tmp = grc_get_object_str(object, property_detail_string(dt));
    p->radio_type = tr_radio_type(cl_string_valueof(tmp));
    free(tmp);

    /* password */
    dt = get_property_detail(GRC_PROPERTY_PASSWORD_MODE);

    if (NULL == dt)
        goto undefined_grc_jkey_block;

    p->password_mode = grc_get_object_value(object, property_detail_string(dt),
                                            false);

    /* horizontal position */
    dt = get_property_detail(GRC_PROPERTY_H_POSITION);

    if (NULL == dt)
        goto undefined_grc_jkey_block;

    tmp = grc_get_object_str(object, property_detail_string(dt));
    p->horizontal_position = tr_horizontal_position(cl_string_valueof(tmp));
    free(tmp);

    /* fps */
    dt = get_property_detail(GRC_PROPERTY_FPS);

    if (NULL == dt)
        goto undefined_grc_jkey_block;

    p->fps = grc_get_object_value(object, property_detail_string(dt), 0);

    /* devices */
    dt = get_property_detail(GRC_PROPERTY_DEVICES);

    if (NULL == dt)
        goto undefined_grc_jkey_block;

    p->devices = grc_get_object_value(object, property_detail_string(dt), 1);

    return p;

undefined_grc_jkey_block:
    grc_set_errno(GRC_ERROR_UNDEFINED_GRC_KEY);
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

const char *grc_obj_get_property_name(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return NULL;

    return cl_string_valueof(prop->name);
}

const char *grc_obj_get_property_key(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return NULL;

    return cl_string_valueof(prop->key);
}

const char *grc_obj_get_property_parent(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return NULL;

    return cl_string_valueof(prop->parent);
}

const char *grc_obj_get_property_text(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return NULL;

    return cl_string_valueof(prop->text);
}

const char *grc_obj_get_property_fg(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return NULL;

    return cl_string_valueof(prop->fg);
}

enum grc_object grc_obj_get_property_type(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return -1;

    return prop->type;
}

int grc_obj_get_property_x(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return -1;

    return prop->x;
}

int grc_obj_get_property_y(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return -1;

    return prop->y;
}

int grc_obj_get_property_w(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return -1;

    return prop->w;
}

int grc_obj_get_property_h(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return -1;

    return prop->h;
}

bool grc_obj_get_property_hide(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return false;

    return prop->hide;
}

int grc_obj_get_property_line_break_mode(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return -1;

    return prop->line_break_mode;
}

int grc_obj_get_property_data_length(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return -1;

    return prop->data_length;
}

int grc_obj_get_property_radio_group(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return -1;

    return prop->radio_group;
}

int grc_obj_get_property_radio_type(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return -1;

    return prop->radio_type;
}

int grc_obj_get_property_password_mode(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return -1;

    return prop->password_mode;
}

int grc_obj_get_property_horizontal_position(struct grc_obj_properties *prop)
{
    if (NULL == prop)
        return -1;

    return prop->horizontal_position;
}

int grc_obj_set_property_type(struct grc_obj_properties *prop,
    enum grc_object type)
{
    if (NULL == prop)
        return -1;

    prop->type = type;

    return 0;
}

