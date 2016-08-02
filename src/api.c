
/*
 * Description: Main library API.
 *
 * Author: Rodrigo Freitas
 * Created at: Sat Dec 13 22:21:54 2014
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
#include "gui/objects.h"

#define LOAD_FROM_MEM       0
#define LOAD_FROM_FILE      1
#define LOAD_BARE_DATA      2

static int start_grc(struct al_grc *grc)
{
/*    if (grc->use_gfx == true) {
        if (gui_change_resolution(grc) < 0)
            return -1;
    }

    if (gui_load_colors(grc) < 0)
        return -1;
*/
    return 0;
}

static struct al_grc *al_grc_init(const char *grc_data, int load_mode,
    bool gfx)
{
    struct al_grc *grc;
    int ret = 0;

    al_errno_clear();
    grc = new_grc();

    if (NULL == grc)
        return NULL;

    /* Load the GRC (file or buffer) to the memory */
    if (load_mode == LOAD_FROM_FILE)
        ret = parse_file(grc, grc_data);
    else if (load_mode == LOAD_FROM_MEM)
        ret = parse_mem(grc, grc_data);
    else if (load_mode == LOAD_BARE_DATA) {
        /*
         * We allow creating an object in this mode only when creating a
         * GRC file in memory.
         */
        return grc;
    } else {
        al_set_errno(AL_ERROR_INVALID_LOAD_MODE);
        goto end_block;
    }

    if (ret < 0) {
        al_set_errno(AL_ERROR_PARSE_GRC);
        goto end_block;
    }

    grc->use_gfx = gfx;

    if (info_parse(grc) < 0)
        goto end_block;

    /*
     * We initialize Allegro here, so we can use anything from it from 
     * this point. Like their color conversion, we need to do this things
     * rigth after...
     */
    if (gui_init(grc) < 0)
        goto end_block;

    if (color_parse(grc) < 0)
        goto end_block;

    /* Do the translation of every GRC object to internal grc_object. */
    if (parse_objects(grc) < 0)
        goto end_block;

    return grc;

end_block:
    destroy_grc(grc);
    return NULL;
}

struct al_grc LIBEXPORT *al_grc_init_from_file(const char *grc_file,
    bool gfx)
{
    return al_grc_init(grc_file, LOAD_FROM_FILE, gfx);
}

struct al_grc LIBEXPORT *al_grc_init_from_mem(const char *data,
    bool gfx)
{
    return al_grc_init(data, LOAD_FROM_MEM, gfx);
}

struct al_grc LIBEXPORT *al_grc_create(void)
{
    return al_grc_init(NULL, LOAD_BARE_DATA, 0);
}

/* TODO: Why do we need this function? */
int LIBEXPORT al_grc_init_from_bare_data(struct al_grc *grc)
{
    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    /* TODO: Parse? */
    /* TODO: Start gui? */
    return start_grc(grc);
}

int LIBEXPORT al_grc_uninit(struct al_grc *grc)
{
    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    /* Return the gfx to text mode */
    if (grc->use_gfx == true)
        gui_reset_resolution();

    /* Free the object */
    destroy_grc(grc);
    cexit();

    return 0;
}

int LIBEXPORT al_grc_prepare_dialog(struct al_grc *grc)
{
    int ret;

    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    ret = DIALOG_create(grc);

    if (ret == 0)
        /* And now we're prepared to run the DIALOG */
        grc->are_we_prepared = true;

    return ret;
}

int LIBEXPORT al_grc_do_dialog(struct al_grc *grc)
{
    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    /* We're not prepared to run the DIALOG yet */
    if (grc->are_we_prepared == false) {
        al_set_errno(AL_ERROR_NOT_PREPARED_YET);
        return -1;
    }

    run_DIALOG(grc);

    return 0;
}

static bool object_has_callback_data(struct al_grc *grc, DIALOG *d)
{
    struct al_callback_data *acd = NULL;

    acd = d->dp3;

    if (NULL == acd)
        return false;

    /* This should not happen at all. */
    return (acd->grc == grc) ? true : false;
}

int LIBEXPORT al_grc_set_callback(struct al_grc *grc, const char *object_name,
    int (*callback)(struct al_callback_data *), void *arg)
{
    DIALOG *d;
    struct al_callback_data *acd = NULL;
    MENU *m;

    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    d = get_DIALOG_from_grc(grc, object_name);

    if (NULL == d) {
        /* Checks if it's a menu item or not */
        m = get_MENU_from_grc(grc, object_name);

        if (NULL == m)
            return -1;

        /*
         * Although the callback functions receive a structure as an argument,
         * in case of a menu item, it does receive none, due an internal
         * Allegro implementation.
         */
        m->proc = (int (*)(void))callback;

        return 0;
    }

    /*
     * @dp3 is used by the library as a way to store informations related to
     * the object callback function.
     */

    if (object_has_callback_data(grc, d) == false) {
        acd = new_callback_data();

        if (NULL == acd) {
            al_set_errno(AL_ERROR_MEMORY);
            return -1;
        }

        acd->grc = (void *)grc;
        d->dp3 = acd;

        /* lock */
        grc->callback_data = cdll_unshift(grc->callback_data, acd);
        /* unlock */
    } else
        acd = d->dp3;

    /* Replace the callback */
    acd->callback = callback;
    acd->user_arg = arg;

    return 0;
}

/*
 * TODO: Change this to use a cvalue_t.
 */
int LIBEXPORT al_grc_get_callback_data(struct al_callback_data *acd,
    enum al_data_type data, ...)
{
    const char *ifmt = "%d\0", *sfmt = "%[^|]";
    va_list ap;
    char tmp[512] = {0};

    al_errno_clear();

    if (NULL == acd) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    va_start(ap, NULL);

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

    va_end(ap);

    return 0;
}

void LIBEXPORT *al_grc_get_callback_user_arg(struct al_callback_data *acd)
{
    al_errno_clear();

    if (NULL == acd) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return NULL;
    }

    return acd->user_arg;
}

struct al_grc LIBEXPORT *al_grc_get_callback_grc(struct al_callback_data *acd)
{
    al_errno_clear();

    if (NULL == acd) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return NULL;
    }

    return acd->grc;
}

int LIBEXPORT al_grc_object_set_data(struct al_grc *grc, const char *object_name,
    enum al_data_type type, void *data)
{
    DIALOG *d;
    int s;
    struct al_callback_data *acd = NULL;

    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    d = get_DIALOG_from_grc(grc, object_name);

    if (NULL == d)
        return -1;

    switch (type) {
        case AL_DT_RADIO_STATE:
        case AL_DT_CHECKBOX_STATE:
            s = *((int *)&data);

            if (s == true)
                d->flags |= D_SELECTED;
            else
                d->flags &= ~D_SELECTED;

            break;

        case AL_DT_D1:
        case AL_DT_SLIDER_LIMIT:
            d->d1 = *((int *)&data);
            break;

        case AL_DT_D2:
        case AL_DT_SLIDER_POSITION:
            d->d2 = *((int *)&data);
            break;

        case AL_DT_DP:
        case AL_DT_EDIT_VALUE:
        case AL_DT_TEXT:
        case AL_DT_LIST_CONTENT_BUILD:
        case AL_DT_ICON:
            d->dp = data;
            break;

        case AL_DT_DP2:
            d->dp2 = data;
            break;

        case AL_DT_DP3:
            /*
             * To keep a standard in all object implemented internally we
             * manipulate the callback structure from the object.
             */
            acd = d->dp3;

            if (NULL == acd) {
                acd = new_callback_data();

                if (NULL == acd) {
                    al_set_errno(AL_ERROR_MEMORY);
                    return -1;
                }

                acd->grc = (void *)grc;
                d->dp3 = acd;
            }

            acd->user_arg = data;
            break;

        default:
            al_set_errno(AL_ERROR_UNSUPPORTED_DATA_TYPE);
            return -1;
    }

    return 0;
}

int LIBEXPORT al_grc_object_set_proc(struct al_grc *grc,
    const char *object_name, int (*function)(int, DIALOG *, int))
{
    DIALOG *d;

    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    d = get_DIALOG_from_grc(grc, object_name);

    if (NULL == d)
        return -1;

    d->proc = function;

    return 0;
}

int LIBEXPORT al_grc_object_send_message(struct al_grc *grc,
    const char *object_name, int msg, int c)
{
    DIALOG *d;

    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    d = get_DIALOG_from_grc(grc, object_name);

    if (NULL == d)
        return -1;

    object_message(d, msg, c);

    return 0;
}

void LIBEXPORT *al_grc_object_get_data(struct al_grc *grc,
    const char *object_name, enum al_data_type type, ...)
{
    const char *ifmt = "%d\0";
    va_list ap;
    char tmp[512] = {0};
    void *data = NULL;
    int value = -1;
    DIALOG *d;

    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return NULL;
    }

    d = get_DIALOG_from_grc(grc, object_name);

    if (NULL == d)
        return NULL;

    va_start(ap, NULL);

    switch (type) {
        case AL_DT_RADIO_STATE:
        case AL_DT_CHECKBOX_STATE:
            if (d->flags & D_SELECTED)
                value = 1;
            else
                value = 0;

            break;

        case AL_DT_D1:
        case AL_DT_SLIDER_LIMIT:
        case AL_DT_LIST_POSITION:
            value = d->d1;
            break;

        case AL_DT_D2:
        case AL_DT_SLIDER_POSITION:
            value = d->d2;
            break;

        case AL_DT_DP:
        case AL_DT_EDIT_VALUE:
        case AL_DT_TEXT:
            data = d->dp;
            break;

        case AL_DT_DP2:
            data = d->dp2;
            break;

        case AL_DT_DP3:
            data = d->dp3;
            break;

        default:
            al_set_errno(AL_ERROR_UNSUPPORTED_DATA_TYPE);
            return NULL;
    }

    if (value != -1) {
        snprintf(tmp, sizeof(tmp) - 1, ifmt, value);
        vsscanf(tmp, ifmt, ap);

        /*
         * Sets the return to a valid pointer, so the user can validate the
         * function call.
         */
        data = grc;
    }

    va_end(ap);

    return data;
}

int LIBEXPORT al_grc_object_hide(struct al_grc *grc, const char *object_name)
{
    DIALOG *d;

    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    d = get_DIALOG_from_grc(grc, object_name);

    if (NULL == d)
        return -1;

    d->flags |= D_HIDDEN;

    return 0;
}

int LIBEXPORT al_grc_object_show(struct al_grc *grc, const char *object_name)
{
    DIALOG *d;

    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    d = get_DIALOG_from_grc(grc, object_name);

    if (NULL == d)
        return -1;

    d->flags &= ~D_HIDDEN;

    return 0;
}

int LIBEXPORT al_grc_log(struct al_grc *grc, const char *object_name,
    const char *msg, const char *color)
{
    DIALOG *d;

    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    d = get_DIALOG_from_grc(grc, object_name);

    if (NULL == d)
        return -1;

    /* Sets internal message */
    gui_messages_set(d->d1, d->fg, msg, color);

    /* Notify the object about it */
    object_message(d, MSG_NEW_LOG_TEXT, 0);

    return 0;
}

int LIBEXPORT al_grc_list_get_selected_index(struct al_grc *grc,
    const char *object_name)
{
    int p = -1;

    if ((al_grc_object_get_data(grc, object_name, AL_DT_LIST_POSITION,
                                &p) == NULL) &&
        (al_get_last_error() != AL_NO_ERROR))
    {
        return -1;
    }

    return p;
}

int LIBEXPORT al_grc_checkbox_get_status(struct al_grc *grc,
    const char *object_name)
{
    int st = -1;

    if ((al_grc_object_get_data(grc, object_name, AL_DT_CHECKBOX_STATE,
                                &st) == NULL) &&
        (al_get_last_error() != AL_NO_ERROR))
    {
        return -1;
    }

    return st;
}

int LIBEXPORT al_grc_radio_get_status(struct al_grc *grc,
    const char *object_name)
{
    int st = -1;

    if ((al_grc_object_get_data(grc, object_name, AL_DT_RADIO_STATE,
                                &st) == NULL) &&
        (al_get_last_error() != AL_NO_ERROR))
    {
        return -1;
    }

    return st;
}

char LIBEXPORT *al_grc_edit_get_data(struct al_grc *grc,
    const char *object_name)
{
    char *v = NULL;

    v = (char *)al_grc_object_get_data(grc, object_name, AL_DT_EDIT_VALUE,
                                       NULL);

    return v;
}

/*int al_grc_set_bitmap_mem(struct al_grc *grc, const char *object_name,
    unsigned char *bimg, unsigned int bsize, int image_fmt)
{
//    object_message(d, MSG_LOAD_IMAGE, c);
    return -1;
}

int al_grc_set_bitmap(struct al_grc *grc, const char *object_name,
    const char *filename, int image_fmt)
{
    return -1;
}*/

