
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Fri Jul 29 09:50:52 2016
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

#include "libalex.h"

/*
 * Parse main DIALOG informations, such as screen resolution, color depth,
 * etc.
 */
int info_parse(struct al_grc *grc)
{
    cjson_t *jinfo;
    struct property_detail *dt;

    jinfo = grc_get_object(grc, OBJ_INFO);

    if (NULL == jinfo) {
        al_set_errno(AL_ERROR_INFO_BLOCK_NOT_FOUND);
        return -1;
    }

    /* width */
    dt = get_property_detail(AL_GRC_JOBJ_WIDTH);

    if (NULL == dt)
        goto unknown_grc_key_block;

    grc->gfx.width = grc_get_object_value(jinfo, property_detail_string(dt),
                                          AL_DEFAULT_WIDTH);

    /* height */
    dt = get_property_detail(AL_GRC_JOBJ_HEIGHT);

    if (NULL == dt)
        goto unknown_grc_key_block;

    grc->gfx.height = grc_get_object_value(jinfo,
                                           property_detail_string(dt),
                                           AL_DEFAULT_HEIGHT);

    /* color depth */
    dt = get_property_detail(AL_GRC_JOBJ_COLOR_DEPTH);

    if (NULL == dt)
        goto unknown_grc_key_block;

    grc->gfx.color_depth = grc_get_object_value(jinfo, property_detail_string(dt),
                                                AL_DEFAULT_COLOR_DEPTH);

    /* block exit keys */
    dt = get_property_detail(AL_GRC_JOBJ_BLOCK_EXIT_KEYS);

    if (NULL == dt)
        goto unknown_grc_key_block;

    grc->gfx.block_keys = grc_get_object_value(jinfo, property_detail_string(dt),
                                               true);

    /* mouse */
    dt = get_property_detail(AL_GRC_JOBJ_MOUSE);

    if (NULL == dt)
        goto unknown_grc_key_block;

    grc->gfx.use_mouse = grc_get_object_value(jinfo, property_detail_string(dt),
                                              false);

    /* ignore_esc_key */
    dt = get_property_detail(AL_GRC_JOBJ_IGNORE_ESC_KEY);

    if (NULL == dt)
        goto unknown_grc_key_block;

    grc->ignore_esc_key = grc_get_object_value(jinfo,
                                               property_detail_string(dt),
                                               false);

    return 0;

unknown_grc_key_block:
    al_set_errno(AL_ERROR_UNDEFINED_GRC_KEY);
    return -1;
}

int info_color_depth(struct al_grc *grc)
{
    if (NULL == grc)
        return AL_DEFAULT_COLOR_DEPTH;

    return grc->gfx.color_depth;
}

