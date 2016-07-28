
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Sun Aug 24 21:05:01 2014
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

#ifndef _LIBALEX_H
#define _LIBALEX_H		        1

#ifdef LIBALEX_COMPILE
# define MAJOR_VERSION          0
# define MINOR_VERSION          1
# define BUILD                  0
#endif

#ifndef LIBEXPORT
# ifdef LINUX
#  define LIBEXPORT             __attribute__((visibility("default")))
# else
#  define LIBEXPORT
# endif
#endif

#ifndef ALLEGRO_H
# include <allegro.h>
#endif

#ifndef _COLLECTIONS_H
# include <collections.h>
#endif

/* Number of supported colors */
enum al_grc_color_depth {
    AL_GRC_COLOR_8 = 8,
    AL_GRC_COLOR_15 = 15,
    AL_GRC_COLOR_16 = 16,
    AL_GRC_COLOR_24 = 24,
    AL_GRC_COLOR_32 = 32
};

/* 'messages_log_box' breaking line types */
enum al_grc_line_break {
    AL_GRC_LBREAK_RAW = 1,  /* raw */
    AL_GRC_LBREAK_SMART     /* smart */
};

/* radio button formats */
enum al_grc_radio_button_fmt {
    AL_GRC_RADIO_CIRCLE = 1,
    AL_GRC_RADIO_SQUARE
};

/* Horizontal positions */
enum al_grc_hpos {
    AL_GRC_H_POS_LEFT = 1,
    AL_GRC_H_POS_RIGHT
};

/* Fields able to handle variable data inside an object */
enum al_data_type {
    /* Original Allegro names */
    AL_DT_D1,
    AL_DT_D2,
    AL_DT_DP,
    AL_DT_DP2,
    AL_DT_DP3,

    /* User friendly names */
    AL_DT_SLIDER_LIMIT,
    AL_DT_SLIDER_POSITION,
    AL_DT_EDIT_VALUE,
    AL_DT_RADIO_STATE,
    AL_DT_CHECKBOX_STATE,
    AL_DT_LIST_POSITION,
    AL_DT_LIST_CONTENT_BUILD,
    AL_DT_TEXT,
    AL_DT_KEY_SCANCODE,
    AL_DT_ICON
};

/*
 * Objects from a DIALOG, including the key objects, even having a specific
 * section inside de GRC file.
 */
enum al_grc_object {
    AL_GRC_OBJ_KEY,
    AL_GRC_OBJ_BOX,
    AL_GRC_OBJ_DIGITAL_CLOCK,
    AL_GRC_OBJ_IMAGE,
    AL_GRC_OBJ_MESSAGES_LOG_BOX,
    AL_GRC_OBJ_VAR_TEXT,
    AL_GRC_OBJ_FIXED_TEXT,
    AL_GRC_OBJ_CUSTOM,
    AL_GRC_OBJ_BUTTON,
    AL_GRC_OBJ_EDIT,
    AL_GRC_OBJ_LIST,
    AL_GRC_OBJ_CHECK,
    AL_GRC_OBJ_RADIO,
    AL_GRC_OBJ_SLIDER,
    AL_GRC_OBJ_LIVE_IMAGE,
    AL_GRC_OBJ_MULTLIVE_IMAGE,
    AL_GRC_OBJ_VT_KEYBOARD,
    AL_GRC_OBJ_ICON,
    AL_GRC_OBJ_TEXTBOX
};

/* JSON objects of the GRC file */
enum al_grc_object_property {
    AL_GRC_JOBJ_WIDTH,
    AL_GRC_JOBJ_HEIGHT,
    AL_GRC_JOBJ_COLOR_DEPTH,
    AL_GRC_JOBJ_FOREGROUND,
    AL_GRC_JOBJ_BACKGROUND,
    AL_GRC_JOBJ_BLOCK_EXIT_KEYS,
    AL_GRC_JOBJ_MOUSE,
    AL_GRC_JOBJ_TYPE,
    AL_GRC_JOBJ_POS_X,
    AL_GRC_JOBJ_POS_Y,
    AL_GRC_JOBJ_NAME,
    AL_GRC_JOBJ_PARENT,
    AL_GRC_JOBJ_KEY,
    AL_GRC_JOBJ_TEXT,
    AL_GRC_JOBJ_HIDE,
    AL_GRC_JOBJ_LINE_BREAK,
    AL_GRC_JOBJ_IGNORE_ESC_KEY,
    AL_GRC_JOBJ_INPUT_LENGTH,
    AL_GRC_JOBJ_RADIO_GROUP,
    AL_GRC_JOBJ_RADIO_TYPE,
    AL_GRC_JOBJ_PASSWORD_MODE,
    AL_GRC_JOBJ_H_POSITION,
    AL_GRC_JOBJ_FPS,
    AL_GRC_JOBJ_DEVICES
};

/* Nonexistent keys inside the Allegro */
enum al_key {
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
#define AL_BLACK                        "black"
#define AL_WHITE                        "white"
#define AL_LIGHT_RED                    "light_red"
#define AL_LIGHT_GREEN                  "light_green"
#define AL_LIGHT_BLUE                   "light_blue"
#define AL_YELLOW                       "yellow"
#define AL_CYAN                         "cyan"
#define AL_MAGENTA                      "magenta"
#define AL_SILVER                       "silver"
#define AL_GRAY                         "gray"
#define AL_MAROON                       "maroon"
#define AL_OLIVE                        "olive"
#define AL_PURPLE                       "purple"
#define AL_TEAL                         "teal"
#define AL_NAVY                         "navy"
#define AL_BLUE                         "blue"
#define AL_GREEN                        "green"
#define AL_RED                          "red"
#define AL_ORANGE_RED                   "orange_red"
#define AL_ORANGE                       "orange"
#define AL_KHAKI                        "khaki"
#define AL_BROWN                        "brown"
#define AL_SADDLE_BROWN                 "saddle_brown"
#define AL_BAKERS_CHOCOLATE             "bakers_chocolate"
#define AL_IVORY                        "ivory"
#define AL_SNOW                         "snow"
#define AL_GHOST_WHITE                  "ghost_white"
#define AL_INDIGO                       "indigo"
#define AL_DARK_SLATE_BLUE              "dark_slate_blue"
#define AL_SKY_BLUE                     "sky_blue"
#define AL_SEA_GREEN                    "sea_green"
#define AL_DARK_GREEN                   "dark_green"
#define AL_FOREST_GREEN                 "forest_green"
#define AL_GOLD                         "gold"
#define AL_GOLDEN_ROD                   "golden_rod"
#define AL_SALMON                       "salmon"
#define AL_CORAL                        "coral"
#define AL_TOMATO                       "tomato"
#define AL_DARK_SEA_GREEN               "dark_sea_green"
#define AL_DARK_CYAN                    "dark_cyan"
#define AL_TURQUOISE                    "turquoise"
#define AL_DARK_TURQUOISE               "dark_turquoise"
#define AL_CADET_BLUE                   "cadet_blue"
#define AL_OLIVE_DRAB                   "olive_drab"
#define AL_STEEL_BLUE                   "steel_blue"
#define AL_DODGER_BLUE                  "dodger_blue"
#define AL_MIDNIGHT_BLUE                "midnight_blue"
#define AL_SLATE_BLUE                   "slate_blue"
#define AL_SIENNA                       "sienna"
#define AL_PERU                         "peru"
#define AL_TAN                          "tan"
#define AL_SLATE_GRAY                   "slate_gray"
#define AL_DARK_KHAKI                   "dark_khaki"
#define AL_INDIAN_RED                   "indian_red"

/* Custom supported messages by objects from DIALOGs */
#define MSG_NEW_LOG_TEXT                MSG_USER
#define MSG_CLEAR_LOG_TEXT              MSG_USER + 1
#define MSG_LOAD_IMAGE                  MSG_USER + 2
#define MSG_UPDATE_CURSOR_POSITION      MSG_USER + 3

/* Exported types */
struct al_grc;
struct al_callback_data;

#include "alex/al_error.h"
#include "alex/al_api.h"
#include "alex/al_write.h"

#ifdef LIBALEX_COMPILE
# include "alex/al_internal.h"
#endif

#endif

