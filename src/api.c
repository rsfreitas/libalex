
/*
 * Description: Main library API.
 *
 * Author: Rodrigo Freitas
 * Created at: Sat Dec 13 22:21:54 2014
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
#include "gui/objects.h"

#define LOAD_FROM_MEM       0
#define LOAD_FROM_FILE      1
#define LOAD_BARE_DATA      2

static int start_grc(grc_t *grc __attribute__((unused)))
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

static grc_t *grc_init(const char *grc_data, int load_mode,
    bool gfx)
{
    grc_t *grc;
    int ret = 0;
    struct gfx_info_s *info;

    grc_errno_clear();
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
        grc_set_errno(GRC_ERROR_INVALID_LOAD_MODE);
        goto end_block;
    }

    if (ret < 0) {
        grc_set_errno(GRC_ERROR_PARSE_GRC);
        goto end_block;
    }

    info = grc_get_info(grc);
    info_set_value(info, INFO_USE_GFX, gfx, NULL);

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

grc_t LIBEXPORT *grc_init_from_file(const char *grc_file,
    bool gfx)
{
    return grc_init(grc_file, LOAD_FROM_FILE, gfx);
}

grc_t LIBEXPORT *grc_init_from_mem(const char *data,
    bool gfx)
{
    return grc_init(data, LOAD_FROM_MEM, gfx);
}

grc_t LIBEXPORT *grc_create(void)
{
    return grc_init(NULL, LOAD_BARE_DATA, 0);
}

/* TODO: Why do we need this function? */
int LIBEXPORT grc_init_from_bare_data(grc_t *grc)
{
    grc_errno_clear();

    if (NULL == grc) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return -1;
    }

    /* TODO: Parse? */
    /* TODO: Start gui? */
    return start_grc(grc);
}

int LIBEXPORT grc_uninit(grc_t *grc)
{
    struct gfx_info_s *info;

    grc_errno_clear();

    if (NULL == grc) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return -1;
    }

    info = grc_get_info(grc);

    /* Return the gfx to text mode */
    if (info_get_value(info, INFO_USE_GFX) == true)
        gui_reset_resolution();

    /* Free the object */
    destroy_grc(grc);

    return 0;
}

int LIBEXPORT grc_prepare_dialog(grc_t *grc)
{
    int ret;
    struct gfx_info_s *info;

    grc_errno_clear();

    if (NULL == grc) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return -1;
    }

    ret = DIALOG_create(grc);

    if (ret == 0) {
        /* And now we're prepared to run the DIALOG */
        info = grc_get_info(grc);
        info_set_value(info, INFO_ARE_WE_PREPARED, true, NULL);
    }

    return ret;
}

int LIBEXPORT grc_do_dialog(grc_t *grc)
{
    struct gfx_info_s *info;

    grc_errno_clear();

    if (NULL == grc) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return -1;
    }

    info = grc_get_info(grc);

    /* We're not prepared to run the DIALOG yet */
    if (info_get_value(info, INFO_ARE_WE_PREPARED) == false) {
        grc_set_errno(GRC_ERROR_NOT_PREPARED_YET);
        return -1;
    }

    run_DIALOG(grc);

    return 0;
}

int LIBEXPORT grc_set_callback(grc_t *grc, const char *object_name,
    int (*callback)(grc_callback_data_t *), void *arg)
{
    DIALOG *d;
    MENU *m;

    grc_errno_clear();

    if (NULL == grc) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return -1;
    }

    d = grc_get_DIALOG_from_tag(grc, object_name);

    if (NULL == d) {
        /* Checks if it's a menu item or not */
        m = grc_get_MENU_from_tag(grc, object_name);

        if (NULL == m)
            return -1;

        /*
         * Although the callback functions receive a structure as an argument,
         * in case of a menu item, it receives none, due an internal Allegro
         * implementation.
         */
        m->proc = (int (*)(void))callback;

        return 0;
    }

    return set_callback(grc, d, callback, arg);
}

/*
 * TODO: Change this to use a cvalue_t.
 */
int LIBEXPORT grc_get_callback_data(grc_callback_data_t *acd,
    enum grc_object_member member, ...)
{
    va_list ap;
    int ret;

    grc_errno_clear();

    if (NULL == acd) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return -1;
    }

    va_start(ap, NULL);
    ret = get_callback_data(acd, member, ap);
    va_end(ap);

    return ret;
}

void LIBEXPORT *grc_get_callback_user_arg(grc_callback_data_t *acd)
{
    grc_errno_clear();

    if (NULL == acd) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return NULL;
    }

    return get_callback_user_arg(acd);
}

grc_t LIBEXPORT *grc_get_callback_grc(grc_callback_data_t *acd)
{
    grc_errno_clear();

    if (NULL == acd) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return NULL;
    }

    return get_callback_grc(acd);
}

int LIBEXPORT grc_object_set_data(grc_t *grc, const char *object_name,
    enum grc_object_member member, void *data)
{
    DIALOG *d;
    int s;

    grc_errno_clear();

    if (NULL == grc) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return -1;
    }

    d = grc_get_DIALOG_from_tag(grc, object_name);

    if (NULL == d)
        return -1;

    switch (member) {
        case GRC_MEMBER_RADIO_STATE:
        case GRC_MEMBER_CHECKBOX_STATE:
            s = *((int *)&data);

            if (s == true)
                d->flags |= D_SELECTED;
            else
                d->flags &= ~D_SELECTED;

            break;

        case GRC_MEMBER_D1:
        case GRC_MEMBER_SLIDER_LIMIT:
            d->d1 = *((int *)&data);
            break;

        case GRC_MEMBER_D2:
        case GRC_MEMBER_SLIDER_POSITION:
            d->d2 = *((int *)&data);
            break;

        case GRC_MEMBER_DP:
        case GRC_MEMBER_EDIT_VALUE:
        case GRC_MEMBER_TEXT:
        case GRC_MEMBER_LIST_CONTENT_BUILD:
        case GRC_MEMBER_ICON:
            d->dp = data;
            break;

        case GRC_MEMBER_DP2:
            d->dp2 = data;
            break;

        case GRC_MEMBER_DP3:
            /*
             * To keep a standard in all object implemented internally we
             * manipulate the callback structure from the object.
             */
            set_callback(grc, d, NULL, data);
            break;

        default:
            grc_set_errno(GRC_ERROR_UNSUPPORTED_DATA_TYPE);
            return -1;
    }

    return 0;
}

int LIBEXPORT grc_object_set_proc(grc_t *grc,
    const char *object_name, int (*function)(int, DIALOG *, int))
{
    DIALOG *d;

    grc_errno_clear();

    if (NULL == grc) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return -1;
    }

    d = grc_get_DIALOG_from_tag(grc, object_name);

    if (NULL == d)
        return -1;

    d->proc = function;

    return 0;
}

int LIBEXPORT grc_object_send_message(grc_t *grc,
    const char *object_name, int msg, int c)
{
    DIALOG *d;

    grc_errno_clear();

    if (NULL == grc) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return -1;
    }

    d = grc_get_DIALOG_from_tag(grc, object_name);

    if (NULL == d)
        return -1;

    object_message(d, msg, c);

    return 0;
}

void LIBEXPORT *grc_object_get_data(grc_t *grc,
    const char *object_name, enum grc_object_member member, ...)
{
    const char *ifmt = "%d\0";
    va_list ap;
    char tmp[512] = {0};
    void *data = NULL;
    int value = -1;
    DIALOG *d;

    grc_errno_clear();

    if (NULL == grc) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return NULL;
    }

    d = grc_get_DIALOG_from_tag(grc, object_name);

    if (NULL == d)
        return NULL;

    va_start(ap, NULL);

    switch (member) {
        case GRC_MEMBER_RADIO_STATE:
        case GRC_MEMBER_CHECKBOX_STATE:
            if (d->flags & D_SELECTED)
                value = 1;
            else
                value = 0;

            break;

        case GRC_MEMBER_D1:
        case GRC_MEMBER_SLIDER_LIMIT:
        case GRC_MEMBER_LIST_POSITION:
            value = d->d1;
            break;

        case GRC_MEMBER_D2:
        case GRC_MEMBER_SLIDER_POSITION:
            value = d->d2;
            break;

        case GRC_MEMBER_DP:
        case GRC_MEMBER_EDIT_VALUE:
        case GRC_MEMBER_TEXT:
            data = d->dp;
            break;

        case GRC_MEMBER_DP2:
            data = d->dp2;
            break;

        case GRC_MEMBER_DP3:
            data = d->dp3;
            break;

        default:
            grc_set_errno(GRC_ERROR_UNSUPPORTED_DATA_TYPE);
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

int LIBEXPORT grc_object_hide(grc_t *grc, const char *object_name)
{
    DIALOG *d;

    grc_errno_clear();

    if (NULL == grc) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return -1;
    }

    d = grc_get_DIALOG_from_tag(grc, object_name);

    if (NULL == d)
        return -1;

    d->flags |= D_HIDDEN;

    return 0;
}

int LIBEXPORT grc_object_show(grc_t *grc, const char *object_name)
{
    DIALOG *d;

    grc_errno_clear();

    if (NULL == grc) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return -1;
    }

    d = grc_get_DIALOG_from_tag(grc, object_name);

    if (NULL == d)
        return -1;

    d->flags &= ~D_HIDDEN;

    return 0;
}

int LIBEXPORT grc_log(grc_t *grc, const char *object_name,
    const char *msg, const char *color)
{
    DIALOG *d;

    grc_errno_clear();

    if (NULL == grc) {
        grc_set_errno(GRC_ERROR_NULL_ARG);
        return -1;
    }

    d = grc_get_DIALOG_from_tag(grc, object_name);

    if (NULL == d)
        return -1;

    /* Sets internal message */
    gui_messages_set(d->d1, d->fg, msg, color);

    /* Notify the object about it */
    object_message(d, MSG_NEW_LOG_TEXT, 0);

    return 0;
}

int LIBEXPORT grc_list_get_selected_index(grc_t *grc,
    const char *object_name)
{
    int p = -1;

    if ((grc_object_get_data(grc, object_name, GRC_MEMBER_LIST_POSITION,
                             &p) == NULL) &&
        (grc_get_last_error() != GRC_NO_ERROR))
    {
        return -1;
    }

    return p;
}

int LIBEXPORT grc_checkbox_get_status(grc_t *grc,
    const char *object_name)
{
    int st = -1;

    if ((grc_object_get_data(grc, object_name, GRC_MEMBER_CHECKBOX_STATE,
                             &st) == NULL) &&
        (grc_get_last_error() != GRC_NO_ERROR))
    {
        return -1;
    }

    return st;
}

int LIBEXPORT grc_radio_get_status(grc_t *grc,
    const char *object_name)
{
    int st = -1;

    if ((grc_object_get_data(grc, object_name, GRC_MEMBER_RADIO_STATE,
                             &st) == NULL) &&
        (grc_get_last_error() != GRC_NO_ERROR))
    {
        return -1;
    }

    return st;
}

char LIBEXPORT *grc_edit_get_data(grc_t *grc,
    const char *object_name)
{
    char *v = NULL;

    v = (char *)grc_object_get_data(grc, object_name, GRC_MEMBER_EDIT_VALUE,
                                    NULL);

    return v;
}

/*int grc_set_bitmap_mem(grc_t *grc, const char *object_name,
    unsigned char *bimg, unsigned int bsize, int image_fmt)
{
//    object_message(d, MSG_LOAD_IMAGE, c);
    return -1;
}

int grc_set_bitmap(grc_t *grc, const char *object_name,
    const char *filename, int image_fmt)
{
    return -1;
}*/

