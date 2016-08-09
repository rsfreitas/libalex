
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Dec 18 15:59:14 2014
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

#ifndef _LIBGRC_WRITE_H
#define _LIBGRC_WRITE_H			1

#ifndef LIBGRC_COMPILE
# ifndef _LIBGRC_H
#  error "Never use <write.h> directly; include <libgrc.h> instead."
# endif
#endif

/**
 * @name grc_GRC_create_colors
 * @brief Create the color block information.
 *
 * Add information about the DIALOG main colors to a GRC file.
 *
 * @param [in,out] grc: Previously created GRC structure.
 * @param [in] foreground: Foreground color name.
 * @param [in] background: Background color name.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int grc_GRC_create_colors(grc_t *grc, const char *foreground,
                          const char *background);

/**
 * @name grc_GRC_create_info
 * @brief Create the main DIALOG block information.
 *
 * @param [in,out] grc: Previously created GRC structure.
 * @param [in] width: Window width.
 * @param [in] height: Window height.
 * @param [in] color_depth: DIALOG colors.
 * @param [in] block_exit_keys: Flag to block Allegro's ctrl+alt+end
 *                              shortcut.
 * @param [in] mouse: Flag to enable mouse.
 * @param [in] ignore_esc_key: Flag to ignore ESC key.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int grc_GRC_create_info(grc_t *grc, unsigned int width, unsigned int height,
                        enum grc_color_depth color_depth, bool block_exit_keys,
                        bool mouse, bool ignore_esc_key);

/**
 * @name grc_GRC_keys_start
 * @brief Create the supported keys block.
 *
 * @param [in,out] grc: Previously created GRC structure.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int grc_GRC_keys_start(grc_t *grc);

/* TODO: Change this name to tag... */
/**
 * @name grc_GRC_add_key
 * @brief Add a supported key from a DIALOG.
 *
 * @param [in,out] grc: Previously created GRC structure.
 * @param [in] key: Supported key. Must be a string the way Allegro uses,
 *                  like KEY_A.
 * @param [in] name: Identification tag to the user.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int grc_GRC_add_key(grc_t *grc, const char *key, const char *name);

/**
 * @name grc_GRC_keys_finish
 * @brief Ends the supported keys block.
 *
 * @param [in,out] grc: Previously created GRC structure.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int grc_GRC_keys_finish(grc_t *grc);

/**
 * @name grc_GRC_objects_start
 * @brief Creates the block which will contain all DIALOG objects.
 *
 * @param [in,out] grc: Previously created GRC structure.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int grc_GRC_objects_start(grc_t *grc);

/**
 * @name grc_GRC_create_object
 * @brief Creates an object which will be added to the DIALOG.
 *
 * @param [in,out] grc: Previously created GRC structure.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int grc_GRC_create_object(grc_t *grc);

/**
 * @name grc_GRC_finish_object
 * @brief Ends the object added to the DIALOG.
 *
 * @param [in,out] grc: Previously created GRC structure.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int grc_GRC_finish_object(grc_t *grc);

/**
 * @name grc_GRC_set_object_property
 * @brief Add a property to a newly created object.
 *
 * @param [in,out] grc: Previously created GRC structure.
 * @param [in] prop: Inserted property.
 * @param [in] ...: Property value.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int grc_GRC_set_object_property(grc_t *grc, enum grc_object_property prop, ...);

/**
 * @name grc_GRC_objects_finish
 * @brief Ends the objects block.
 *
 * @param [in,out] grc: Previously created GRC structure.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int grc_GRC_objects_finish(grc_t *grc);

#endif

