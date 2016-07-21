
/*
 * Description: Arquivo contendo somente declaracoes internas da
 *              biblioteca (nada e visivel para o usuario).
 *
 * Author: Rodrigo Freitas
 * Created at: Tue Dec  9 22:44:20 2014
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

#ifndef _LIBALEX_INTERNAL_H
#define _LIBALEX_INTERNAL_H	1

/* Tamanho maximo suportado numa string por um objeto 'edit' */
#define MAX_EDIT_SIZE               1024

/* Valores default para janela da interface grafica */
#define AL_DEFAULT_WIDTH            800
#define AL_DEFAULT_HEIGHT           600
#define AL_DEFAULT_COLOR_DEPTH      32

/* Tipos de valores de objetos em arquivos GRC */
enum grc_entry_type_value {
    GRC_NUMBER,
    GRC_STRING,
    GRC_BOOL
};

/* Objetos principais dos arquivos GRC */
#define OBJ_INFO                    "info"
#define OBJ_OBJECTS                 "objects"
#define OBJ_KEYS                    "keys"
#define OBJ_COLORS                  "colors"
#define OBJ_MENU                    "menu"
#define OBJ_MAIN_OPTIONS            "main_options"
#define OBJ_MENU_OPTIONS            "menu_options"
#define OBJ_OPTIONS                 "options"

/* Objetos secundarios dos arquivos GRC */
#define OBJ_WIDTH                   "width"
#define OBJ_HEIGHT                  "height"
#define OBJ_COLOR_DEPTH             "color_depth"
#define OBJ_FOREGROUND              "foreground"
#define OBJ_BACKGROUND              "background"
#define OBJ_BLOCK_EXIT_KEYS         "block_exit_keys"
#define OBJ_MOUSE                   "mouse"
#define OBJ_TYPE                    "type"
#define OBJ_POS_X                   "pos_x"
#define OBJ_POS_Y                   "pos_y"
#define OBJ_NAME                    "name"
#define OBJ_PARENT                  "parent"
#define OBJ_KEY                     "key"
#define OBJ_TEXT                    "text"
#define OBJ_HIDE                    "hide"
#define OBJ_LINE_BREAK              "line_break"
#define OBJ_IGNORE_ESC_KEY          "ignore_esc_key"
#define OBJ_INPUT_LENGTH            "input_length"
#define OBJ_RADIO_GROUP             "radio_group"
#define OBJ_RADIO_TYPE              "radio_type"
#define OBJ_PASSWORD                "password"
#define OBJ_HORIZONTAL_POSITION     "horizontal_position"
#define OBJ_FPS                     "fps"
#define OBJ_DEVICES                 "devices"

/* Nomes dos objetos suportados num DIALOG */
#define DLG_OBJ_KEY                 "key"
#define DLG_OBJ_BOX                 "box"
#define DLG_OBJ_DIGITAL_CLOCK       "digital_clock"
#define DLG_OBJ_IMAGE               "image"
#define DLG_OBJ_MESSAGES_LOG_BOX    "messages_log_box"
#define DLG_OBJ_VAR_TEXT            "var_text"
#define DLG_OBJ_FIXED_TEXT          "fixed_text"
#define DLG_OBJ_CUSTOM              "custom"
#define DLG_OBJ_BUTTON              "button"
#define DLG_OBJ_EDIT                "edit"
#define DLG_OBJ_LIST                "list"
#define DLG_OBJ_CHECKBOX            "checkbox"
#define DLG_OBJ_RADIO               "radio"
#define DLG_OBJ_SLIDER              "slider"
#define DLG_OBJ_LIVE_IMAGE          "live_image"
#define DLG_OBJ_MULTLIVE_IMAGE      "multlive_image"
#define DLG_OBJ_VT_KEYBOARD         "virtual_keyboard"
#define DLG_OBJ_ICON                "icon"
#define DLG_OBJ_TEXTBOX             "textbox"

/* String identificando um separador de itens em um menu */
#define MENU_SEPARATOR              "separator"

/* Layout de teclados */
enum grc_keyboard_layout {
    GRC_KLAYOUT_LETTERS,
    GRC_KLAYOUT_NUMBERS
};

/*
 * Estrutura para identificar os possiveis objetos do tipo "string": valor,
 * em um arquivo GRC.
 */
struct grc_json_key {
    char                        name[128];
    enum al_grc_object_property prop;
    enum grc_entry_type_value   type;
};

/* Estrutura para armazenar um objeto "key" do arquivo GRC */
struct grc_key_data {
    char    *key;
    char    *name;
};

/* Estrutura para armazenar um objeto "object" do arquivo GRC */
struct grc_obj_properties {
    enum al_grc_object  type;
    char                *name;
    char                *parent;
    char                *text;
    char                *fg;
    int                 x;
    int                 y;
    int                 w;
    int                 h;
    int                 hide;
    int                 line_break_mode;
    int                 data_length;
    int                 radio_group;
    int                 radio_type;
    int                 password_mode;
    int                 horizontal_position;
    int                 fps;
    int                 devices;
};

/*
 * Estrutura para mapear os objetos capazes de serem referenciados pelo
 * usuario dentro da estrutura DIALOG interna.
 */
struct dlg_obj_ref {
    struct dlg_obj_ref  *next;
    char                *name;
    int                 dlg_index;
    enum al_grc_object  type;
};

/* Estrutura para ser utilizada nas funcoes de callback dos objetos */
struct al_callback_data {
    struct al_callback_data *next;
    char                    *value_string;
    int                     value_int;
    void                    *user_arg;
    void                    *grc; /* Aponta para a estrutura principal do
                                     aplicativo. */

    /* internal */
    int                     (*callback)(struct al_callback_data *);
};

/* Estrutura com informacoes do modo grafico */
struct al_gfx_info {
    int width;
    int height;
    int color_depth;
    int block_keys;
    int use_mouse;
};

/* Estrutura para armazenar os menus lidos do arquivo GRC */
struct grc_menu {
    struct grc_menu *next;
    char            *name;
    char            *parent;

    /* Lista de itens do menu */
    struct grc_menu *items;

    /* Informacao de cada item */
    char            *text;
};

/* Lista de menus e submenus de um DIALOG */
struct al_menu {
    struct al_menu  *next;
    char            *name;
    int             t_items;
    char            *parent;
    MENU            *menu;
};

struct grc_generic_data {
    struct grc_generic_data   *next;
    char                      data[MAX_EDIT_SIZE];
};

/* Estrutura principal para a manipulacao de um DIALOG da Allegro */
struct al_grc {
    /* JSON carregado do arquivo GRC */
    cjson_t                 *jgrc;

    /* DIALOG da Allegro */
    DIALOG                  *dlg;

    /* Referencias para objetos com propriedade 'name' */
    struct dlg_obj_ref      *ref;

    /* Lista de funcoes de callback de objetos */
    struct al_callback_data *callback_data;

    /* Informacoes principais do modo grafico da Allegro */
    enum al_flag            use_gfx;
    struct al_gfx_info      gfx;

    /* Cores principais do DIALOG */
    int                     fg;
    int                     bg;

    /* Tipo de quebra de linha do objeto 'messages_log_box' */
    enum al_grc_line_break  lbreak;

    /* Flag indicando ignorar tecla ESC ou nao */
    enum al_flag            ignore_esc_key;

    /* Flag indicando se a tecla ESC foi definida pelo usuario */
    enum al_flag            esc_key_user_defined;

    /* Flag indicando a utilizacao ou nao do teclado virtual */
    enum al_flag            virtual_keyboard;

    /*
     * Ponteiro para o objeto 'edit' selecionado ao ativar e utilizar o teclado
     * virtual.
     */
    DIALOG                  *last_edit_object;

    /* Variaveis utilizadas pelo objeto 'digital_clock' */
    char                    dlg_clock_str[256];
    struct tm               dlg_tm;

    /*
     * Lista utilizada para armazenar variaveis para o texto com asterisco '*'
     * num objeto 'edit' com 'password = true' ou para dados de um objeto 'text'
     * provenientes de um objeto pai.
     */
    struct grc_generic_data *g_data;

    /* Variaveis para controle de um objeto do tipo menu */
    struct grc_menu         *grc_menu;
    struct al_menu          *menu;
    MENU                    *dlg_menu;
    int                     dlg_menu_t_items;

    /*
     * Variaveis temporarias durante o processo de criacao em memoria do
     * arquivo GRC.
     */
    cjson_t                 *jtmp;
    cjson_t                 *jtmp_obj;
};

/* Arquivos de prototipos que nao devem ser visiveis para usuarios */
/*#include "gui_button.h"
#include "gui_clock.h"
#include "gui_keyboard.h"
#include "gui_messages_log_box.h"
#include "gui_bitmap.h"
#include "gui_edit.h"
#include "gui_list.h"
#include "gui_slider.h"
#include "gui_check.h"
#include "gui_radio.h"
#include "gui_vt_keyboard.h"
#include "gui_icon.h"
#include "gui_textbox.h"*/

#include "grc_gui.h"
#include "grc.h"

#endif

