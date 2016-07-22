
/*
 * Description: Translate info loaded from an UI file to Allegro
 *              calls.
 *
 * Author: Rodrigo Freitas
 * Created at: Mon Dec 15 11:25:28 2014
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

#include <ctype.h>

#include "libalex.h"
#include "gui/objects.h"

/* Altura default de um edit */
#define DEFAULT_EDIT_HEIGHT                 15

/* Espacador default de um botao */
#define BUTTON_DEFAULT_SPACER               4

/* Altura default de um botao */
#define DEFAULT_BUTTON_HEIGHT               30

/* Espacador default de um checkbox */
#define CHECKBOX_DEFAULT_SPACER             13

/* Altura default de um checkbox */
#define DEFAULT_CHECKBOX_HEIGHT             15

/* Espacador default de um radio button */
#define RADIO_DEFAULT_SPACER                13

/* Altura default de um radio button */
#define DEFAULT_RADIO_HEIGHT                15

struct rgb {
    int r;
    int g;
    int b;
};

struct color {
    char        name[128];
    struct rgb  rgb;
};

static struct color __colors[] = {
    { AL_BLACK,             { 0,    0,      0   }},
    { AL_WHITE,             { 255,  255,    255 }},
    { AL_LIGHT_RED,         { 255,  0,      0   }},
    { AL_LIGHT_GREEN,       { 0,    255,    0   }},
    { AL_LIGHT_BLUE,        { 0,    0,      255 }},
    { AL_YELLOW,            { 240,  240,    0   }},
    { AL_CYAN,              { 0,    255,    255 }},
    { AL_MAGENTA,           { 255,  0,      255 }},
    { AL_SILVER,            { 192,  192,    192 }},
    { AL_GRAY,              { 128,  128,    128 }},
    { AL_MAROON,            { 128,  0,      0   }},
    { AL_OLIVE,             { 128,  128,    0   }},
    { AL_PURPLE,            { 128,  0,      128 }},
    { AL_TEAL,              { 0,    128,    128 }},
    { AL_NAVY,              { 0,    0,      128 }},
    { AL_BLUE,              { 0,    0,      150 }},
    { AL_GREEN,             { 0,    150,    0   }},
    { AL_RED,               { 150,  0,      0   }},
    { AL_ORANGE_RED,        { 255,  69,     0   }},
    { AL_ORANGE,            { 255,  165,    0   }},
    { AL_KHAKI,             { 240,  230,    140 }},
    { AL_BROWN,             { 165,  42,     42  }},
    { AL_SADDLE_BROWN,      { 139,  69,     19  }},
    { AL_BAKERS_CHOCOLATE,  { 92,   51,     23  }},
    { AL_IVORY,             { 255,  255,    240 }},
    { AL_SNOW,              { 255,  250,    250 }},
    { AL_GHOST_WHITE,       { 248,  248,    255 }},
    { AL_INDIGO,            { 75,   0,      130 }},
    { AL_DARK_SLATE_BLUE,   { 72,   61,     139 }},
    { AL_SKY_BLUE,          { 135,  206,    235 }},
    { AL_SEA_GREEN,         { 46,   139,    87  }},
    { AL_DARK_GREEN,        { 0,    100,    0   }},
    { AL_FOREST_GREEN,      { 34,   139,    24  }},
    { AL_GOLD,              { 255,  215,    0   }},
    { AL_GOLDEN_ROD,        { 218,  165,    32  }},
    { AL_SALMON,            { 250,  128,    114 }},
    { AL_CORAL,             { 255,  127,    80  }},
    { AL_TOMATO,            { 255,  99,     71  }},
    { AL_DARK_SEA_GREEN,    { 143,  188,    143 }},
    { AL_DARK_CYAN,         { 0,    139,    139 }},
    { AL_TURQUOISE,         { 64,   224,    208 }},
    { AL_DARK_TURQUOISE,    { 0,    206,    209 }},
    { AL_CADET_BLUE,        { 95,   158,    160 }},
    { AL_OLIVE_DRAB,        { 107,  142,    35  }},
    { AL_STEEL_BLUE,        { 70,   130,    180 }},
    { AL_DODGER_BLUE,       { 30,   144,    255 }},
    { AL_MIDNIGHT_BLUE,     { 25,   25,     112 }},
    { AL_SLATE_BLUE,        { 106,  90,     205 }},
    { AL_SIENNA,            { 160,  82,     45  }},
    { AL_PERU,              { 205,  133,    63  }},
    { AL_TAN,               { 210,  180,    140 }},
    { AL_SLATE_GRAY,        { 112,  128,    144 }},
    { AL_DARK_KHAKI,        { 189,  183,    107 }},
    { AL_INDIAN_RED,        { 205,  92,     92  }}
};

#define MAX_COLORS              \
    (sizeof(__colors) / sizeof(__colors[0]))

/*
 * Objetos default num DIALOG:
 *
 *  d_clear_proc
 *  d_yield_proc
 */
#define DLG_DEFAULT_NOBJS           2

/*
 * ------- Funcoes diversas -------
 */

static int special_keys(const char *key)
{
    int ret = KEY_F1 - 1, k = 0;
    char *tmp;

    if (!strcmp(key, "ESC"))
        return KEY_ESC;

    if (key[0] == 'F') {
        tmp = (char *)&key[1];
        k = strtol(tmp, NULL, 10);
        return ret + k;
    }

    return -1;
}

static int key_char(const char *key)
{
    int c = key[0];

    if (isalpha(c))
        return c - '@';

    if (isdigit(c))
        return c - 21;

    return -1;
}

/*
 * Converte uma string indicando uma tecla, desde que ela esteja no formato
 * 'KEY_X', para seu correspondente scancode.
 */
static int grc_tr_str_key_to_al_key(const char *skey)
{
    cstring_t *tmp = NULL, *p = NULL;
    cstring_list_t *list = NULL;
    int ret = -1;

    tmp = cstring_create("%s", skey);

    if (NULL == tmp)
        return -1;

    list = cstring_split(tmp, "_");

    if (NULL == list)
        goto end_block;

    if (cstring_list_size(list) != 2)
        goto end_block;

    /* ok */
    p = cstring_list_get(list, 1);

    if (cstring_length(p) == 1)
        ret = key_char(cstring_valueof(p));
    else
        ret = special_keys(cstring_valueof(p));

    cstring_unref(p);

end_block:
    if (tmp != NULL)
        cstring_unref(tmp);

    if (list != NULL)
        cstring_list_destroy(list);

    return ret;
}

/*
 * Converte uma string numa cor. Tratando corretamente a quantidade de cores
 * do ambiente grafico na hora de converter.
 */
int grc_tr_color_to_al_color(int color_depth, const char *color)
{
    struct color *c = NULL;
    unsigned int i;

    for (i = 0; i < MAX_COLORS; i++)
        if (!strcmp(__colors[i].name, color)) {
            c = &__colors[i];
            break;
        }

    if (NULL == c)
        return -1;

    return makecol_depth(color_depth, c->rgb.r, c->rgb.g, c->rgb.b);
}

static cjson_t *grc_get_object(struct al_grc *grc, const char *object)
{
    return cjson_get_object_item(grc->jgrc, object);
}

static int search_object_ref(void *a, void *b)
{
    struct dlg_obj_ref *r = (struct dlg_obj_ref *)a;
    char *name = (char *)b;

    if (!strcmp(r->name, name))
        return 1;

    return 0;
}

/*
 * Procura por um objeto atraves de seu "name" dentro da lista de referencia de
 * objetos e retorna um ponteiro para sua estrutura DIALOG.
 */
DIALOG *get_DIALOG_from_grc(struct al_grc *grc, const char *object_name)
{
    struct dlg_obj_ref *ref = NULL;

    if (NULL == object_name) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return NULL;
    }

    ref = cdll_map(grc->ref, search_object_ref, (void *)object_name);

    if (NULL == ref) {
        al_set_errno(AL_ERROR_OBJECT_NOT_FOUND);
        return NULL;
    }

    return &grc->dlg[ref->dlg_index];
}

static MENU *search_menu(MENU *menu, int t_items, const char *object_name)
{
    int i;

    for (i = 0; i < t_items; i++) {
        if (menu[i].dp == NULL)
            continue;

        if (!strcmp(menu[i].dp, object_name))
            return &menu[i];
    }

    return NULL;
}

MENU *get_MENU_from_grc(struct al_grc *grc, const char *object_name)
{
    struct al_menu *p = NULL;
    MENU *it = NULL;

    if (NULL == object_name) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return NULL;
    }

    /* Procura no menu principal */
    it = search_menu(grc->dlg_menu, grc->dlg_menu_t_items, object_name);

    if (it != NULL)
        return it;

    /* Procura nos outros menus */
    for (p = grc->menu; p; p = p->next) {
        it = search_menu(p->menu, p->t_items, object_name);

        if (it != NULL)
            return it;
    }

    if (NULL == it) {
        al_set_errno(AL_ERROR_OBJECT_NOT_FOUND);
        return NULL;
    }

    return NULL;
}

/*
 * ------- Funcoes de manipulacao da interface grafica -------
 */

static int real_change_resolution(struct al_grc *grc)
{
    if (install_allegro(SYSTEM_AUTODETECT, NULL, NULL)) {
        al_set_errno(AL_ERROR_LIB_INIT);
        return -1;
    }

    if (install_keyboard()) {
        al_set_errno(AL_ERROR_KEYBOARD_INIT);
        return -1;
    }

    install_timer();
    set_color_depth(grc->gfx.color_depth);

    if (set_gfx_mode(GFX_XWINDOWS, grc->gfx.width, grc->gfx.height, 0, 0) != 0) {
        if (set_gfx_mode(GFX_FBCON, grc->gfx.width, grc->gfx.height, 0, 0) != 0) {
            remove_keyboard();
            allegro_exit();
            al_set_errno(AL_ERROR_SET_GFX_MODE);
            return -1;
        }
    } else {
        if (grc->gfx.use_mouse == AL_TRUE) {
            install_mouse();
            gui_mouse_focus = FALSE;
        }
    }

    /* Desabilita ctrl+alt+end */
    if (grc->gfx.block_keys == AL_TRUE)
        three_finger_flag = FALSE;

    return 0;
}

static int load_resolution_info(struct al_grc *grc)
{
    cjson_t *jinfo;
    struct grc_json_key *e;

    jinfo = grc_get_object(grc, OBJ_INFO);

    if (NULL == jinfo) {
        al_set_errno(AL_ERROR_INFO_BLOCK_NOT_FOUND);
        return -1;
    }

    /* width */
    e = get_grc_json_key(AL_GRC_JOBJ_WIDTH);

    if (NULL == e)
        goto unknown_grc_key_block;

    grc->gfx.width = grc_get_object_value(jinfo, e->name, AL_DEFAULT_WIDTH);

    /* height */
    e = get_grc_json_key(AL_GRC_JOBJ_HEIGHT);

    if (NULL == e)
        goto unknown_grc_key_block;

    grc->gfx.height = grc_get_object_value(jinfo, e->name, AL_DEFAULT_HEIGHT);

    /* color depth */
    e = get_grc_json_key(AL_GRC_JOBJ_COLOR_DEPTH);

    if (NULL == e)
        goto unknown_grc_key_block;

    grc->gfx.color_depth = grc_get_object_value(jinfo, e->name,
                                                AL_DEFAULT_COLOR_DEPTH);

    /* block exit keys */
    e = get_grc_json_key(AL_GRC_JOBJ_BLOCK_EXIT_KEYS);

    if (NULL == e)
        goto unknown_grc_key_block;

    grc->gfx.block_keys = grc_get_object_value(jinfo, e->name, AL_TRUE);

    /* mouse */
    e = get_grc_json_key(AL_GRC_JOBJ_MOUSE);

    if (NULL == e)
        goto unknown_grc_key_block;

    grc->gfx.use_mouse = grc_get_object_value(jinfo, e->name, AL_FALSE);

    /* ignore_esc_key */
    e = get_grc_json_key(AL_GRC_JOBJ_IGNORE_ESC_KEY);

    if (NULL == e)
        goto unknown_grc_key_block;

    grc->ignore_esc_key = grc_get_object_value(jinfo, e->name, AL_FALSE);

    return 0;

unknown_grc_key_block:
    al_set_errno(AL_ERROR_UNDEFINED_GRC_KEY);
    return -1;
}

/*
 * Carrega as informacoes principais de um arquivo GRC relacionadas ao modo
 * grafico do DIALOG e ajusta o ambiente para permitir a execucao de aplicacoes
 * com interfaces graficas.
 */
int gui_change_resolution(struct al_grc *grc)
{
    if (load_resolution_info(grc) < 0)
        return -1;

    return real_change_resolution(grc);
}

/*
 * Carrega as informacoes das cores principais do DIALOG que sera inicializado.
 */
int gui_load_colors(struct al_grc *grc)
{
    cjson_t *jcolors;
    struct grc_json_key *e;
    char *fg, *bg;

    jcolors = grc_get_object(grc, OBJ_COLORS);

    if (NULL == jcolors) {
        al_set_errno(AL_ERROR_COLORS_BLOCK_NOT_FOUND);
        return -1;
    }

    /* foreground */
    e = get_grc_json_key(AL_GRC_JOBJ_FOREGROUND);

    if (NULL == e)
        goto unknown_grc_key_block;

    fg = grc_get_object_str(jcolors, e->name);

    /* background */
    e = get_grc_json_key(AL_GRC_JOBJ_BACKGROUND);

    if (NULL == e)
        goto unknown_grc_key_block;

    bg = grc_get_object_str(jcolors, e->name);

    /* Inicializa cores globais */
    grc->fg = grc_tr_color_to_al_color(get_color_depth(), fg);
    grc->bg = grc_tr_color_to_al_color(get_color_depth(), bg);

    return 0;

unknown_grc_key_block:
    al_set_errno(AL_ERROR_UNDEFINED_GRC_KEY);
    return -1;
}

/*
 * Volta a resolucao da tela para o modo texto padrao.
 */
void gui_reset_resolution(void)
{
    cmsleep(100);
    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
    remove_keyboard();
    allegro_exit();
}

/*
 * ------- Funcoes de manipulacao do DIALOG -------
 */

static void DIALOG_creation_start(struct al_grc *grc)
{
    DIALOG *d = &grc->dlg[0];

    d->proc = d_clear_proc;
}

static void DIALOG_creation_finish(struct al_grc *grc, int dlg_size)
{
    DIALOG *d = &grc->dlg[dlg_size];

    d->proc = d_yield_proc;
}

static void set_object_internal_data(DIALOG *d, struct al_grc *grc)
{
    struct al_callback_data *acd;

    acd = new_callback_data();

    if (NULL == acd) {
        al_set_errno(AL_ERROR_MEMORY);
        return;
    }

    acd->grc = (void *)grc;
    d->dp3 = acd;

    grc->callback_data = cdll_unshift(grc->callback_data, acd);
}

static int add_object(struct al_grc *grc, int dlg_index, cjson_t *object)
{
    struct grc_obj_properties *prop;
    DIALOG *d = &grc->dlg[dlg_index], *p;
    struct dlg_obj_ref *ref = NULL;
    struct grc_generic_data *g_data;
    int w=-1, h=-1;
    char tmp[MAX_EDIT_SIZE]={0};

    prop = new_obj_properties(object);

    if (NULL == prop)
        return -1;

    /*
     * Objetos que o usuario precisa definir largura (width) e altura (height):
     *
     *  listbox
     *  box
     *  slider
     */

    /*
     * Adiciona objeto no DIALOG. Recomenda-se que as caixas 'box' sejam os
     * primeiros objetos definidos, para que nenhum tipo de texto seja
     * sobreposto indevidamente.
     */
    switch (prop->type) {
        case AL_GRC_OBJ_BOX:
            d->proc = d_box_proc;
            break;

        case AL_GRC_OBJ_DIGITAL_CLOCK:
            d->proc = gui_clock_proc;
            d->dp = grc->dlg_clock_str;
            break;

        case AL_GRC_OBJ_IMAGE:
        case AL_GRC_OBJ_MESSAGES_LOG_BOX:
            if (prop->type == AL_GRC_OBJ_MESSAGES_LOG_BOX) {
                d->proc = gui_messages_log_proc;
                d->d1 = prop->line_break_mode;
            } else {
                d->proc = gui_d_bitmap_proc;

                /*
                 * Inicializa como NULL para nao dar erro quando passar pelo
                 * objeto e nao possuir imagem para exibir.
                 */
                d->dp = NULL;
            }

            /*
             * Se possuir uma referencia para um objeto "pai", sua posicao sera
             * relativa a ele. Vale a pena ressaltar que este objeto "pai" deve
             * ter sido definido __ANTES__ deste, se nao, nao tem como obter sua
             * posicao ;-). Caso contrario, o usuario precisa defini-la.
             */
            if (prop->parent != NULL) {
                p = get_DIALOG_from_grc(grc, prop->parent);
                d->x = p->x + 2;
                d->y = p->y + 2;
                d->w = p->w - 2;
                d->h = p->h - 2;
            }

            break;

        case AL_GRC_OBJ_CUSTOM:
            /*
             * Neste objeto, tanto a funcao de proprio objeto quanto a funcao
             * de callback, devem ser definidas em tempo de execucao, antes
             * da execucao do DIALOG.
             *
             * d->proc -> Funcao do objeto
             * d->dp3  -> callback
             */
            break;

        case AL_GRC_OBJ_VAR_TEXT:
        case AL_GRC_OBJ_FIXED_TEXT:
            d->proc = d_text_proc;

            if (prop->type == AL_GRC_OBJ_FIXED_TEXT) {
                /*
                 * Apesar da estrutura 'prop' ser desalocada logo abaixo, este
                 * texto somente aponta para o texto original carregado do arquivo
                 * GRC, desta forma o texto consegue ser exibido.
                 */
                d->dp = (char *)prop->text;
            }

            break;

        case AL_GRC_OBJ_BUTTON:
            d->proc = gui_d_button_proc;
            d->flags = D_EXIT;
            d->dp = (char *)prop->text;

            /* Calcula largura e altura automaticamente, caso necessario */
            if (prop->w < 0)
                w = text_length(font, prop->text) + BUTTON_DEFAULT_SPACER;

            if (prop->h < 0)
                h = DEFAULT_BUTTON_HEIGHT;

            break;

        case AL_GRC_OBJ_EDIT:
            if (prop->data_length >= MAX_EDIT_SIZE) {
                al_set_errno(AL_ERROR_UNSUPPORTED_EDIT_INPUT_LENGTH);
                return -1;
            }

            if (prop->password_mode == AL_FALSE)
                d->proc = gui_d_edit_proc;
            else {
                d->proc = gui_d_password_proc;

                /*
                 * Cria um buffer temporario para armazenar a string contendo
                 * os asteriscos.
                 */
                g_data = new_grc_generic_data();
                grc->g_data = cdll_unshift(grc->g_data, g_data);
                d->dp2 = g_data->data;
            }

            /* Cria o buffer para armazenar a string digitada no objeto. */
            g_data = new_grc_generic_data();
            grc->g_data = cdll_unshift(grc->g_data, g_data);
            d->dp = g_data->data;

            d->flags = D_EXIT;
            d->d1 = prop->data_length;

            /*
             * Ajusta um tamanho default para permitir que o mouse possa
             * atrair o foco para este objeto.
             */
            h = DEFAULT_EDIT_HEIGHT;

            /* Calcula a largura automaticamente, caso necessario */
            if (prop->parent != NULL) {
                p = get_DIALOG_from_grc(grc, prop->parent);
                d->x = p->x + 3;
                d->y = p->y + 3;
                d->w = p->w - 4;
                d->h = h;
            } else {
                if (prop->w < 0) {
                    memset(tmp, '0', prop->data_length + 1);
                    w = text_length(font, tmp);
                }
            }

            break;

        case AL_GRC_OBJ_LIST:
            d->proc = gui_d_list_proc;
            d->flags = D_EXIT;
            break;

        case AL_GRC_OBJ_CHECK:
            d->proc = gui_d_check_proc;
            d->dp = (char *)prop->text;

            if (prop->horizontal_position == AL_GRC_H_POS_RIGHT)
                d->d1 = 1;
            else
                d->d1 = 0;

            /* Calcula largura e altura automaticamente, caso necessario */
            if (prop->w < 0)
                w = text_length(font, prop->text) + CHECKBOX_DEFAULT_SPACER;

            if (prop->h < 0)
                h = DEFAULT_CHECKBOX_HEIGHT;

            break;

        case AL_GRC_OBJ_RADIO:
            d->proc = gui_d_radio_proc;
            d->dp = (char *)prop->text;
            d->d1 = prop->radio_group;
            d->d2 = prop->radio_type;

            /* Calcula largura e altura automaticamente, caso necessario */
            if (prop->w < 0)
                w = text_length(font, prop->text) + RADIO_DEFAULT_SPACER;

            if (prop->h < 0)
                h = DEFAULT_RADIO_HEIGHT;

            break;

        case AL_GRC_OBJ_SLIDER:
            d->proc = gui_d_slider_proc;
            d->d1 = prop->data_length;
            break;

        /* Not implemented yet */
/*        case AL_GRC_OBJ_LIVE_IMAGE:
        case AL_GRC_OBJ_MULTLIVE_IMAGE:
            if (prop->type == AL_GRC_OBJ_LIVE_IMAGE)
                d->proc = gui_d_live_image;
            else {
                d->proc = gui_d_multlive_image;
                d->d2 = prop->devices;
            }

            d->d1 = prop->fps;

            if (prop->parent != NULL) {
                p = get_DIALOG_from_grc(grc, prop->parent);
                d->x = p->x + 2;
                d->y = p->y + 2;
                d->w = p->w - 4;
                d->h = p->h - 4;
            }

            break;*/

        case AL_GRC_OBJ_VT_KEYBOARD:
            d->proc = gui_d_vt_keyboard_proc;
            d->d1 = GRC_KLAYOUT_LETTERS;
            d->dp = grc;
            grc->virtual_keyboard = AL_TRUE;
            break;

        case AL_GRC_OBJ_ICON:
            d->proc = gui_d_icon_proc;
            d->flags = D_EXIT;
            break;

        case AL_GRC_OBJ_TEXTBOX:
            d->proc = d_textbox_proc;
            break;

        default:
            al_set_errno(AL_ERROR_UNKNOWN_OBJECT_TYPE);
            return -1;
    }

    /* Posicao do objeto na tela */
    if (prop->parent == NULL) {
        d->x = prop->x;
        d->y = prop->y;
        d->w = (w == -1) ? prop->w : w;
        d->h = (h == -1) ? prop->h : h;
    }

    /* Define cores (especificas) do objeto */
    if (prop->fg != NULL)
        d->fg = grc_tr_color_to_al_color(get_color_depth(), prop->fg);
    else
        d->fg = grc->fg;

    d->bg = grc->bg;

    /* Mostra ou nao o objeto */
    if (prop->hide == AL_FALSE)
        d->flags &= ~D_HIDDEN;
    else
        d->flags |= D_HIDDEN;

    /*
     * Todo objeto ja possui acesso a informacoes internas da biblioteca,
     * mesmo que eles nao possuam funcoes de callback.
     */
    set_object_internal_data(d, grc);

    /* Cria referencia para utilizacao posterior pelo usuario */
    if (prop->name != NULL) {
        ref = new_obj_ref(prop->name, dlg_index, prop->type);

        if (NULL == ref)
            return -1;

        grc->ref = cdll_unshift(grc->ref, ref);
    }

    destroy_obj_properties(prop);

    return 0;
}

/*
 * Percorre o array de objetos que constituirao o DIALOG definido no arquivo
 * GRC. A funcao retorna o ultimo indice do ultimo objeto inserido no DIALOG
 * para permitir que as teclas adicionadas posteriormente estejam na sequencia,
 * em memoria.
 */
static int DIALOG_add_objects(struct al_grc *grc, cjson_t *objects,
    int dlg_index)
{
    int i, t = 0, p;
    cjson_t *o;

    t = cjson_get_array_size(objects);

    if (t <= 0) {
        al_set_errno(AL_ERROR_NO_OBJECTS);
        return -1;
    }

    for (i = 0, p = dlg_index; i < t; i++, p++) {
        o = cjson_get_array_item(objects, i);

        if (add_object(grc, p, o) < 0)
            return -1;
    }

    return p;
}

static int add_key(struct al_grc *grc, int dlg_index, cjson_t *key)
{
    DIALOG *d = &grc->dlg[dlg_index];
    struct dlg_obj_ref *ref = NULL;
    struct grc_key_data *kdata;

    kdata = new_key_data(key);

    if (NULL == kdata)
        return -1;

    /* Adiciona objeto no DIALOG */
    d->proc = gui_d_keyboard_proc;
    d->d1 = grc_tr_str_key_to_al_key(kdata->key);

    /*
     * Caso esteja adicionando a tecla ESC, deixa avisado internamente,
     * para nao sobrepo-la.
     */
    if (d->d1 == KEY_ESC)
        grc->esc_key_user_defined = AL_TRUE;

    /* Cria referencia para utilizacao posterior */
    ref = new_obj_ref(kdata->name, dlg_index, AL_GRC_OBJ_KEY);

    if (NULL == ref)
        return -1;

    grc->ref = cdll_unshift(grc->ref, ref);
    destroy_key_data(kdata);

    return 0;
}

/*
 * Funcao de callback simples para bloquear a tecla ESC para o usuario.
 */
static int __disable_key(void *arg __attribute__((unused)))
{
    return D_O_K;
}

static void add_default_esc_key(struct al_grc *grc, int dlg_index)
{
    DIALOG *d = &grc->dlg[dlg_index];

    /* A tecla foi definida pelo usuario? */
    if (grc->esc_key_user_defined == AL_TRUE)
        return;

    /* O usuario pediu para ignorar ESC default da Allegro? */
    if (grc->ignore_esc_key == AL_FALSE)
        return;

    /*
     * Utiliza o objeto de teclas da Allegro para evitar a criacao de uma
     * estrutura 'al_callback_data' desnecessaria.
     */
    d->proc = d_keyboard_proc;
    d->d1 = KEY_ESC;
    d->dp = __disable_key;
}

/*
 * Percorre o array de objetos de teclas definidos no arquivo GRC e faz a
 * sua insercao no DIALOG.
 */
static int DIALOG_add_keys(struct al_grc *grc, cjson_t *keys, int dlg_index)
{
    int i, t;
    cjson_t *n;

    t = cjson_get_array_size(keys);

    if (t <= 0) {
        al_set_errno(AL_ERROR_NO_KEYS);
        return -1;
    }

    grc->esc_key_user_defined = AL_FALSE;

    for (i = 0; i < t; i++) {
        n = cjson_get_array_item(keys, i);

        if (add_key(grc, dlg_index + i, n) < 0)
            return -1;
    }

    return 0;
}

static int cvt_grc_menu(void *a, void *b)
{
    struct grc_menu *menu = (struct grc_menu *)a, *p;
    struct al_grc *grc = (struct al_grc *)b;
    int t_items = 0, i;
    MENU *m;
    struct al_menu *al_menu;

    if (menu->items != NULL)
        t_items = cdll_size(menu->items);

    m = calloc(t_items + 1, sizeof(MENU));

    if (NULL == m)
        return -1;

    for (i = 0, p = menu->items; p; p = p->next, i++) {
        if (strcmp(p->text, MENU_SEPARATOR)) {
            m[i].text = p->text;
            m[i].dp = p->name;
        } else
            m[i].text = "";
    }

    al_menu = calloc(1, sizeof(struct al_menu));
    al_menu->name = menu->name;
    al_menu->parent = menu->parent;
    al_menu->menu = m;
    al_menu->t_items = t_items;

    grc->menu = cdll_unshift(grc->menu, al_menu);

    return 0;
}

static void grc_menu_to_MENU(struct al_grc *grc)
{
    cdll_map(grc->grc_menu, cvt_grc_menu, grc);
}

static int create_main_MENU(struct al_grc *grc, cjson_t *main_options)
{
    cjson_t *n;
    int i, t;
    char *tmp;
    struct grc_json_key *e;

    /* Cria menu principal */
    t = cjson_get_array_size(main_options);
    grc->dlg_menu = calloc(t + 1, sizeof(MENU));

    for (i = 0; i < t; i++) {
        n = cjson_get_array_item(main_options, i);

        /* text */
        e = get_grc_json_key(AL_GRC_JOBJ_TEXT);

        if (NULL == e)
            return -1;

        tmp = grc_get_object_str(n, e->name);
        grc->dlg_menu[i].text = tmp;

        /* name */
        e = get_grc_json_key(AL_GRC_JOBJ_NAME);

        if (NULL == e)
            return -1;

        tmp = grc_get_object_str(n, e->name);
        grc->dlg_menu[i].dp = tmp;
    }

    return t;
}

static int search_child(void *a, void *b)
{
    struct al_menu *al_menu = (struct al_menu *)a;
    char *name = (char *)b;

    if (!strcmp(al_menu->name, name))
        return 1;

    return 0;
}

static MENU *search_child_MENU(struct al_grc *grc, const char *child_name)
{
    struct al_menu *al_menu;

    al_menu = cdll_map(grc->menu, search_child, (void *)child_name);

    if (NULL == al_menu)
        return NULL;

    return al_menu->menu;
}

static MENU *search_MENU_item(struct al_grc *grc, const char *item_name)
{
    struct al_menu *p;
    int i;

    for (p = grc->menu; p; p = p->next)
        for (i = 0; i < p->t_items; i++)
            if (p->menu[i].dp != NULL)
                if (!strcmp(p->menu[i].dp, item_name))
                    return &p->menu[i];

    return NULL;
}

/*
 * Faz o ajuste de alguns ponteiros nos itens dos menus para que apontem
 * corretamente para seus submenus (quando necessario).
 */
static int adjust_submenus(void *a, void *b)
{
    struct al_menu *al_menu = (struct al_menu *)a;
    struct al_grc *grc = (struct al_grc *)b;
    MENU *it;

    if (al_menu->parent == NULL)
        return 0;

    /* Procura pelo item e aponta-o para este menu */
    it = search_MENU_item(grc, al_menu->parent);

    if (it != NULL)
        it->child = al_menu->menu;

    return 0;
}

/*
 * Cria a estrutura do tipo MENU que sera utilizada pelo objeto
 * 'd_menu_proc' posteriormente e adiciona todos os itens e submenus
 * definidos pelo usuario.
 */
static void create_MENU(struct al_grc *grc, cjson_t *main_options)
{
    int i;

    grc->dlg_menu_t_items = create_main_MENU(grc, main_options);

    /* Ajusta menus principal */
    for (i = 0; i < grc->dlg_menu_t_items; i++) {
        if (grc->dlg_menu[i].text == NULL)
            continue;

        if (grc->dlg_menu[i].dp != NULL)
            grc->dlg_menu[i].child = search_child_MENU(grc, grc->dlg_menu[i].dp);
    }

    /* Ajusta submenus */
    cdll_map(grc->menu, adjust_submenus, grc);
}

/*
 * Adiciona um objeto do tipo 'd_menu_proc' na lista de objetos do DIALOG
 * que esta sendo criado. Como nao existe nenhuma configuracao (ainda) sua
 * posicao na tela e fixa, no canto superior direito.
 */
static void add_menu(struct al_grc *grc, int dlg_index)
{
    DIALOG *d = &grc->dlg[dlg_index];

    d->proc = d_menu_proc;
    d->dp = grc->dlg_menu;

    /* Posicoes x e y fixas para o usuario (por enquanto) */
    d->x = 0;
    d->y = 0;
}

/*
 * Todos items de menus definidos no arquivo GRC sao armazenados em
 * estruturas do tipo 'struct grc_menu'. Elas que serao "manipuladas" pelo
 * usuario, quando necessario, como no caso de ajustar a funcao de 'callback'
 * de um item.
 *
 * A lista de estruturas do tipo 'struct al_menu' e utilizada para guardar
 * os menus agrupados numa estrutura entendida pela Allegro, a estrutura MENU.
 * E servir como origem para a montagem do menu que sera utilizado pelo objeto
 * 'd_menu_proc' do DIALOG.
 */
static void DIALOG_add_menu(struct al_grc *grc, cjson_t *menu, int dlg_index)
{
    cjson_t *menu_options = NULL, *main_options = NULL, *n;
    int i, t;
    struct grc_menu *grcm;

    menu_options = cjson_get_object_item(menu, OBJ_MENU_OPTIONS);
    main_options = cjson_get_object_item(menu, OBJ_MAIN_OPTIONS);

    /* Extrai as informacoes de menus e itens definidos */
    if ((NULL == menu_options) || (NULL == main_options))
        return;

    t = cjson_get_array_size(menu_options);

    for (i = 0; i < t; i++) {
        n = cjson_get_array_item(menu_options, i);

        grcm = new_menu(n);
        grc->grc_menu = cdll_unshift(grc->grc_menu, grcm);
    }

    /* Converte menus GRC para menus MENU (Allegro) */
    grc_menu_to_MENU(grc);

    /* Realiza a "montagem" do(s) menu(s) no formato suportado pela Allegro */
    create_MENU(grc, main_options);

    /* Adiciona um objeto 'd_menu_proc' no DIALOG que esta sendo criado */
    add_menu(grc, dlg_index);
}

int create_DIALOG(struct al_grc *grc)
{
    cjson_t *objects, *keys = NULL, *menu = NULL;
    int total = 0, p = 0;

    objects = grc_get_object(grc, OBJ_OBJECTS);

    if (NULL == objects) {
        al_set_errno(AL_ERROR_OBJECTS_BLOCK_NOT_FOUND);
        return -1;
    }

    keys = grc_get_object(grc, OBJ_KEYS);
    menu = grc_get_object(grc, OBJ_MENU);

    /*
     * O tamanho total do DIALOG sera a quantidade de teclas + a quantidade de
     * objetos + menu + tecla ESC ignorada + d_yield_proc + d_clear_proc + 1.
     */
    total = cjson_get_array_size(objects);
    total += 1; /* d_yield_proc */

    if (grc->use_gfx == AL_TRUE)
        total += 1; /* d_clear_proc */

    if (keys != NULL)
        total += cjson_get_array_size(keys);

    /*
     * Caso um menu tenha sido definido, ele tambem precisara de espaco no
     * DIALOG.
     */
    if (menu != NULL)
        total += 1;

    /*
     * Mesmo que o usuario tenha definido a tecla ESC, aloca espaco para mais
     * um objeto, pois neste caso ele nao sera utilizado.
     */
    if (grc->ignore_esc_key)
        total += 1;

    grc->dlg = calloc(total + 1, sizeof(DIALOG));

    if (NULL == grc->dlg) {
        al_set_errno(AL_ERROR_MEMORY);
        return -1;
    }

    /* Inicializa DIALOG */
    if (grc->use_gfx == AL_TRUE)
        DIALOG_creation_start(grc);

    /* Adiciona objetos definidos no arquivo GRC */
    p = DIALOG_add_objects(grc, objects, (grc->use_gfx == AL_TRUE) ? 1 : 0);

    if (p < 0)
        return -1;

    /* Adiciona um (possivel) menu no DIALOG */
    if (menu != NULL) {
        DIALOG_add_menu(grc, menu, p);
        p += 1;

        /*
         * Muda as cores principais do ambiente grafico para que o menu tambem
         * utilize as mesmas definidas para os objetos.
         */
        gui_fg_color = grc->fg;
        gui_bg_color = grc->bg;
    }

    /* Adiciona teclas (tambem) definidas no arquivo GRC */
    if (keys != NULL)
        if (DIALOG_add_keys(grc, keys, p) < 0)
            return -1;

    /* Ignora tecla ESC (se necessario) */
    add_default_esc_key(grc, total - 2);

    /* Finaliza o DIALOG */
    DIALOG_creation_finish(grc, total - 1);

    return 0;
}

void run_DIALOG(struct al_grc *grc)
{
    if (grc->use_gfx == AL_FALSE)
        centre_dialog(grc->dlg);

    do_dialog(grc->dlg, -1);
}

int run_callback(struct al_callback_data *acd)
{
    if (acd->callback != NULL)
        return (acd->callback)(acd);

    return D_O_K;
}

