
/*
 * Description: Functions to parse an UI file.
 *
 * Author: Rodrigo Freitas
 * Created at: Tue Dec  9 22:42:05 2014
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
#include <string.h>

#include "libalex.h"

/* Used to map possible objects from a DIALOG */
struct grc_dlg_object {
    char                name[256];
    enum al_grc_object  type;
};

static struct grc_json_key __entries[] = {
    { OBJ_WIDTH,                AL_GRC_JOBJ_WIDTH,              GRC_NUMBER  },
    { OBJ_HEIGHT,               AL_GRC_JOBJ_HEIGHT,             GRC_NUMBER  },
    { OBJ_COLOR_DEPTH,          AL_GRC_JOBJ_COLOR_DEPTH,        GRC_NUMBER  },
    { OBJ_FOREGROUND,           AL_GRC_JOBJ_FOREGROUND,         GRC_STRING  },
    { OBJ_BACKGROUND,           AL_GRC_JOBJ_BACKGROUND,         GRC_STRING  },
    { OBJ_BLOCK_EXIT_KEYS,      AL_GRC_JOBJ_BLOCK_EXIT_KEYS,    GRC_BOOL    },
    { OBJ_MOUSE,                AL_GRC_JOBJ_MOUSE,              GRC_BOOL    },
    { OBJ_TYPE,                 AL_GRC_JOBJ_TYPE,               GRC_STRING  },
    { OBJ_POS_X,                AL_GRC_JOBJ_POS_X,              GRC_NUMBER  },
    { OBJ_POS_Y,                AL_GRC_JOBJ_POS_Y,              GRC_NUMBER  },
    { OBJ_NAME,                 AL_GRC_JOBJ_NAME,               GRC_STRING  },
    { OBJ_PARENT,               AL_GRC_JOBJ_PARENT,             GRC_STRING  },
    { OBJ_KEY,                  AL_GRC_JOBJ_KEY,                GRC_STRING  },
    { OBJ_TEXT,                 AL_GRC_JOBJ_TEXT,               GRC_STRING  },
    { OBJ_HIDE,                 AL_GRC_JOBJ_HIDE,               GRC_BOOL    },
    { OBJ_LINE_BREAK,           AL_GRC_JOBJ_LINE_BREAK,         GRC_STRING  },
    { OBJ_IGNORE_ESC_KEY,       AL_GRC_JOBJ_IGNORE_ESC_KEY,     GRC_BOOL    },
    { OBJ_INPUT_LENGTH,         AL_GRC_JOBJ_INPUT_LENGTH,       GRC_NUMBER  },
    { OBJ_RADIO_GROUP,          AL_GRC_JOBJ_RADIO_GROUP,        GRC_NUMBER  },
    { OBJ_RADIO_TYPE,           AL_GRC_JOBJ_RADIO_TYPE,         GRC_STRING  },
    { OBJ_PASSWORD,             AL_GRC_JOBJ_PASSWORD_MODE,      GRC_BOOL    },
    { OBJ_HORIZONTAL_POSITION,  AL_GRC_JOBJ_H_POSITION,         GRC_STRING  },
    { OBJ_FPS,                  AL_GRC_JOBJ_FPS,                GRC_NUMBER  },
    { OBJ_DEVICES,              AL_GRC_JOBJ_DEVICES,            GRC_NUMBER  }
};

#define MAX_GRC_ENTRIES             \
    (sizeof(__entries) / sizeof(__entries[0]))

/* Supported objects from a DIALOG */
static struct grc_dlg_object __dlg_objects [] = {
    { DLG_OBJ_KEY,              AL_GRC_OBJ_KEY                  },
    { DLG_OBJ_BOX,              AL_GRC_OBJ_BOX                  },
    { DLG_OBJ_DIGITAL_CLOCK,    AL_GRC_OBJ_DIGITAL_CLOCK        },
    { DLG_OBJ_IMAGE,            AL_GRC_OBJ_IMAGE                },
    { DLG_OBJ_MESSAGES_LOG_BOX, AL_GRC_OBJ_MESSAGES_LOG_BOX     },
    { DLG_OBJ_VAR_TEXT,         AL_GRC_OBJ_VAR_TEXT             },
    { DLG_OBJ_FIXED_TEXT,       AL_GRC_OBJ_FIXED_TEXT           },
    { DLG_OBJ_CUSTOM,           AL_GRC_OBJ_CUSTOM               },
    { DLG_OBJ_BUTTON,           AL_GRC_OBJ_BUTTON               },
    { DLG_OBJ_EDIT,             AL_GRC_OBJ_EDIT                 },
    { DLG_OBJ_LIST,             AL_GRC_OBJ_LIST                 },
    { DLG_OBJ_CHECKBOX,         AL_GRC_OBJ_CHECK                },
    { DLG_OBJ_RADIO,            AL_GRC_OBJ_RADIO                },
    { DLG_OBJ_SLIDER,           AL_GRC_OBJ_SLIDER               },
    { DLG_OBJ_LIVE_IMAGE,       AL_GRC_OBJ_LIVE_IMAGE           },
    { DLG_OBJ_MULTLIVE_IMAGE,   AL_GRC_OBJ_MULTLIVE_IMAGE       },
    { DLG_OBJ_VT_KEYBOARD,      AL_GRC_OBJ_VT_KEYBOARD          },
    { DLG_OBJ_ICON,             AL_GRC_OBJ_ICON                 },
    { DLG_OBJ_TEXTBOX,          AL_GRC_OBJ_TEXTBOX              }
};

#define MAX_DLG_SUPPORTED_OBJECTS   \
    (sizeof(__dlg_objects) / sizeof(__dlg_objects[0]))

/* 'messages_log_box' line break types */
#define LBREAK_RAW_STR      "raw"
#define LBREAK_SMART_STR    "smart"

/* Radio button types */
#define RADIO_CIRCLE        "circle"
#define RADIO_SQUARE        "square"

/* Horizontal positions */
#define POS_H_LEFT          "left"
#define POS_H_RIGHT         "right"

/*
 * ------- Common functions -------
 */

/*
 * Translate a string into a line break mode.
 */
static int tr_line_break(const char *mode)
{
    /* default option in case there is no key */
    if ((NULL == mode) || !strcmp(mode, LBREAK_SMART_STR))
        return AL_GRC_LBREAK_SMART;

    if (!strcmp(mode, LBREAK_RAW_STR))
        return AL_GRC_LBREAK_RAW;

    return -1;
}

const char *str_line_break(enum al_grc_line_break lbreak)
{
    switch (lbreak) {
        case AL_GRC_LBREAK_SMART:
            return LBREAK_SMART_STR;

        case AL_GRC_LBREAK_RAW:
            return LBREAK_RAW_STR;
    }

    return NULL;
}

/*
 * Translate a string to the radio button type.
 */
static int tr_radio_type(const char *type)
{
    /* default option in case there is no key */
    if ((NULL == type) || !strcmp(type, RADIO_CIRCLE))
        return AL_GRC_RADIO_CIRCLE;

    if (!strcmp(type, RADIO_SQUARE))
        return AL_GRC_RADIO_SQUARE;

    return -1;
}

const char *str_radio_type(enum al_grc_radio_button_fmt radio)
{
    switch (radio) {
        case AL_GRC_RADIO_CIRCLE:
            return RADIO_CIRCLE;

        case AL_GRC_RADIO_SQUARE:
            return RADIO_SQUARE;
    }

    return NULL;
}

/*
 * Translate a string pointing to a horizontal position from an object.
 */
static int tr_horizontal_position(const char *pos)
{
    /* default option in case there is no key */
    if ((NULL == pos) || !strcmp(pos, POS_H_RIGHT))
        return AL_GRC_H_POS_RIGHT;

    if (!strcmp(pos, POS_H_LEFT))
        return AL_GRC_H_POS_LEFT;

    return -1;
}

const char *str_horizontal_position(enum al_grc_hpos hpos)
{
    switch (hpos) {
        case AL_GRC_H_POS_RIGHT:
            return POS_H_RIGHT;

        case AL_GRC_H_POS_LEFT:
            return POS_H_LEFT;
    }

    return NULL;
}

/*
 * ------- GRC object handling -------
 */

/*
 * Search for an object able to be used in a DIALOG through its name. This
 * name is the same loaded from the GRC file.
 *
 * Returns an int to allow negative values without compile warnings.
 *
 * @type_name: Object name which will be searched.
 */
static int tr_str_type_to_grc_type(const char *type_name)
{
    unsigned int i;
    int ret = -1;

    if (NULL == type_name)
        return -1;

    for (i = 0; i < MAX_DLG_SUPPORTED_OBJECTS; i++)
        if (!strcmp(__dlg_objects[i].name, type_name)) {
            ret = __dlg_objects[i].type;
            break;
        }

    return ret;
}

const char *str_grc_obj_type(enum al_grc_object obj)
{
    unsigned int i;
    char *n = NULL;

    for (i = 0; i < MAX_DLG_SUPPORTED_OBJECTS; i++)
        if (__dlg_objects[i].type == obj) {
            n = __dlg_objects[i].name;
            break;
        }

    return n;
}

/*
 * Search for an information structure from a JSON object in the supported
 * objects list.
 */
struct grc_json_key *get_grc_json_key(enum al_grc_object_property prop)
{
    unsigned int i;
    struct grc_json_key *e = NULL;

    for (i = 0; i < MAX_GRC_ENTRIES; i++)
        if (__entries[i].prop == prop) {
            e = &__entries[i];
            break;
        }

    return e;
}

/*
 * THE parse ;-) (from a file)
 */
int grc_parse_file(struct al_grc *grc, const char *grc_filename)
{
    grc->jgrc = cjson_read_file(grc_filename);

    if (NULL == grc->jgrc)
        return -1;

    return 0;
}

/*
 * THE parse ;-) (from a buffer)
 */
int grc_parse_mem(struct al_grc *grc, const char *data)
{
    grc->jgrc = cjson_parse(data);

    if (NULL == grc->jgrc)
        return -1;

    return 0;
}

/*
 * Get the object value from within a GRC file, expecting to be a GRC_NUMBER
 * or a GRC_BOOL object.
 */
int grc_get_object_value(cjson_t *object, const char *object_name,
    int default_value)
{
    cjson_t *n = NULL;
    enum cjson_type type;
    cstring_t *s = NULL;
    int v = 0;

    n = cjson_get_object_item(object, object_name);

    if (NULL == n)
        return default_value;

    type = cjson_get_object_type(n);

    if (type == CJSON_NUMBER) {
        s = cjson_get_object_value(n);
        v = cstring_to_int(s);
        cstring_unref(s);

        return v;
    }

    return (type == CJSON_TRUE ? true : false);
}

/*
 * Get the object value from within a GRC file, expecting to be a GRC_STRING
 * object.
 */
char *grc_get_object_str(cjson_t *object, const char *object_name)
{
    cjson_t *n = NULL;
    cstring_t *s = NULL;
    char *p = NULL;

    n = cjson_get_object_item(object, object_name);

    if (NULL == n)
        return NULL;

    s = cjson_get_object_value(n);
    p = strdup(cstring_valueof(s));
    cstring_unref(s);

    return p;
}

/*
 * Destroy a structure 'struct grc_key_data'.
 */
void destroy_key_data(struct grc_key_data *kdata)
{
    if (kdata->name != NULL)
        free(kdata->name);

    if (kdata->key != NULL)
        free(kdata->key);

    free(kdata);
}

/*
 * Create and return a structure 'grc_key_data'. It is created and filled with
 * content from an object from an array 'keys' inside a GRC file.
 */
struct grc_key_data *new_key_data(cjson_t *key)
{
    struct grc_key_data *k = NULL;
    struct grc_json_key *e;
    char *tmp;

    k = calloc(1, sizeof(struct grc_key_data));

    if (NULL == k) {
        al_set_errno(AL_ERROR_MEMORY);
        return NULL;
    }

    /* key */
    e = get_grc_json_key(AL_GRC_JOBJ_KEY);

    if (NULL == e)
        goto undefined_grc_jkey_block;

    tmp = grc_get_object_str(key, e->name);

    if (NULL == tmp)
        goto undefined_grc_jkey_block;

    k->key = tmp;

    /* name */
    e = get_grc_json_key(AL_GRC_JOBJ_NAME);

    if (NULL == e)
        goto undefined_grc_jkey_block;

    tmp = grc_get_object_str(key, e->name);
    k->name = tmp;

    return k;

undefined_grc_jkey_block:
    al_set_errno(AL_ERROR_UNDEFINED_GRC_KEY);
    destroy_key_data(k);

    return NULL;
}

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

void destroy_menu(void *a)
{
    struct grc_menu *menu = (struct grc_menu *)a;

    if (menu->items != NULL)
        cdll_free(menu->items, destroy_menu);

    if (menu->parent != NULL)
        free(menu->parent);

    if (menu->name != NULL)
        free(menu->name);

    if (menu->text != NULL)
        free(menu->text);

    free(menu);
}

struct grc_menu *new_menu(cjson_t *object)
{
    struct grc_json_key *e;
    struct grc_menu *m, *it;
    char *tmp;
    cjson_t *o, *n;
    int i, t;

    m = calloc(1, sizeof(struct grc_menu));

    if (NULL == m)
        return NULL;

    m->parent = NULL;
    m->items = NULL;

    /* name */
    e = get_grc_json_key(AL_GRC_JOBJ_NAME);

    if (e != NULL) {
        tmp = grc_get_object_str(object, e->name);

        if (tmp != NULL)
            m->name = tmp;
    }

    /* parent */
    e = get_grc_json_key(AL_GRC_JOBJ_PARENT);

    if (e != NULL) {
        tmp = grc_get_object_str(object, e->name);

        if (tmp != NULL)
            m->parent = tmp;
    }

    /* text */
    e = get_grc_json_key(AL_GRC_JOBJ_TEXT);

    if (e != NULL) {
        tmp = grc_get_object_str(object, e->name);

        if (tmp != NULL)
            m->text = tmp;
    }

    /* options (items) */
    o = cjson_get_object_item(object, OBJ_OPTIONS);

    if (o != NULL) {
        t = cjson_get_array_size(o);

        for (i = 0; i < t; i++) {
            n = cjson_get_array_item(o, i);

            it = new_menu(n);
            m->items = cdll_unshift(m->items, it);
        }
    }

    return m;
}

