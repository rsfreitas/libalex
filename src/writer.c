
/*
 * Description: Functions to create/save an UI file.
 *
 * Author: Rodrigo Freitas
 * Created at: Mon Dec 15 22:59:58 2014
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

/* Number os colors used when outside graphic mode */
#define OUT_GFX_COLOR_DEPTH             8

static int create_root_object(struct al_grc *grc)
{
    if (NULL == grc->jgrc) {
        grc->jgrc = cjson_create_object();

        if (NULL == grc->jgrc) {
            al_set_errno(AL_ERROR_MEMORY);
            return -1;
        }
    }

    return 0;
}

/**
 * @name al_grc_GRC_create_colors
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
int LIBEXPORT al_grc_GRC_create_colors(struct al_grc *grc,
    const char *foreground, const char *background)
{
    cjson_t *fg, *bg, *c;

    al_errno_clear();

    if ((NULL == grc) || (NULL == foreground) || (NULL == background)) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    if (create_root_object(grc) < 0)
        return -1;

    /* Invalid colors? */
    if ((grc_tr_color_to_al_color(OUT_GFX_COLOR_DEPTH, foreground) < 0) ||
        (grc_tr_color_to_al_color(OUT_GFX_COLOR_DEPTH, background) < 0))
    {
        al_set_errno(AL_ERROR_UNSUPPORTED_COLORS);
        return -1;
    }

    fg = cjson_create_string(foreground);
    bg = cjson_create_string(background);
    c = cjson_create_object();

    if ((NULL == fg) || (NULL == bg) || (NULL == c)) {
        al_set_errno(AL_ERROR_MEMORY);
        return -1;
    }

    /* Create an object with the colors */
    cjson_add_item_to_object(c, OBJ_FOREGROUND, fg);
    cjson_add_item_to_object(c, OBJ_BACKGROUND, bg);

    /* Add it to the main object */
    cjson_add_item_to_object(grc->jgrc, OBJ_COLORS, c);

    return 0;
}

/**
 * @name al_grc_GRC_create_info
 * @brief Cria o objeto com as informacoes principais do DIALOG.
 *
 * @param [in,out] grc: Previously created GRC structure.
 * @param [in] width: Largura da janela.
 * @param [in] height: Altura da janela.
 * @param [in] color: Quantidade de cores da janela.
 * @param [in] block_exit_keys: Flag indicando o bloqueio ou nao das teclas
 *                              ctrl+alt+end.
 * @param [in] mouse: Flag indicando ou nao o uso do mouse.
 * @param [in] ignore_esc_key: Flag indicando ou nao se a tecla ESC sera
 *                             ignorada.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int LIBEXPORT al_grc_GRC_create_info(struct al_grc *grc, unsigned int width,
    unsigned int height, enum al_grc_color_depth color,
    bool block_exit_keys, bool mouse, bool ignore_esc_key)
{
    cjson_t *c, *k, *m, *e, *w, *h, *i;

    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    if (create_root_object(grc) < 0)
        return -1;

    w = cjson_create_number(width);
    h = cjson_create_number(height);
    c = cjson_create_number(color);

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
        al_set_errno(AL_ERROR_MEMORY);
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
    cjson_add_item_to_object(grc->jgrc, OBJ_INFO, i);

    return 0;
}

static int create_tmp_array(struct al_grc *grc)
{
    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    create_root_object(grc);
    grc->jtmp = cjson_create_array();

    if (NULL == grc->jtmp) {
        al_set_errno(AL_ERROR_MEMORY);
        return -1;
    }

    return 0;
}

/**
 * @name al_grc_GRC_keys_start
 * @brief Inicializa o objeto de teclas suportadas pelo DIALOG.
 *
 * @param [in,out] grc: Previously created GRC structure.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int LIBEXPORT al_grc_GRC_keys_start(struct al_grc *grc)
{
    return create_tmp_array(grc);
}

/**
 * @name al_grc_GRC_add_key
 * @brief Adiciona uma tecla no objeto principal de teclas do DIALOG.
 *
 * @param [in,out] grc: Previously created GRC structure.
 * @param [in] key: Tecla que sera suportada pelo DIALOG. Deve ser uma string
 *                  no mesmo padrao do scancode da Allegro. Exemplo: KEY_A.
 * @param [in] name: Nome de identificacao do objeto para o usuario.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int LIBEXPORT al_grc_GRC_add_key(struct al_grc *grc, const char *key,
    const char *name)
{
    cjson_t *k, *n = NULL, *p;

    al_errno_clear();

    if ((NULL == grc) || (NULL == key)) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    k = cjson_create_string(key);

    if (name != NULL)
        n = cjson_create_string(name);

    p = cjson_create_object();

    if ((NULL == k) || ((name != NULL) && (NULL == n)) || (NULL == p)) {
        al_set_errno(AL_ERROR_MEMORY);
        return -1;
    }

    /* Cria objeto */
    cjson_add_item_to_object(p, OBJ_KEY, k);

    if (n != NULL)
        cjson_add_item_to_object(p, OBJ_NAME, n);

    /* Adiciona ao array */
    cjson_add_item_to_array(grc->jtmp, p);

    return 0;
}

static int add_tmp_array(struct al_grc *grc, const char *array_name)
{
    al_errno_clear();

    if ((NULL == grc) || (NULL == array_name)) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    cjson_add_item_to_object(grc->jgrc, array_name, grc->jtmp);

    return 0;
}

/**
 * @name al_grc_GRC_keys_finish
 * @brief Finaliza o objeto de teclas suportadas pelo DIALOG.
 *
 * @param [in,out] grc: Previously created GRC structure.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int LIBEXPORT al_grc_GRC_keys_finish(struct al_grc *grc)
{
    return add_tmp_array(grc, OBJ_KEYS);
}

/**
 * @name al_grc_GRC_objects_start
 * @brief Inicializa o objeto que contera os objetos suportados pelo DIALOG.
 *
 * @param [in,out] grc: Previously created GRC structure.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int LIBEXPORT al_grc_GRC_objects_start(struct al_grc *grc)
{
    return create_tmp_array(grc);
}

/**
 * @name al_grc_GRC_create_object
 * @brief Inicializa um objeto para ser inserido no DIALOG.
 *
 * @param [in,out] grc: Previously created GRC structure.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int LIBEXPORT al_grc_GRC_create_object(struct al_grc *grc)
{
    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    grc->jtmp_obj = cjson_create_object();

    if (NULL == grc->jtmp_obj) {
        al_set_errno(AL_ERROR_MEMORY);
        return -1;
    }

    return 0;
}

/**
 * @name al_grc_GRC_finish_object
 * @brief Finaliza o objeto que sera inserido no DIALOG.
 *
 * @param [in,out] grc: Previously created GRC structure.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int LIBEXPORT al_grc_GRC_finish_object(struct al_grc *grc)
{
    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    cjson_add_item_to_array(grc->jtmp, grc->jtmp_obj);

    return 0;
}

/**
 * @name al_grc_GRC_set_object_property
 * @brief Insere uma propriedade no objeto recem criado.
 *
 * @param [in,out] grc: Previously created GRC structure.
 * @param [in] prop: Propriedade que sera inserida no objeto.
 * @param [in] ...: Valor da propriedade.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int LIBEXPORT al_grc_GRC_set_object_property(struct al_grc *grc,
    enum al_grc_object_property prop, ...)
{
    va_list ap;
    enum grc_entry_type_value grc_value;
    const char *jkey = NULL;
    cjson_t *c;
    int i;
    char *s = NULL;

    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    va_start(ap, NULL);

    switch (prop) {
        case AL_GRC_JOBJ_WIDTH:
            jkey = OBJ_WIDTH;
            i = va_arg(ap, int);
            grc_value = GRC_NUMBER;
            break;

        case AL_GRC_JOBJ_HEIGHT:
            jkey = OBJ_HEIGHT;
            i = va_arg(ap, int);
            grc_value = GRC_NUMBER;
            break;

        case AL_GRC_JOBJ_FOREGROUND:
            jkey = OBJ_FOREGROUND;
            s = va_arg(ap, char *);

            if (grc_tr_color_to_al_color(AL_DEFAULT_COLOR_DEPTH, s) < 0) {
                al_set_errno(AL_ERROR_UNSUPPORTED_COLOR_DEPTH);
                return -1;
            }

            grc_value = GRC_STRING;
            break;

        case AL_GRC_JOBJ_TYPE:
            jkey = OBJ_TYPE;
            i = va_arg(ap, int);
            grc_value = GRC_STRING;
            s = (char *)str_grc_obj_type(i);

            if (NULL == s) {
                al_set_errno(AL_ERROR_UNKNOWN_OBJECT_TYPE);
                return -1;
            }

            break;

        case AL_GRC_JOBJ_POS_X:
            jkey = OBJ_POS_X;
            i = va_arg(ap, int);
            grc_value = GRC_NUMBER;
            break;

        case AL_GRC_JOBJ_POS_Y:
            jkey = OBJ_POS_Y;
            i = va_arg(ap, int);
            grc_value = GRC_NUMBER;
            break;

        case AL_GRC_JOBJ_NAME:
            jkey = OBJ_NAME;
            s = va_arg(ap, char *);
            grc_value = GRC_STRING;
            break;

        case AL_GRC_JOBJ_PARENT:
            jkey = OBJ_PARENT;
            s = va_arg(ap, char *);
            grc_value = GRC_STRING;
            break;

        case AL_GRC_JOBJ_TEXT:
            jkey = OBJ_TEXT;
            s = va_arg(ap, char *);
            grc_value = GRC_STRING;
            break;

        case AL_GRC_JOBJ_HIDE:
            jkey = OBJ_HIDE;
            i = va_arg(ap, int);
            grc_value = GRC_BOOL;
            break;

        case AL_GRC_JOBJ_LINE_BREAK:
            jkey = OBJ_LINE_BREAK;
            i = va_arg(ap, int);
            grc_value = GRC_STRING;
            s = (char *)str_line_break(i);
            break;

        case AL_GRC_JOBJ_INPUT_LENGTH:
            jkey = OBJ_INPUT_LENGTH;
            i = va_arg(ap, int);
            grc_value = GRC_NUMBER;
            break;

        case AL_GRC_JOBJ_RADIO_GROUP:
            jkey = OBJ_RADIO_GROUP;
            i = va_arg(ap, int);
            grc_value = GRC_NUMBER;
            break;

        case AL_GRC_JOBJ_RADIO_TYPE:
            jkey = OBJ_RADIO_TYPE;
            i = va_arg(ap, int);
            grc_value = GRC_STRING;
            s = (char *)str_radio_type(i);
            break;

        case AL_GRC_JOBJ_PASSWORD_MODE:
            jkey = OBJ_PASSWORD;
            i = va_arg(ap, int);
            grc_value = GRC_BOOL;
            break;

        case AL_GRC_JOBJ_H_POSITION:
            jkey = OBJ_HORIZONTAL_POSITION;
            i = va_arg(ap, int);
            grc_value = GRC_STRING;
            s = (char *)str_horizontal_position(i);
            break;

        default:
            al_set_errno(AL_ERROR_UNKNOWN_PROPERTY);
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
            al_set_errno(AL_ERROR_MEMORY);
            return -1;
        }

        cjson_add_item_to_object(grc->jtmp_obj, jkey, c);
    }

    va_end(ap);

    return 0;
}

/**
 * @name al_grc_GRC_objects_finish
 * @brief Finaliza o objeto contendo os objetos principais suportados pelo DIALOG.
 *
 * @param [in,out] grc: Previously created GRC structure.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int LIBEXPORT al_grc_GRC_objects_finish(struct al_grc *grc)
{
    return add_tmp_array(grc, OBJ_OBJECTS);
}

