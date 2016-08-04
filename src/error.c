
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Wed Dec 10 17:18:41 2014
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

static const char *__error_descriptions[] = {
    "Ok",
    "Invalid argument",
    "Internal memory error",
    "Unabled to load GRC data",
    "Object not previously defined",
    "Graphic mode information block not found",
    "Color block not found",
    "Error changing screen resolution",
    "Internal Allegro error",
    "Keyboard init error",
    "Objects block not found",
    "No object was found",
    "Error creating internal reference to object",
    "No key was found",
    "Object not found",
    "Unknown object",
    "Invalid library init mode",
    "The object does not support this number of keys",
    "Unsupported colors",
    "Unsupported number of colors",
    "Unknown property",
    "Unsupported type",
    "The DIALOG is not prepared to run"
};

static int __al_errno;
static const char *__unknown_error = "Unknown error";

void al_errno_clear(void)
{
    __al_errno = AL_NO_ERROR;
}

void al_set_errno(enum al_error_code code)
{
    __al_errno = code;
}

enum al_error_code LIBEXPORT al_get_last_error(void)
{
    return __al_errno;
}

const char LIBEXPORT *al_strerror(enum al_error_code code)
{
    if (code >= AL_MAX_ERROR_CODE)
        return __unknown_error;

    return __error_descriptions[code];
}

