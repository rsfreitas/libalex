
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Wed Dec 10 17:21:26 BRST 2014
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

#ifndef _LIBALEX_ERROR_H
#define _LIBALEX_ERROR_H                            1

#ifndef LIBALEX_COMPILE
# ifndef _LIBALEX_H
#  error "Never use <error.h> directly; include <libalex.h> instead."
# endif
#endif

enum al_error_code {
    AL_NO_ERROR = 0,

    AL_ERROR_NULL_ARG,
    AL_ERROR_MEMORY,
    AL_ERROR_PARSE_GRC,
    AL_ERROR_UNDEFINED_GRC_KEY,
    AL_ERROR_INFO_BLOCK_NOT_FOUND,
    AL_ERROR_COLORS_BLOCK_NOT_FOUND,
    AL_ERROR_SET_GFX_MODE,
    AL_ERROR_LIB_INIT,
    AL_ERROR_KEYBOARD_INIT,
    AL_ERROR_OBJECTS_BLOCK_NOT_FOUND,
    AL_ERROR_NO_OBJECTS,
    AL_ERROR_NEW_REF,
    AL_ERROR_NO_KEYS,
    AL_ERROR_OBJECT_NOT_FOUND,
    AL_ERROR_UNKNOWN_OBJECT_TYPE,
    AL_ERROR_INVALID_LOAD_MODE,
    AL_ERROR_UNSUPPORTED_EDIT_INPUT_LENGTH,
    AL_ERROR_UNSUPPORTED_COLORS,
    AL_ERROR_UNSUPPORTED_COLOR_DEPTH,
    AL_ERROR_UNKNOWN_PROPERTY,
    AL_ERROR_UNSUPPORTED_DATA_TYPE,

    AL_MAX_ERROR_CODE
};

#ifdef LIBALEX_COMPILE
void al_errno_clear(void);
void al_set_errno(enum al_error_code code);
#endif

/**
 * @name al_get_last_error
 * @brief Gets the last error code internally occurred.
 *
 * @return Returns the last error code.
 */
enum al_error_code al_get_last_error(void);

/**
 * @name al_strerror
 * @brief Converts a numeric error code in a text message.
 *
 * @param [in] error_code: Numeric error code.
 *
 * @return Returns the text corresponding the error code.
 */
const char *al_strerror(enum al_error_code code);

#endif

