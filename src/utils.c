
/*
 * Description: Utility functions inside the library.
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Jul 28 09:01:31 2016
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

#include <ctype.h>

#include "libgrc.h"

/* Used to map possible objects from a DIALOG */
struct grc_dlg_object {
    char                name[256];
    enum grc_object     type;
};

/* Supported objects from a DIALOG */
static struct grc_dlg_object __dlg_objects [] = {
    { DLG_OBJ_KEY,              GRC_OBJECT_KEY                  },
    { DLG_OBJ_BOX,              GRC_OBJECT_BOX                  },
    { DLG_OBJ_DIGITAL_CLOCK,    GRC_OBJECT_DIGITAL_CLOCK        },
    { DLG_OBJ_IMAGE,            GRC_OBJECT_IMAGE                },
    { DLG_OBJ_MESSAGES_LOG_BOX, GRC_OBJECT_MESSAGES_LOG_BOX     },
    { DLG_OBJ_VAR_TEXT,         GRC_OBJECT_VAR_TEXT             },
    { DLG_OBJ_FIXED_TEXT,       GRC_OBJECT_FIXED_TEXT           },
    { DLG_OBJ_CUSTOM,           GRC_OBJECT_CUSTOM               },
    { DLG_OBJ_BUTTON,           GRC_OBJECT_BUTTON               },
    { DLG_OBJ_EDIT,             GRC_OBJECT_EDIT                 },
    { DLG_OBJ_LIST,             GRC_OBJECT_LIST                 },
    { DLG_OBJ_CHECKBOX,         GRC_OBJECT_CHECK                },
    { DLG_OBJ_RADIO,            GRC_OBJECT_RADIO                },
    { DLG_OBJ_SLIDER,           GRC_OBJECT_SLIDER               },
    { DLG_OBJ_LIVE_IMAGE,       GRC_OBJECT_LIVE_IMAGE           },
    { DLG_OBJ_MULTLIVE_IMAGE,   GRC_OBJECT_MULTLIVE_IMAGE       },
    { DLG_OBJ_VT_KEYBOARD,      GRC_OBJECT_VT_KEYBOARD          },
    { DLG_OBJ_ICON,             GRC_OBJECT_ICON                 },
    { DLG_OBJ_TEXTBOX,          GRC_OBJECT_TEXTBOX              }
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
 * This is an original Allegro function. We put it here because there it
 * is declared as static and we need it.
 */
void dotted_rect(int x1, int y1, int x2, int y2, int fg, int bg)
{
    BITMAP *gui_bmp = gui_get_screen();
    int x = ((x1 + y1) & 1) ? 1 : 0;
    int c;

    /* two loops to avoid bank switches */
    for (c = x1; c <= x2; c++)
        putpixel(gui_bmp, c, y1, (((c + y1) & 1) == x) ? fg : bg);

    for (c = x1; c <= x2; c++)
        putpixel(gui_bmp, c, y2, (((c + y2) & 1) == x) ? fg : bg);

    for (c = y1 + 1; c < y2; c++) {
        putpixel(gui_bmp, x1, c, (((c + x1) & 1) == x) ? fg : bg);
        putpixel(gui_bmp, x2, c, (((c + x2) & 1) == x) ? fg : bg);
    }
}

/*
 * Search for an object able to be used in a DIALOG through its name. This
 * name is the same loaded from the GRC file.
 *
 * Returns an int to allow negative values without compile warnings.
 *
 * @type_name: Object name which will be searched.
 */
int tr_str_type_to_grc_type(const char *type_name)
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

/*
 * Translate a string into a line break mode.
 */
int tr_line_break(const char *mode)
{
    /* default option in case there is no key */
    if ((NULL == mode) || !strcmp(mode, LBREAK_SMART_STR))
        return GRC_LINE_BREAK_SMART;

    if (!strcmp(mode, LBREAK_RAW_STR))
        return GRC_LINE_BREAK_RAW;

    return -1;
}

/*
 * Translate a string to the radio button type.
 */
int tr_radio_type(const char *type)
{
    /* default option in case there is no key */
    if ((NULL == type) || !strcmp(type, RADIO_CIRCLE))
        return GRC_RADIO_CIRCLE;

    if (!strcmp(type, RADIO_SQUARE))
        return GRC_RADIO_SQUARE;

    return -1;
}

/*
 * Translate a string pointing to a horizontal position from an object.
 */
int tr_horizontal_position(const char *pos)
{
    /* default option in case there is no key */
    if ((NULL == pos) || !strcmp(pos, POS_H_RIGHT))
        return GRC_H_POS_RIGHT;

    if (!strcmp(pos, POS_H_LEFT))
        return GRC_H_POS_LEFT;

    return -1;
}

const char *str_line_break(enum grc_line_break lbreak)
{
    switch (lbreak) {
        case GRC_LINE_BREAK_SMART:
            return LBREAK_SMART_STR;

        case GRC_LINE_BREAK_RAW:
            return LBREAK_RAW_STR;
    }

    return NULL;
}

const char *str_radio_type(enum grc_radio_button_fmt radio)
{
    switch (radio) {
        case GRC_RADIO_CIRCLE:
            return RADIO_CIRCLE;

        case GRC_RADIO_SQUARE:
            return RADIO_SQUARE;
    }

    return NULL;
}

const char *str_horizontal_position(enum grc_horizontal_position hpos)
{
    switch (hpos) {
        case GRC_H_POS_RIGHT:
            return POS_H_RIGHT;

        case GRC_H_POS_LEFT:
            return POS_H_LEFT;
    }

    return NULL;
}

const char *str_grc_obj_type(enum grc_object obj)
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

cl_json_t *grc_get_object(struct grc_s *grc, const char *object)
{
    return cl_json_get_object_item(grc->jgrc, object);
}

static int special_keys(const char *key)
{
    int ret = KEY_F1 - 1, k = 0;
    char *tmp;

    if (!strcmp(key, "ESC"))
        return KEY_ESC;

    if (key[0] == 'F') {
        tmp = (char *)&key[1];
        k = strtol(tmp, NULL, 10);
        return ret + k;
    }

    return -1;
}

static int key_char(const char *key)
{
    int c = key[0];

    if (isalpha(c))
        return c - '@';

    if (isdigit(c))
        return c - 21;

    return -1;
}

/*
 * Translate a key string, since it is in the format 'KEY_X', to its
 * corresponding scancode.
 */
int tr_str_key_to_al_key(const char *skey)
{
    cl_string_t *tmp = NULL, *p = NULL;
    cl_string_list_t *list = NULL;
    int ret = -1;

    tmp = cl_string_create("%s", skey);

    if (NULL == tmp)
        return -1;

    list = cl_string_split(tmp, "_");

    if (NULL == list)
        goto end_block;

    if (cl_string_list_size(list) != 2)
        goto end_block;

    /* ok */
    p = cl_string_list_get(list, 1);

    if (cl_string_length(p) == 1)
        ret = key_char(cl_string_valueof(p));
    else
        ret = special_keys(cl_string_valueof(p));

    cl_string_unref(p);

end_block:
    if (tmp != NULL)
        cl_string_unref(tmp);

    if (list != NULL)
        cl_string_list_destroy(list);

    return ret;
}

