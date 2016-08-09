
/*
 * Description: Functions to create/save an UI file.
 *
 * Author: Rodrigo Freitas
 * Created at: Mon Dec 15 22:59:58 2014
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

/* Number os colors used when outside graphic mode */
#define OUT_GFX_COLOR_DEPTH             8

struct writer_builder {
    cjson_t     *array;
    cjson_t     *object;
};

static void destroy_writer_builder(void *ptr)
{
    struct writer_builder *w;

    if (NULL == ptr)
        return;

    w = (struct writer_builder *)ptr;

    free(w);
}

static struct writer_builder *new_writer_builder(void)
{
    struct writer_builder *w = NULL;

    w = calloc(1, sizeof(struct writer_builder));

    if (NULL == w)
        return NULL;

    return w;
}

static int create_root_object(struct grc_s *grc)
{
    cjson_t *p = NULL;

    p = grc_get_JSON(grc);

    if (NULL == p) {
        p = cjson_create_object();

        if (NULL == p) {
            grc_set_errno(GRC_ERROR_MEMORY);
            return -1;
        }

        grc_set_JSON(grc, p);
    }

    return 0;
}

int LIBEXPORT grc_GRC_create_colors(grc_t *grc,
    const char *foreground, const char *background)
{
    cjson_t *fg, *bg, *c, *root;

    grc_errno_clear();

    if ((NULL == grc) || (NULL == foreground) || (NULL == background)) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return -1;
    }

    if (create_root_object(grc) < 0)
        return -1;

    /* Invalid colors? */
    if ((color_grc_to_al(OUT_GFX_COLOR_DEPTH, foreground) < 0) ||
        (color_grc_to_al(OUT_GFX_COLOR_DEPTH, background) < 0))
    {
        grc_set_errno(GRC_ERROR_UNSUPPORTED_COLORS);
        return -1;
    }

    fg = cjson_create_string(foreground);
    bg = cjson_create_string(background);
    c = cjson_create_object();

    if ((NULL == fg) || (NULL == bg) || (NULL == c)) {
        grc_set_errno(GRC_ERROR_MEMORY);
        return -1;
    }

    /* Create an object with the colors */
    cjson_add_item_to_object(c, OBJ_FOREGROUND, fg);
    cjson_add_item_to_object(c, OBJ_BACKGROUND, bg);

    /* Add it to the main object */
    root = grc_get_JSON(grc);
    cjson_add_item_to_object(root, OBJ_COLORS, c);

    return 0;
}

int LIBEXPORT grc_GRC_create_info(grc_t *grc, unsigned int width,
    unsigned int height, enum grc_color_depth color_depth,
    bool block_exit_keys, bool mouse, bool ignore_esc_key)
{
    cjson_t *c, *k, *m, *e, *w, *h, *i, *root;

    grc_errno_clear();

    if (NULL == grc) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return -1;
    }

    if (create_root_object(grc) < 0)
        return -1;

    w = cjson_create_number(width);
    h = cjson_create_number(height);
    c = cjson_create_number(color_depth);

    if (block_exit_keys == false)
        k = cjson_create_false();
    else
        k = cjson_create_true();

    if (mouse == false)
        m = cjson_create_false();
    else
        m = cjson_create_true();

    if (ignore_esc_key == false)
        e = cjson_create_false();
    else
        e = cjson_create_true();

    i = cjson_create_object();

    if ((NULL == w) || (NULL == h) || (NULL == k) || (NULL == m) ||
        (NULL == e) || (NULL == i))
    {
        grc_set_errno(GRC_ERROR_MEMORY);
        return -1;
    }

    /* Create an object with the info */
    cjson_add_item_to_object(i, OBJ_WIDTH, w);
    cjson_add_item_to_object(i, OBJ_HEIGHT, h);
    cjson_add_item_to_object(i, OBJ_COLOR_DEPTH, c);
    cjson_add_item_to_object(i, OBJ_BLOCK_EXIT_KEYS, k);
    cjson_add_item_to_object(i, OBJ_MOUSE, m);
    cjson_add_item_to_object(i, OBJ_IGNORE_ESC_KEY, e);

    /* Add it to the main object */
    root = grc_get_JSON(grc);
    cjson_add_item_to_object(root, OBJ_INFO, i);

    return 0;
}

static int create_tmp_array(grc_t *grc)
{
    struct writer_builder *wb = NULL;

    grc_errno_clear();

    if (NULL == grc) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return -1;
    }

    create_root_object(grc);
    wb = new_writer_builder();
    wb->array = cjson_create_array();

    if (NULL == wb->array) {
        grc_set_errno(GRC_ERROR_MEMORY);
        return -1;
    }

    if (grc_get_internal_data(grc) != NULL)
        grc_release_internal_data(grc);

    grc_set_internal_data(grc, wb, destroy_writer_builder);

    return 0;
}

int LIBEXPORT grc_GRC_keys_start(grc_t *grc)
{
    return create_tmp_array(grc);
}

int LIBEXPORT grc_GRC_add_key(grc_t *grc, const char *key,
    const char *name)
{
    cjson_t *k, *n = NULL, *p;
    struct writer_builder *wb = NULL;

    grc_errno_clear();

    if ((NULL == grc) || (NULL == key)) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return -1;
    }

    wb = (struct writer_builder *)grc_get_internal_data(grc);
    k = cjson_create_string(key);

    if (name != NULL)
        n = cjson_create_string(name);

    p = cjson_create_object();

    if ((NULL == k) || ((name != NULL) && (NULL == n)) || (NULL == p)) {
        grc_set_errno(GRC_ERROR_MEMORY);
        return -1;
    }

    /* Create the object */
    cjson_add_item_to_object(p, OBJ_KEY, k);

    if (n != NULL)
        cjson_add_item_to_object(p, OBJ_TAG, n);

    /* Add to the array */
    cjson_add_item_to_array(wb->array, p);

    return 0;
}

static int add_tmp_array(grc_t *grc, const char *array_name)
{
    cjson_t *root = NULL;
    struct writer_builder *wb = NULL;

    grc_errno_clear();

    if ((NULL == grc) || (NULL == array_name)) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return -1;
    }

    root = grc_get_JSON(grc);
    wb = (struct writer_builder *)grc_get_internal_data(grc);
    cjson_add_item_to_object(root, array_name, wb->array);

    return 0;
}

int LIBEXPORT grc_GRC_keys_finish(grc_t *grc)
{
    return add_tmp_array(grc, OBJ_KEYS);
}

int LIBEXPORT grc_GRC_objects_start(grc_t *grc)
{
    return create_tmp_array(grc);
}

int LIBEXPORT grc_GRC_create_object(grc_t *grc)
{
    struct writer_builder *wb = NULL;

    grc_errno_clear();

    if (NULL == grc) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return -1;
    }

    wb = (struct writer_builder *)grc_get_internal_data(grc);

    if (NULL == wb) {
        /* FIXME: set erro code */
        return -1;
    }

    wb->object = cjson_create_object();

    if (NULL == wb->object) {
        grc_set_errno(GRC_ERROR_MEMORY);
        return -1;
    }

    return 0;
}

int LIBEXPORT grc_GRC_finish_object(grc_t *grc)
{
    struct writer_builder *wb = NULL;

    grc_errno_clear();

    if (NULL == grc) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return -1;
    }

    wb = (struct writer_builder *)grc_get_internal_data(grc);
    cjson_add_item_to_array(wb->array, wb->object);

    return 0;
}

int LIBEXPORT grc_GRC_set_object_property(grc_t *grc,
    enum grc_object_property prop, ...)
{
    struct writer_builder *wb = NULL;
    va_list ap;
    enum grc_entry_type_value grc_value;
    const char *jkey = NULL;
    cjson_t *c;
    int i;
    char *s = NULL;

    grc_errno_clear();

    if (NULL == grc) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return -1;
    }

    va_start(ap, NULL);

    switch (prop) {
        case GRC_PROPERTY_WIDTH:
            jkey = OBJ_WIDTH;
            i = va_arg(ap, int);
            grc_value = GRC_NUMBER;
            break;

        case GRC_PROPERTY_HEIGHT:
            jkey = OBJ_HEIGHT;
            i = va_arg(ap, int);
            grc_value = GRC_NUMBER;
            break;

        case GRC_PROPERTY_FOREGROUND:
            jkey = OBJ_FOREGROUND;
            s = va_arg(ap, char *);

            if (color_grc_to_al(DEFAULT_COLOR_DEPTH, s) < 0) {
                grc_set_errno(GRC_ERROR_UNSUPPORTED_COLOR_DEPTH);
                return -1;
            }

            grc_value = GRC_STRING;
            break;

        case GRC_PROPERTY_TYPE:
            jkey = OBJ_TYPE;
            i = va_arg(ap, int);
            grc_value = GRC_STRING;
            s = (char *)str_grc_obj_type(i);

            if (NULL == s) {
                grc_set_errno(GRC_ERROR_UNKNOWN_OBJECT_TYPE);
                return -1;
            }

            break;

        case GRC_PROPERTY_POS_X:
            jkey = OBJ_POS_X;
            i = va_arg(ap, int);
            grc_value = GRC_NUMBER;
            break;

        case GRC_PROPERTY_POS_Y:
            jkey = OBJ_POS_Y;
            i = va_arg(ap, int);
            grc_value = GRC_NUMBER;
            break;

        case GRC_PROPERTY_TAG:
            jkey = OBJ_TAG;
            s = va_arg(ap, char *);
            grc_value = GRC_STRING;
            break;

        case GRC_PROPERTY_PARENT:
            jkey = OBJ_PARENT;
            s = va_arg(ap, char *);
            grc_value = GRC_STRING;
            break;

        case GRC_PROPERTY_TEXT:
            jkey = OBJ_TEXT;
            s = va_arg(ap, char *);
            grc_value = GRC_STRING;
            break;

        case GRC_PROPERTY_HIDE:
            jkey = OBJ_HIDE;
            i = va_arg(ap, int);
            grc_value = GRC_BOOL;
            break;

        case GRC_PROPERTY_LINE_BREAK:
            jkey = OBJ_LINE_BREAK;
            i = va_arg(ap, int);
            grc_value = GRC_STRING;
            s = (char *)str_line_break(i);
            break;

        case GRC_PROPERTY_INPUT_LENGTH:
            jkey = OBJ_INPUT_LENGTH;
            i = va_arg(ap, int);
            grc_value = GRC_NUMBER;
            break;

        case GRC_PROPERTY_RADIO_GROUP:
            jkey = OBJ_RADIO_GROUP;
            i = va_arg(ap, int);
            grc_value = GRC_NUMBER;
            break;

        case GRC_PROPERTY_RADIO_TYPE:
            jkey = OBJ_RADIO_TYPE;
            i = va_arg(ap, int);
            grc_value = GRC_STRING;
            s = (char *)str_radio_type(i);
            break;

        case GRC_PROPERTY_PASSWORD_MODE:
            jkey = OBJ_PASSWORD;
            i = va_arg(ap, int);
            grc_value = GRC_BOOL;
            break;

        case GRC_PROPERTY_H_POSITION:
            jkey = OBJ_HORIZONTAL_POSITION;
            i = va_arg(ap, int);
            grc_value = GRC_STRING;
            s = (char *)str_horizontal_position(i);
            break;

        default:
            grc_set_errno(GRC_ERROR_UNKNOWN_PROPERTY);
            return -1;
    }

    /* Only supported properties are inserted */
    if (jkey != NULL) {
        switch (grc_value) {
            case GRC_NUMBER:
                c = cjson_create_number(i);
                break;

            case GRC_STRING:
                c = cjson_create_string(s);
                break;

            case GRC_BOOL:
                if (i == false)
                    c = cjson_create_false();
                else
                    c = cjson_create_true();

                break;
        }

        if (NULL == c) {
            grc_set_errno(GRC_ERROR_MEMORY);
            return -1;
        }

        wb = (struct writer_builder *)grc_get_internal_data(grc);
        cjson_add_item_to_object(wb->object, jkey, c);
    }

    va_end(ap);

    return 0;
}

int LIBEXPORT grc_GRC_objects_finish(grc_t *grc)
{
    return add_tmp_array(grc, OBJ_OBJECTS);
}

