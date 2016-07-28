
/*
 * Description: Internal library declarations only.
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

/* Main objects of a GRC file */
#define OBJ_INFO                    "info"
#define OBJ_OBJECTS                 "objects"
#define OBJ_KEYS                    "keys"
#define OBJ_COLORS                  "colors"
#define OBJ_MENU                    "menu"
#define OBJ_MAIN_OPTIONS            "main_options"
#define OBJ_MENU_OPTIONS            "menu_options"
#define OBJ_OPTIONS                 "options"

/* Secondary objects of a GRC file */
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

/* DIALOG supported object names (values) */
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
    bool                hide;
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
    clist_t             *prev;
    clist_t             *next;
    char                *name;
    int                 dlg_index;
    enum al_grc_object  type;
};

/* Structure to be used in the callback functions */
struct al_callback_data {
    clist_t         *prev;
    clist_t         *next;
    char            *value_string;
    int             value_int;
    void            *user_arg;
    void            *grc; /* Pointer to the main structure */

    /* internal */
    int             (*callback)(struct al_callback_data *);
};

/* Graphic mode informations */
struct al_gfx_info {
    int     width;
    int     height;
    int     color_depth;
    bool    block_keys;
    bool    use_mouse;
};

/* Menus loaded */
struct grc_menu {
    clist_t         *prev;
    clist_t         *next;
    char            *name;
    char            *parent;

    /* Items from menu */
    struct grc_menu *items;

    /* Item info */
    char            *text;
};

/* List of menus and submenus from a DIALOG */
struct al_menu {
    clist_t     *prev;
    clist_t     *next;
    char        *name;
    int         t_items;
    char        *parent;
    MENU        *menu;
};

struct grc_generic_data {
    clist_t     *prev;
    clist_t     *next;
    char        data[MAX_EDIT_SIZE];
};

/* TODO Split this structure in others */
/* Main structure to handle an Allegro DIALOG */
struct al_grc {
    /* GRC is a JSON object inside */
    cjson_t                 *jgrc;

    /* Allegro's DIALOG */
    DIALOG                  *dlg;

    /* Objects with property 'name' */
    struct dlg_obj_ref      *ref;

    /* Callback functions */
    struct al_callback_data *callback_data;

    /* Graphic mode info */
    bool                    use_gfx;
    struct al_gfx_info      gfx;
    bool                    are_we_prepared;

    /* Main colors of the DIALOG */
    int                     fg;
    int                     bg;

    /* 'messages_log_box' break line type */
    enum al_grc_line_break  lbreak;

    /* Flag to ignore the ESC key */
    bool                    ignore_esc_key;

    /* Flag showing if the user has declared the ESC key */
    bool                    esc_key_user_defined;

    /* Flag to use the virtual keyboard */
    bool                    virtual_keyboard;

    /*
     * Pointer to the 'edit' object that was selected when the virtual
     * keyboard is activated.
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

/* gui.c */
int gui_change_resolution(struct al_grc *grc);
int gui_load_colors(struct al_grc *grc);
void gui_reset_resolution(void);
int create_DIALOG(struct al_grc *grc);
void run_DIALOG(struct al_grc *grc);
int run_callback(struct al_callback_data *acd, unsigned int default_return);
int grc_tr_color_to_al_color(int color_depth, const char *color);
DIALOG *get_DIALOG_from_grc(struct al_grc *grc, const char *object_name);
struct grc_menu *get_grc_menu_from_grc(struct al_grc *grc,
                                       const char *object_name);

MENU *get_MENU_from_grc(struct al_grc *grc, const char *object_name);

/* parser.c */
struct grc_json_key *get_grc_json_key(enum al_grc_object_property prop);
int grc_get_object_value(cjson_t *object, const char *object_name,
                         int default_value);

char *grc_get_object_str(cjson_t *object, const char *object_name);
int grc_parse_file(struct al_grc *grc, const char *grc_filename);
int grc_parse_mem(struct al_grc *grc, const char *data);

/* error.c */
void al_errno_clear(void);
void al_set_errno(enum al_error_code code);

/* al_callback.c */
struct al_callback_data *new_callback_data(void);

/* al_menu.c */
void destroy_al_menu(void *a);
struct al_menu *new_al_menu(void);

/* grc.c */
struct al_grc *new_grc(void);
void destroy_grc(struct al_grc *grc);

/* grc_generic.c */
struct grc_generic_data *new_grc_generic_data(void);

/* grc_key.c */
void destroy_key_data(struct grc_key_data *kdata);
struct grc_key_data *new_key_data(cjson_t *key);

/* grc_menu.c */
void destroy_grc_menu(void *a);
struct grc_menu *new_menu(cjson_t *object);

/* grc_obj_properties.c */
void destroy_obj_properties(struct grc_obj_properties *odata);
struct grc_obj_properties *new_obj_properties(cjson_t *object);

/* obj_ref.c */
void destroy_obj_ref(void *a);
struct dlg_obj_ref *new_obj_ref(const char *name, int dlg_index,
                                enum al_grc_object type);

/* utils.c */
void dotted_rect(int x1, int y1, int x2, int y2, int fg, int bg);
int tr_str_type_to_grc_type(const char *type_name);
int tr_line_break(const char *mode);
int tr_radio_type(const char *type);
int tr_horizontal_position(const char *pos);
const char *str_radio_type(enum al_grc_radio_button_fmt radio);
const char *str_horizontal_position(enum al_grc_hpos hpos);
const char *str_line_break(enum al_grc_line_break lbreak);
const char *str_grc_obj_type(enum al_grc_object obj);

#endif

