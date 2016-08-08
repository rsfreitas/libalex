
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

/** Defines */

/* Maximum string size supported by an 'edit' object */
#define MAX_EDIT_SIZE               1024

/* Default main window resolution and color */
#define AL_DEFAULT_WIDTH            800
#define AL_DEFAULT_HEIGHT           600
#define AL_DEFAULT_COLOR_DEPTH      32

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
#define OBJ_TAG                     "tag"
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

/* Edit deafult height */
#define DEFAULT_EDIT_HEIGHT         15

/* Space between buttons */
#define BUTTON_DEFAULT_SPACER        4

/* Button default height */
#define DEFAULT_BUTTON_HEIGHT       30

/* Space between checkboxes */
#define CHECKBOX_DEFAULT_SPACER     13

/* Checkbox default height */
#define DEFAULT_CHECKBOX_HEIGHT     15

/* Space between radio buttons */
#define RADIO_DEFAULT_SPACER        13

/* Radio button default height */
#define DEFAULT_RADIO_HEIGHT        15

/** Enumerators */

/* Keyboard layout */
enum grc_keyboard_layout {
    GRC_KLAYOUT_LETTERS,
    GRC_KLAYOUT_NUMBERS
};

/* Values supported inside a GRC file */
enum grc_entry_type_value {
    GRC_NUMBER,
    GRC_STRING,
    GRC_BOOL
};

/* Internal known objects */
enum grc_object_type {
    GRC_OBJ_STANDARD,
    GRC_OBJ_MENU,
    GRC_OBJ_KEY,
    GRC_OBJ_MENU_ITEM
};

/** Structures */

/** Object's properties loaded from a GRC file */
struct grc_obj_properties;

/** A structure to get a property detail */
struct property_detail;

/** Structure to be used in the callback functions */
struct callback_data;

/** Main UI informations */
struct gfx_info;

/** DIALOG colors */
struct gfx_color;

struct grc_generic_data {
    clist_t     *prev;
    clist_t     *next;
    char        data[MAX_EDIT_SIZE];
};

struct grc_object {
    clist_t                     *prev;
    clist_t                     *next;

    enum grc_object_type        type;       /** Object type */
    char                        *tag;       /** Reference tag */

    DIALOG                      *dlg;       /** Real Allegro's DIALOG object */
    struct callback_data        *cb_data;   /** Object's callback */
    struct grc_generic_data     *g_data; // TODO: Replace this for a cstring_list_t
    struct grc_obj_properties   *prop;

    /* Maybe this a 'digital_clock' object */
    char                        dlg_clock_str[256];

    /* Is this a menu? */
    MENU                        *menu;
    struct grc_object           *items;
};

/* Main structure to handle an Allegro DIALOG */
struct al_grc {
    /* GRC is a JSON object inside */
    cjson_t                 *jgrc;

    /*
     * Even having a DIALOG structure inside every grc_object we keep this
     * to be the real DIALOG, the one used by Allegro.
     */
    DIALOG                  *al_dlg;
    struct grc_object       *ui_objects;
    struct grc_object       *tmp_objects;
    struct grc_object       *ui_keys;
    struct grc_object       *ui_menu;

    /* Graphic mode info */
    struct gfx_info         *info;

    /* Main colors of the DIALOG */
    int                     fg;
    int                     bg;

    /*
     * Pointer to the 'edit' object that was selected when a virtual
     * keyboard is activated.
     */
    DIALOG                  *last_edit_object;

    /* 'digital_clock' info */
    char                    dlg_clock_str[256];
    struct tm               dlg_tm;

    /* Temporary values while creating a GRC file. */
    cjson_t                 *jtmp;
    cjson_t                 *jtmp_obj;
};

/** Prototypes */

/* gui.c */
int gui_init(struct al_grc *grc);
void gui_reset_resolution(void);
int DIALOG_create(struct al_grc *grc);
void run_DIALOG(struct al_grc *grc);
int grc_tr_color_to_al_color(int color_depth, const char *color);
DIALOG *get_DIALOG_from_grc(struct al_grc *grc, const char *object_name);
struct grc_menu *get_grc_menu_from_grc(struct al_grc *grc,
                                       const char *object_name);

MENU *get_MENU_from_grc(struct al_grc *grc, const char *object_name);

/* parser.c */
int grc_get_object_value(cjson_t *object, const char *object_name,
                         int default_value);

cstring_t *grc_get_object_str(cjson_t *object, const char *object_name);
int parse_file(struct al_grc *grc, const char *grc_filename);
int parse_mem(struct al_grc *grc, const char *data);
int parse_colors(struct al_grc *grc);
int parse_objects(struct al_grc *grc);

/* error.c */
void al_errno_clear(void);
void al_set_errno(enum al_error_code code);

/* callback.c */
struct callback_data *new_callback_data(void);
void destroy_callback_data(void *a);
void set_object_callback_data(struct grc_object *gobject,
                              struct al_grc *grc);

int get_callback_data(struct callback_data *acd, enum al_data_type data,
                      va_list ap);
void *get_callback_user_arg(struct callback_data *acd);
struct al_grc *get_callback_grc(struct callback_data *acd);
int set_callback(struct al_grc *grc, DIALOG *dlg,
                 int (*callback)(struct callback_data *), void *arg);

int run_callback(struct callback_data *acd, unsigned int default_return);
void callback_set_int(struct callback_data *acd, int value);
void callback_set_string(struct callback_data *acd, char *value);

/* grc.c */
struct al_grc *new_grc(void);
void destroy_grc(struct al_grc *grc);
void grc_creates_reference(struct al_grc *grc, struct grc_object *object);
DIALOG *grc_get_DIALOG_from_tag(struct al_grc *grc, const char *tag);
MENU *grc_get_MENU_from_tag(struct al_grc *grc, const char *tag);

/* grc_generic.c */
struct grc_generic_data *new_grc_generic_data(void);
void destroy_grc_generic_data(void *a);

/* grc_object.c */
void destroy_grc_object(void *a);
struct grc_object *new_grc_object(enum grc_object_type type);
struct grc_obj_properties *grc_object_get_properties(struct grc_object *object);
DIALOG *grc_object_get_DIALOG(struct grc_object *object);
MENU *grc_object_get_MENU(struct grc_object *object);
void grc_object_set_MENU(struct grc_object *object, MENU *menu);
void grc_object_set_tag(struct grc_object *object, const char *tag);
DIALOG *grc_object_get_DIALOG_from_tag(struct grc_object *object,
                                       const char *tag);

MENU *grc_object_get_MENU_from_tag(struct grc_object *object, const char *tag);

/* object_properties.c */
void destroy_obj_properties(struct grc_obj_properties *odata);
struct grc_obj_properties *new_obj_properties(cjson_t *object);
bool grc_obj_properties_has_name(struct grc_obj_properties *prop);
bool grc_obj_properties_has_parent(struct grc_obj_properties *prop);
bool grc_obj_properties_has_fg(struct grc_obj_properties *prop);
const char *grc_obj_get_property_name(struct grc_obj_properties *prop);
const char *grc_obj_get_property_parent(struct grc_obj_properties *prop);
const char *grc_obj_get_property_text(struct grc_obj_properties *prop);
const char *grc_obj_get_property_fg(struct grc_obj_properties *prop);
const char *grc_obj_get_property_key(struct grc_obj_properties *prop);
enum al_grc_object grc_obj_get_property_type(struct grc_obj_properties *prop);
int grc_obj_get_property_x(struct grc_obj_properties *prop);
int grc_obj_get_property_y(struct grc_obj_properties *prop);
int grc_obj_get_property_w(struct grc_obj_properties *prop);
int grc_obj_get_property_h(struct grc_obj_properties *prop);
int grc_obj_get_property_line_break_mode(struct grc_obj_properties *prop);
int grc_obj_get_property_data_length(struct grc_obj_properties *prop);
int grc_obj_get_property_radio_group(struct grc_obj_properties *prop);
int grc_obj_get_property_radio_type(struct grc_obj_properties *prop);
int grc_obj_get_property_password_mode(struct grc_obj_properties *prop);
int grc_obj_get_property_horizontal_position(struct grc_obj_properties *prop);
bool grc_obj_get_property_hide(struct grc_obj_properties *prop);

int grc_obj_set_property_type(struct grc_obj_properties *prop,
                              enum al_grc_object type);

#define PROP_get(prop, name)    \
    grc_obj_get_property_##name(prop)

#define PROP_check(prop, name)  \
    grc_obj_properties_has_##name(prop)

#define PROP_set(prop, name, value) \
    grc_obj_set_property_##name(prop, value)

struct property_detail *get_property_detail(enum al_grc_object_property prop);
const char *property_detail_string(struct property_detail *d);
enum al_grc_object_property property_detail(struct property_detail *d);
enum grc_entry_type_value propery_detail_type(struct property_detail *d);

/* utils.c */
void dotted_rect(int x1, int y1, int x2, int y2, int fg, int bg);
int tr_str_type_to_grc_type(const char *type_name);
int tr_line_break(const char *mode);
int tr_radio_type(const char *type);
int tr_horizontal_position(const char *pos);
int tr_str_key_to_al_key(const char *skey);
const char *str_radio_type(enum al_grc_radio_button_fmt radio);
const char *str_horizontal_position(enum al_grc_hpos hpos);
const char *str_line_break(enum al_grc_line_break lbreak);
const char *str_grc_obj_type(enum al_grc_object obj);
cjson_t *grc_get_object(struct al_grc *grc, const char *object);

/* colors.c */
int color_parse(struct al_grc *grc);
int color_grc_to_al(int color_depth, const char *color_name);
int color_get_global_fg(struct al_grc *grc);
int color_get_global_bg(struct al_grc *grc);

/* info.c */
int info_parse(struct al_grc *grc);
int info_color_depth(struct al_grc *grc);
struct gfx_info *info_start(void);
void info_finish(struct gfx_info *info);
int info_set_value(struct gfx_info *info, enum al_gfx_info field, ...);
int info_get_value(struct gfx_info *info, enum al_gfx_info field);

#endif

