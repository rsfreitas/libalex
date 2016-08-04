
/*
 * Description: Functions to manipulate a 'struct callback_data' structure.
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Jul 28 09:21:58 2016
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
#include <stdarg.h>

#include "libalex.h"

/* Structure to be used in the callback functions */
struct callback_data {
    char            *value_string;
    int             value_int;
    void            *user_arg;
    void            *grc; /* Pointer to the main structure */

    /* internal */
    int             (*callback)(struct callback_data *);
};

/*
 * Create and return a structure of type 'struct callback_data'.
 */
struct callback_data *new_callback_data(void)
{
    struct callback_data *cd = NULL;

    cd = calloc(1, sizeof(struct callback_data));

    if (NULL == cd)
        return NULL;

    return cd;
}

void destroy_callback_data(void *a)
{
    struct callback_data *p = (struct callback_data *)a;

    if (NULL == p)
        return;

    free(p);
}

/*
 * Sets an object callback data to point to the main library structure.
 */
void set_object_callback_data(struct grc_object *gobject,
    struct al_grc *grc)
{
    struct callback_data *acd;
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

int get_callback_data(struct callback_data *acd, enum al_data_type data,
    va_list ap)
{
    const char *ifmt = "%d\0", *sfmt = "%[^|]";
    char tmp[512] = {0};

    if (NULL == acd)
        return -1;

    switch (data) {
        case AL_DT_KEY_SCANCODE:
        case AL_DT_SLIDER_POSITION:
        case AL_DT_CHECKBOX_STATE:
        case AL_DT_RADIO_STATE:
        case AL_DT_LIST_POSITION:
            snprintf(tmp, sizeof(tmp) - 1, ifmt, acd->value_int);
            vsscanf(tmp, ifmt, ap);
            break;

        case AL_DT_EDIT_VALUE:
            snprintf(tmp, sizeof(tmp) - 1, "%s", acd->value_string);
            vsscanf(tmp, sfmt, ap);
            break;

        default:
            al_set_errno(AL_ERROR_UNSUPPORTED_DATA_TYPE);
            return -1;
    }

    return 0;
}

void *get_callback_user_arg(struct callback_data *acd)
{
    if (NULL == acd)
        return NULL;

    return acd->user_arg;
}

struct al_grc *get_callback_grc(struct callback_data *acd)
{
    if (NULL == acd)
        return NULL;

    return acd->grc;
}

static bool object_has_callback_data(struct al_grc *grc, DIALOG *d)
{
    struct callback_data *acd = NULL;

    acd = d->dp3;

    if (NULL == acd)
        return false;

    /* This should not happen at all. */
    return (acd->grc == grc) ? true : false;
}

int set_callback(struct al_grc *grc, DIALOG *dlg,
    int (*callback)(struct callback_data *), void *arg)
{
    struct callback_data *acd = NULL;

    if ((NULL == grc) || (NULL == dlg))
        return -1;

    /*
     * @dp3 is used by the library as a way to store informations related to
     * the object callback function.
     */

    if (object_has_callback_data(grc, dlg) == false) {
        acd = new_callback_data();

        if (NULL == acd)
            return -1;

        acd->grc = (void *)grc;
        dlg->dp3 = acd;
    } else
        acd = dlg->dp3;

    /* Replace the callback */
    if (callback != NULL)
        acd->callback = callback;

    if (arg != NULL)
        acd->user_arg = arg;

    return 0;
}

/*
 * Execute a callback function from an object. If there is no function we
 * return a @default_return value.
 */
int run_callback(struct callback_data *acd, unsigned int default_return)
{
    if (acd->callback != NULL)
        return (acd->callback)(acd);

    return default_return;
}

void callback_set_int(struct callback_data *acd, int value)
{
    if (NULL == acd)
        return;

    acd->value_int = value;
}

void callback_set_string(struct callback_data *acd, char *value)
{
    if (NULL == acd)
        return;

    acd->value_string = value;
}

