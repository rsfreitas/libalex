
/*
 * Description: Functions to parse an GRC UI file.
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
#include "gui/objects.h"

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

/*
 * ------- GRC object handling -------
 */

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
 * THE parse ;-) (from a file). Just keep the DIALOG information as a JSON
 * internally.
 */
int parse_file(struct al_grc *grc, const char *grc_filename)
{
    grc->jgrc = cjson_read_file(grc_filename);

    if (NULL == grc->jgrc)
        return -1;

    return 0;
}

/*
 * THE parse ;-) (from a buffer). Just keep the DIALOG information as a JSON
 * internally.
 */
int parse_mem(struct al_grc *grc, const char *data)
{
    grc->jgrc = cjson_parse(data);

    if (NULL == grc->jgrc)
        return -1;

    return 0;
}

static int grc_to_DIALOG(struct grc_object *gobject, struct al_grc *grc)
{
    DIALOG *d, *p = NULL;
    struct grc_obj_properties *prop;
    struct grc_generic_data *gdata;
    int w = -1, h = -1;
    char tmp[MAX_EDIT_SIZE] = {0};
    enum al_grc_object type;

    prop = grc_object_get_properties(gobject);
    type = PROP_get(prop, type);
    d = grc_object_get_DIALOG(gobject);

    /*
     * Objects that the user MUST define _width_ and _height_:
     *
     *  listbox
     *  box
     *  slider
     */

    /*
     * Add an object into the DIALOG. It is recommended that 'boxes' be the
     * first objects defined, so that no text be superimposed.
     */
    switch (type) {
        case AL_GRC_OBJ_BOX:
            d->proc = d_box_proc;
            break;

        case AL_GRC_OBJ_DIGITAL_CLOCK:
            d->proc = gui_clock_proc;
            d->dp = gobject->dlg_clock_str;
            break;

        case AL_GRC_OBJ_IMAGE:
        case AL_GRC_OBJ_MESSAGES_LOG_BOX:
            if (type == AL_GRC_OBJ_MESSAGES_LOG_BOX) {
                d->proc = gui_messages_log_proc;
                d->d1 = PROP_get(prop, line_break_mode);
            } else {
                d->proc = gui_d_bitmap_proc;

                /*
                 * We set as NULL here so no error will be thrown when trying
                 * to draw the object and no image exists.
                 */
                d->dp = NULL;
            }

            /*
             * If we have a reference to a "father" object, the object position
             * will derived from his position. Worth mentioning that, for this
             * work correctly the "father" object must have been defined BEFORE
             * the object, otherwise we can't get his positions. ;-)
             */
            if (PROP_check(prop, parent) == true) {
                p = get_DIALOG_from_obj_ref(grc->ref,
                                            PROP_get(prop, parent));

                d->x = p->x + 2;
                d->y = p->y + 2;
                d->w = p->w - 2;
                d->h = p->h - 2;
            }

            break;

        case AL_GRC_OBJ_CUSTOM:
            /*
             * In this object, both the object and the callback functions must
             * be defined in runtime, before the DIALOG executes.
             *
             * d->proc -> Funcao do objeto
             * d->dp3  -> callback
             */
            break;

        case AL_GRC_OBJ_VAR_TEXT:
        case AL_GRC_OBJ_FIXED_TEXT:
            d->proc = d_text_proc;

            if (type == AL_GRC_OBJ_FIXED_TEXT)
                d->dp = (char *)PROP_get(prop, text);

            break;

        case AL_GRC_OBJ_BUTTON:
            d->proc = gui_d_button_proc;
            d->flags = D_EXIT;
            d->dp = (char *)PROP_get(prop, text);

            /* We compute width and height automatically, case is necessary */
            if (PROP_get(prop, w) < 0)
                w = text_length(font, d->dp) + BUTTON_DEFAULT_SPACER;

            if (PROP_get(prop, h) < 0)
                h = DEFAULT_BUTTON_HEIGHT;

            break;

        case AL_GRC_OBJ_EDIT:
            if (PROP_get(prop, data_length) >= MAX_EDIT_SIZE) {
                al_set_errno(AL_ERROR_UNSUPPORTED_EDIT_INPUT_LENGTH);
                return -1;
            }

            if (PROP_get(prop, password_mode) == false)
                d->proc = gui_d_edit_proc;
            else {
                d->proc = gui_d_password_proc;

                /*
                 * Creates a temporary buffer to store the asterisks strings.
                 */
                gdata = new_grc_generic_data();
                gobject->g_data = cdll_unshift(gobject->g_data, gdata);
                d->dp2 = gdata->data;
            }

            /* Creates the buffer to store the typed string in the object. */
            gdata = new_grc_generic_data();
            gobject->g_data = cdll_unshift(gobject->g_data, gdata);
            d->dp = gdata->data;

            d->flags = D_EXIT;
            d->d1 = PROP_get(prop, data_length);

            /*
             * Adjusts a default size allowing that the mouse may set the
             * focus on this object.
             */
            h = DEFAULT_EDIT_HEIGHT;

            /* We compute width automatically, case is necessary */
            if (PROP_check(prop, parent) == true) {
                p = get_DIALOG_from_obj_ref(grc->ref,
                                            PROP_get(prop, parent));

                d->x = p->x + 3;
                d->y = p->y + 3;
                d->w = p->w - 4;
                d->h = h;
            } else {
                if (PROP_get(prop, w) < 0) {
                    memset(tmp, '0', PROP_get(prop, data_length) + 1);
                    w = text_length(font, tmp);
                }
            }

            break;

        case AL_GRC_OBJ_LIST:
            d->proc = gui_d_list_proc;
            d->flags = D_EXIT;
            break;

        case AL_GRC_OBJ_CHECK:
            d->proc = gui_d_check_proc;
            d->dp = (char *)PROP_get(prop, text);

            if (PROP_get(prop, horizontal_position) == AL_GRC_H_POS_RIGHT)
                d->d1 = 1;
            else
                d->d1 = 0;

            /* We compute width and height automatically, case is necessary */
            if (PROP_get(prop, w) < 0)
                w = text_length(font, d->dp) + CHECKBOX_DEFAULT_SPACER;

            if (PROP_get(prop, h) < 0)
                h = DEFAULT_CHECKBOX_HEIGHT;

            break;

        case AL_GRC_OBJ_RADIO:
            d->proc = gui_d_radio_proc;
            d->dp = (char *)PROP_get(prop, text);
            d->d1 = PROP_get(prop, radio_group);
            d->d2 = PROP_get(prop, radio_type);

            /* We compute width and height automatically, case is necessary */
            if (PROP_get(prop, w) < 0)
                w = text_length(font, d->dp) + RADIO_DEFAULT_SPACER;

            if (PROP_get(prop, h) < 0)
                h = DEFAULT_RADIO_HEIGHT;

            break;

        case AL_GRC_OBJ_SLIDER:
            d->proc = gui_d_slider_proc;
            d->d1 = PROP_get(prop, data_length);
            break;

        case AL_GRC_OBJ_VT_KEYBOARD:
            d->proc = gui_d_vt_keyboard_proc;
            d->d1 = GRC_KLAYOUT_LETTERS;
            d->dp = grc;

            /*
             * Set that the virtual keyboard is enabled to this DIALOG
             * so that every object may known this.
             */
            grc->virtual_keyboard = true;
            break;

        case AL_GRC_OBJ_ICON:
            d->proc = gui_d_icon_proc;
            d->flags = D_EXIT;
            break;

        case AL_GRC_OBJ_TEXTBOX:
            d->proc = d_textbox_proc;
            break;

        default:
            al_set_errno(AL_ERROR_UNKNOWN_OBJECT_TYPE);
            return -1;
    }

    /* Object position into the screen */
    if (PROP_check(prop, parent) == false) {
        d->x = PROP_get(prop, x);
        d->y = PROP_get(prop, y);
        d->w = (w == -1) ? PROP_get(prop, w) : w;
        d->h = (h == -1) ? PROP_get(prop, h) : h;
    }

    /* Specific object colors */
    if (PROP_check(prop, fg) == true)
        d->fg = color_grc_to_al(info_color_depth(grc), PROP_get(prop, fg));
    else
        d->fg = color_get_global_fg(grc);

    d->bg = color_get_global_bg(grc);

    /* Hides the object or not */
    if (PROP_get(prop, hide) == false)
        d->flags &= ~D_HIDDEN;
    else
        d->flags |= D_HIDDEN;

    return 0;
}

static void set_callback_data(struct grc_object *gobject, struct al_grc *grc)
{
    struct al_callback_data *acd;
    DIALOG *d;

    acd = new_callback_data();

    if (NULL == acd) {
        al_set_errno(AL_ERROR_MEMORY);
        return;
    }

    d = grc_object_get_DIALOG(gobject);
    acd->grc = (void *)grc;
    d->dp3 = acd;

    gobject->cb_data = acd;
}

static load_object_to_grc(cjson_t *object, struct al_grc *grc)
{
    struct grc_object *gobj = NULL;

    gobj = new_grc_object();

    if (NULL == gobj)
        return -1;

    /* Load object properties from the GRC */
    gobj->prop = new_obj_properties(object);

    if (NULL == gobj->prop)
        goto error_block;

    /* Translate this to Allegro's DIALOG format */
    if (grc_to_DIALOG(gobj, grc) < 0)
        goto error_block;

    /*
     * Every object already has access to internal library information,
     * even if they do not have callback functions.
     */
    set_callback_data(gobj, grc);

    /* Creates a reference for this object, if it is a parent object */
    grc_creates_reference(grc, gobj);

    /* Store the loaded object */
    grc->ui_objects = cdll_unshift(grc->ui_objects, gobj);

    return 0;

error_block:
    if (gobj != NULL)
        destroy_grc_object(gobj);

    return -1;
}

/*
 * Load all objects related info from the GRC to an usable format, such as
 * the objects, keys and menu informations.
 */
int parse_objects(struct al_grc *grc)
{
    cjson_t *objects, *p;
    int t_objects, i;

    objects = grc_get_object(grc, OBJ_OBJECTS);

    if (NULL == objects) {
        al_set_errno(AL_ERROR_OBJECTS_BLOCK_NOT_FOUND);
        return -1;
    }

    t_objects = cjson_get_array_size(objects);

    if (t_objects <= 0) {
        al_set_errno(AL_ERROR_NO_OBJECTS);
        return -1;
    }

    for (i = 0; i < t_objects; i++) {
        p = cjson_get_array_item(objects, i);

        if (NULL == p)
            /* TODO: set error code */
            return -1;

        if (load_object_to_grc(p, grc) < 0)
            /* TODO: set error code */
            return -1;
    }

    /* Parse keys */
    // TODO

    /* Parse menu */
    // TODO

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

    return p;
}

