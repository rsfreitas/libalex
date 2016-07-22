
/*
 * Description:
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
#define _LIBALEX_INTERNAL_H         1

/* Maximum string size supported by an 'edit' object */
#define MAX_EDIT_SIZE               1024

/* Default main window resolution and color */
#define AL_DEFAULT_WIDTH            800
#define AL_DEFAULT_HEIGHT           600
#define AL_DEFAULT_COLOR_DEPTH      32

/* Values supported inside a GRC file */
enum grc_entry_type_value {
    GRC_NUMBER,
    GRC_STRING,
    GRC_BOOL
};

/* Main objects of GRC file */
#define OBJ_INFO                    "info"
#define OBJ_OBJECTS                 "objects"
#define OBJ_KEYS                    "keys"
#define OBJ_COLORS                  "colors"
#define OBJ_MENU                    "menu"
#define OBJ_MAIN_OPTIONS            "main_options"
#define OBJ_MENU_OPTIONS            "menu_options"
#define OBJ_OPTIONS                 "options"

/* Secondary objects of GRC file */
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

/* DIALOG supported object names */
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

/* Menu separator string */
#define MENU_SEPARATOR              "separator"

/* Keyboard layout */
enum grc_keyboard_layout {
    GRC_KLAYOUT_LETTERS,
    GRC_KLAYOUT_NUMBERS
};

/*
 * Structure to identify possible objects of type "string": value
 * in a GRC file.
 */
struct grc_json_key {
    char                        name[128];
    enum al_grc_object_property prop;
    enum grc_entry_type_value   type;
};

/* Structure to store an object "key" of a GRC file */
struct grc_key_data {
    char    *key;
    char    *name;
};

/* Structure to store an object "object" of a GRC file */
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
 * Structure to map objects able to be referenced by the user inside a
 * DIALOG structure.
 */
struct dlg_obj_ref {
    struct dlg_obj_ref  *prev;
    struct dlg_obj_ref  *next;
    char                *name;
    int                 dlg_index;
    enum al_grc_object  type;
};

/* Structure to be used in the callback functions */
struct al_callback_data {
    struct al_callback_data *prev;
    struct al_callback_data *next;
    char                    *value_string;
    int                     value_int;
    void                    *user_arg;
    void                    *grc; /* Pointer to the main structure */

    /* internal */
    int                     (*callback)(struct al_callback_data *);
};

/* Graphic mode informations */
struct al_gfx_info {
    int width;
    int height;
    int color_depth;
    int block_keys;
    int use_mouse;
};

/* Menus loaded */
struct grc_menu {
    struct grc_menu *prev;
    struct grc_menu *next;
    char            *name;
    char            *parent;

    /* Items from menu */
    struct grc_menu *items;

    /* Item info */
    char            *text;
};

/* List of menus and submenus from a DIALOG */
struct al_menu {
    struct al_menu  *prev;
    struct al_menu  *next;
    char            *name;
    int             t_items;
    char            *parent;
    MENU            *menu;
};

struct grc_generic_data {
    struct grc_generic_data   *prev;
    struct grc_generic_data   *next;
    char                      data[MAX_EDIT_SIZE];
};

/* Main structure to handle an Allegro DIALOG */
struct al_grc {
    /* GRC JSON */
    cjson_t                 *jgrc;

    /* Allegro's DIALOG */
    DIALOG                  *dlg;

    /* Objects with property 'name' */
    struct dlg_obj_ref      *ref;

    /* Callback functions */
    struct al_callback_data *callback_data;

    /* Graphic mode info */
    enum al_flag            use_gfx;
    struct al_gfx_info      gfx;

    /* Main colors of the DIALOG */
    int                     fg;
    int                     bg;

    /* 'messages_log_box' break line type */
    enum al_grc_line_break  lbreak;

    /* Flag to ignore the ESC key */
    enum al_flag            ignore_esc_key;

    /* Flag showing if the user has declared the ESC key */
    enum al_flag            esc_key_user_defined;

    /* Flag to use the virtual keyboard */
    enum al_flag            virtual_keyboard;

    /*
     * Pointer to the 'edit' object selected while activated the virtual
     * keyboard.
     */
    DIALOG                  *last_edit_object;

    /* 'digital_clock' info */
    char                    dlg_clock_str[256];
    struct tm               dlg_tm;

    /*
     * List of values to store texts with asterisks '*' in a 'edit' object with
     * 'password = true' or data from a 'text' object.
     */
    struct grc_generic_data *g_data;

    /* Used to control a menu from an object */
    struct grc_menu         *grc_menu;
    struct al_menu          *menu;
    MENU                    *dlg_menu;
    int                     dlg_menu_t_items;

    /* Temporary values while creating a GRC file. */
    cjson_t                 *jtmp;
    cjson_t                 *jtmp_obj;
};

#include "gui.h"
#include "parser.h"

#endif

