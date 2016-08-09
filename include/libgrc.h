
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Sun Aug 24 21:05:01 2014
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

#ifndef _LIBGRC_H
#define _LIBGRC_H		        1

#ifdef LIBGRC_COMPILE
# define MAJOR_VERSION          0
# define MINOR_VERSION          1
# define BUILD                  0
#endif

#ifndef LIBEXPORT
# define LIBEXPORT             __attribute__((visibility("default")))
#endif

#ifndef ALLEGRO_H
# include <allegro.h>
#endif

#ifndef _COLLECTIONS_H
# include <collections.h>
#endif

/* Number of supported colors */
enum grc_color_depth {
    GRC_COLOR_8 = 8,
    GRC_COLOR_15 = 15,
    GRC_COLOR_16 = 16,
    GRC_COLOR_24 = 24,
    GRC_COLOR_32 = 32
};

/* 'messages_log_box' breaking line types */
enum grc_line_break {
    GRC_LINE_BREAK_RAW = 1,  /* raw */
    GRC_LINE_BREAK_SMART     /* smart */
};

/* radio button formats */
enum grc_radio_button_fmt {
    GRC_RADIO_CIRCLE = 1,
    GRC_RADIO_SQUARE
};

/* Horizontal positions */
enum grc_horizontal_position {
    GRC_H_POS_LEFT = 1,
    GRC_H_POS_RIGHT
};

/* Fields able to handle variable data inside an object */
enum grc_object_member {
    /* Original Allegro names */
    GRC_MEMBER_D1,
    GRC_MEMBER_D2,
    GRC_MEMBER_DP,
    GRC_MEMBER_DP2,
    GRC_MEMBER_DP3,

    /* User friendly names */
    GRC_MEMBER_SLIDER_LIMIT,
    GRC_MEMBER_SLIDER_POSITION,
    GRC_MEMBER_EDIT_VALUE,
    GRC_MEMBER_RADIO_STATE,
    GRC_MEMBER_CHECKBOX_STATE,
    GRC_MEMBER_LIST_POSITION,
    GRC_MEMBER_LIST_CONTENT_BUILD,
    GRC_MEMBER_TEXT,
    GRC_MEMBER_KEY_SCANCODE,
    GRC_MEMBER_ICON
};

/*
 * Objects from a DIALOG, including the key objects, even having a specific
 * section inside de GRC file.
 */
enum grc_object {
    GRC_OBJECT_KEY,
    GRC_OBJECT_BOX,
    GRC_OBJECT_DIGITAL_CLOCK,
    GRC_OBJECT_IMAGE,
    GRC_OBJECT_MESSAGES_LOG_BOX,
    GRC_OBJECT_VAR_TEXT,
    GRC_OBJECT_FIXED_TEXT,
    GRC_OBJECT_CUSTOM,
    GRC_OBJECT_BUTTON,
    GRC_OBJECT_EDIT,
    GRC_OBJECT_LIST,
    GRC_OBJECT_CHECK,
    GRC_OBJECT_RADIO,
    GRC_OBJECT_SLIDER,
    GRC_OBJECT_LIVE_IMAGE,
    GRC_OBJECT_MULTLIVE_IMAGE,
    GRC_OBJECT_VT_KEYBOARD,
    GRC_OBJECT_ICON,
    GRC_OBJECT_TEXTBOX,
    GRC_OBJECT_MENU,
    GRC_OBJECT_MENU_ITEM
};

/* JSON objects of the GRC file */
enum grc_object_property {
    GRC_PROPERTY_WIDTH,
    GRC_PROPERTY_HEIGHT,
    GRC_PROPERTY_COLOR_DEPTH,
    GRC_PROPERTY_FOREGROUND,
    GRC_PROPERTY_BACKGROUND,
    GRC_PROPERTY_BLOCK_EXIT_KEYS,
    GRC_PROPERTY_MOUSE,
    GRC_PROPERTY_TYPE,
    GRC_PROPERTY_POS_X,
    GRC_PROPERTY_POS_Y,
    GRC_PROPERTY_TAG,
    GRC_PROPERTY_PARENT,
    GRC_PROPERTY_KEY,
    GRC_PROPERTY_TEXT,
    GRC_PROPERTY_HIDE,
    GRC_PROPERTY_LINE_BREAK,
    GRC_PROPERTY_IGNORE_ESC_KEY,
    GRC_PROPERTY_INPUT_LENGTH,
    GRC_PROPERTY_RADIO_GROUP,
    GRC_PROPERTY_RADIO_TYPE,
    GRC_PROPERTY_PASSWORD_MODE,
    GRC_PROPERTY_H_POSITION,
    GRC_PROPERTY_FPS,
    GRC_PROPERTY_DEVICES
};

/*
 * Nonexistent keys inside the Allegro. We kept the prefix KEY_ to an
 * Allegro compatibility.
 */
enum grc_key {
    KEY_EXCLAMATION = KEY_MAX + 1,
    KEY_SHARP,
    KEY_DOLLAR,
    KEY_PERCENT,
    KEY_AMPERSAND,
    KEY_LPARENTHESIS,
    KEY_RPARENTHESIS,
    KEY_UNDERLINE,
    KEY_PLUS,
    KEY_LBRACKET,
    KEY_RBRACKET,
    KEY_PIPE,
    KEY_GRAVE_ACCENT,
    KEY_DOUBLE_QUOTE,
    KEY_LESS,
    KEY_GREATER,
    KEY_INTERROGATION
};

/* Supported colors */
#define GRC_BLACK                       "black"
#define GRC_WHITE                       "white"
#define GRC_LIGHT_RED                   "light_red"
#define GRC_LIGHT_GREEN                 "light_green"
#define GRC_LIGHT_BLUE                  "light_blue"
#define GRC_YELLOW                      "yellow"
#define GRC_CYAN                        "cyan"
#define GRC_MAGENTA                     "magenta"
#define GRC_SILVER                      "silver"
#define GRC_GRAY                        "gray"
#define GRC_MAROON                      "maroon"
#define GRC_OLIVE                       "olive"
#define GRC_PURPLE                      "purple"
#define GRC_TEAL                        "teal"
#define GRC_NAVY                        "navy"
#define GRC_BLUE                        "blue"
#define GRC_GREEN                       "green"
#define GRC_RED                         "red"
#define GRC_ORANGE_RED                  "orange_red"
#define GRC_ORANGE                      "orange"
#define GRC_KHAKI                       "khaki"
#define GRC_BROWN                       "brown"
#define GRC_SADDLE_BROWN                "saddle_brown"
#define GRC_BAKERS_CHOCOLATE            "bakers_chocolate"
#define GRC_IVORY                       "ivory"
#define GRC_SNOW                        "snow"
#define GRC_GHOST_WHITE                 "ghost_white"
#define GRC_INDIGO                      "indigo"
#define GRC_DARK_SLATE_BLUE             "dark_slate_blue"
#define GRC_SKY_BLUE                    "sky_blue"
#define GRC_SEA_GREEN                   "sea_green"
#define GRC_DARK_GREEN                  "dark_green"
#define GRC_FOREST_GREEN                "forest_green"
#define GRC_GOLD                        "gold"
#define GRC_GOLDEN_ROD                  "golden_rod"
#define GRC_SALMON                      "salmon"
#define GRC_CORAL                       "coral"
#define GRC_TOMATO                      "tomato"
#define GRC_DARK_SEA_GREEN              "dark_sea_green"
#define GRC_DARK_CYAN                   "dark_cyan"
#define GRC_TURQUOISE                   "turquoise"
#define GRC_DARK_TURQUOISE              "dark_turquoise"
#define GRC_CADET_BLUE                  "cadet_blue"
#define GRC_OLIVE_DRAB                  "olive_drab"
#define GRC_STEEL_BLUE                  "steel_blue"
#define GRC_DODGER_BLUE                 "dodger_blue"
#define GRC_MIDNIGHT_BLUE               "midnight_blue"
#define GRC_SLATE_BLUE                  "slate_blue"
#define GRC_SIENNA                      "sienna"
#define GRC_PERU                        "peru"
#define GRC_TAN                         "tan"
#define GRC_SLATE_GRAY                  "slate_gray"
#define GRC_DARK_KHAKI                  "dark_khaki"
#define GRC_INDIAN_RED                  "indian_red"

/* Custom supported messages by objects from DIALOGs */
#define MSG_NEW_LOG_TEXT                MSG_USER
#define MSG_CLEAR_LOG_TEXT              MSG_USER + 1
#define MSG_LOAD_IMAGE                  MSG_USER + 2
#define MSG_UPDATE_CURSOR_POSITION      MSG_USER + 3

/* Exported types */
typedef void    grc_t;
typedef void    grc_callback_data_t;

#include "grc/grc_error.h"
#include "grc/grc_api.h"
#include "grc/grc_write.h"

#ifdef LIBGRC_COMPILE
# include "grc/grc_internal.h"
#endif

#endif

