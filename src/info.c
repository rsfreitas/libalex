
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Fri Jul 29 09:50:52 2016
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

struct gfx_info_s {
    /* Internal info */
    bool                    use_gfx;
    bool                    are_we_prepared;

    enum grc_line_break     lbreak;                 /** 'messages_log_box' break
                                                        line type */

    bool                    ignore_esc_key;         /** Flag to ignore the ESC
                                                        key */

    bool                    virtual_keyboard;       /** Flag to use the virtual
                                                        keyboard */

    bool                    esc_key_user_defined;   /** Flag showing if the user
                                                        has declared the ESC
                                                        key */

    /* Info from the GRC file */
    int                     width;
    int                     height;
    int                     color_depth;
    bool                    block_keys;
    bool                    use_mouse;
};

struct gfx_info_s *info_start(void)
{
    struct gfx_info_s *i = NULL;

    i = calloc(1, sizeof(struct gfx_info_s));

    if (NULL == i)
        return NULL;

    return i;
}

void info_finish(struct gfx_info_s *info)
{
    if (NULL == info)
        return;

    free(info);
}

int info_set_value(struct gfx_info_s *info, enum gfx_info field, ...)
{
    va_list ap;
    int value;

    va_start(ap, NULL);
    value = va_arg(ap, int);
    va_end(ap);

    switch (field) {
        case INFO_USE_GFX:
            info->use_gfx = value;
            break;

        case INFO_ARE_WE_PREPARED:
            info->are_we_prepared = value;
            break;

        case INFO_LINE_BREAK:
            info->lbreak = value;
            break;

        case INFO_IGNORE_ESC_KEY:
            info->ignore_esc_key = value;
            break;

        case INFO_VIRTUAL_KEYBOARD:
            info->virtual_keyboard = value;
            break;

        case INFO_ESC_KEY_USER_DEFINED:
            info->esc_key_user_defined = value;
            break;

        case INFO_WIDTH:
            info->width = value;
            break;

        case INFO_HEIGHT:
            info->height = value;
            break;

        case INFO_COLOR_DEPTH:
            info->color_depth = value;
            break;

        case INFO_BLOCK_KEYS:
            info->block_keys = value;
            break;

        case INFO_USE_MOUSE:
            info->use_mouse = value;
            break;

        default:
            return -1;
    }

    return 0;
}

int info_get_value(struct gfx_info_s *info, enum gfx_info field)
{
    if (NULL == info)
        return -1;

    switch (field) {
        case INFO_USE_GFX:
            return info->use_gfx;

        case INFO_ARE_WE_PREPARED:
            return info->are_we_prepared;

        case INFO_LINE_BREAK:
            return info->lbreak;

        case INFO_IGNORE_ESC_KEY:
            return info->ignore_esc_key;

        case INFO_VIRTUAL_KEYBOARD:
            return info->virtual_keyboard;

        case INFO_ESC_KEY_USER_DEFINED:
            return info->esc_key_user_defined;

        case INFO_WIDTH:
            return info->width;

        case INFO_HEIGHT:
            return info->height;

        case INFO_COLOR_DEPTH:
            return info->color_depth;

        case INFO_BLOCK_KEYS:
            return info->block_keys;

        case INFO_USE_MOUSE:
            return info->use_mouse;
    }

    /* default */
    return -1;
}

/*
 * Parse main DIALOG informations, such as screen resolution, color depth,
 * etc.
 */
int info_parse(struct grc_s *grc)
{
    cjson_t *jinfo;
    struct property_detail *dt;

    jinfo = grc_get_object(grc, OBJ_INFO);

    if (NULL == jinfo) {
        grc_set_errno(GRC_ERROR_INFO_BLOCK_NOT_FOUND);
        return -1;
    }

    /* width */
    dt = get_property_detail(GRC_PROPERTY_WIDTH);

    if (NULL == dt)
        goto unknown_grc_key_block;

    info_set_value(grc->info, INFO_WIDTH,
                   grc_get_object_value(jinfo, property_detail_string(dt),
                                        DEFAULT_WIDTH), NULL);

    /* height */
    dt = get_property_detail(GRC_PROPERTY_HEIGHT);

    if (NULL == dt)
        goto unknown_grc_key_block;

    info_set_value(grc->info, INFO_HEIGHT,
                   grc_get_object_value(jinfo, property_detail_string(dt),
                                        DEFAULT_HEIGHT), NULL);

    /* color depth */
    dt = get_property_detail(GRC_PROPERTY_COLOR_DEPTH);

    if (NULL == dt)
        goto unknown_grc_key_block;

    info_set_value(grc->info, INFO_COLOR_DEPTH,
                   grc_get_object_value(jinfo, property_detail_string(dt),
                                        DEFAULT_COLOR_DEPTH), NULL);

    /* block exit keys */
    dt = get_property_detail(GRC_PROPERTY_BLOCK_EXIT_KEYS);

    if (NULL == dt)
        goto unknown_grc_key_block;

    info_set_value(grc->info, INFO_BLOCK_KEYS,
                   grc_get_object_value(jinfo, property_detail_string(dt),
                                        true), NULL);

    /* mouse */
    dt = get_property_detail(GRC_PROPERTY_MOUSE);

    if (NULL == dt)
        goto unknown_grc_key_block;

    info_set_value(grc->info, INFO_USE_MOUSE,
                   grc_get_object_value(jinfo, property_detail_string(dt),
                                        false), NULL);

    /* ignore_esc_key */
    dt = get_property_detail(GRC_PROPERTY_IGNORE_ESC_KEY);

    if (NULL == dt)
        goto unknown_grc_key_block;

    info_set_value(grc->info, INFO_IGNORE_ESC_KEY,
                   grc_get_object_value(jinfo, property_detail_string(dt),
                                        false), NULL);

    return 0;

unknown_grc_key_block:
    grc_set_errno(GRC_ERROR_UNDEFINED_GRC_KEY);
    return -1;
}

int info_color_depth(struct grc_s *grc)
{
    if (NULL == grc)
        return DEFAULT_COLOR_DEPTH;

    return info_get_value(grc->info, INFO_COLOR_DEPTH);
}

