
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Wed Dec 10 17:21:26 BRST 2014
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

#ifndef _LIBGRC_ERROR_H
#define _LIBGRC_ERROR_H                            1

#ifndef LIBGRC_COMPILE
# ifndef _LIBGRC_H
#  error "Never use <error.h> directly; include <libgrc.h> instead."
# endif
#endif

enum grc_error_code {
    GRC_NO_ERROR = 0,

    GRC_ERROR_NULL_ARG,
    GRC_ERROR_MEMORY,
    GRC_ERROR_PARSE_GRC,
    GRC_ERROR_UNDEFINED_GRC_KEY,
    GRC_ERROR_INFO_BLOCK_NOT_FOUND,
    GRC_ERROR_COLORS_BLOCK_NOT_FOUND,
    GRC_ERROR_SET_GFX_MODE,
    GRC_ERROR_LIB_INIT,
    GRC_ERROR_KEYBOARD_INIT,
    GRC_ERROR_OBJECTS_BLOCK_NOT_FOUND,
    GRC_ERROR_NO_OBJECTS,
    GRC_ERROR_NEW_REF,
    GRC_ERROR_NO_KEYS,
    GRC_ERROR_OBJECT_NOT_FOUND,
    GRC_ERROR_UNKNOWN_OBJECT_TYPE,
    GRC_ERROR_INVALID_LOAD_MODE,
    GRC_ERROR_UNSUPPORTED_EDIT_INPUT_LENGTH,
    GRC_ERROR_UNSUPPORTED_COLORS,
    GRC_ERROR_UNSUPPORTED_COLOR_DEPTH,
    GRC_ERROR_UNKNOWN_PROPERTY,
    GRC_ERROR_UNSUPPORTED_DATA_TYPE,
    GRC_ERROR_NOT_PREPARED_YET,

    GRC_MAX_ERROR_CODE
};

/**
 * @name grc_get_last_error
 * @brief Gets the last error code internally occurred.
 *
 * @return Returns the last error code.
 */
enum grc_error_code grc_get_last_error(void);

/**
 * @name grc_strerror
 * @brief Converts a numeric error code in a text message.
 *
 * @param [in] error_code: Numeric error code.
 *
 * @return Returns the text corresponding the error code.
 */
const char *grc_strerror(enum grc_error_code code);

#endif

