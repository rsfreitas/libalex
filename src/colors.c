
/*
 * Description: Functions to handle colors inside the library.
 *
 * Author: Rodrigo Freitas
 * Created at: Fri Jul 29 09:50:59 2016
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
 * Translate a string into a color. Dealing correctly with the number of
 * colors from the GFX mode.
 */
int color_grc_to_al(int color_depth, const char *color_name)
{
    struct color *c = NULL;
    unsigned int i;

    for (i = 0; i < MAX_COLORS; i++)
        if (!strcmp(__colors[i].name, color_name)) {
            c = &__colors[i];
            break;
        }

    if (NULL == c)
        return -1;

    return makecol_depth(color_depth, c->rgb.r, c->rgb.g, c->rgb.b);
}

int color_parse(struct al_grc *grc)
{
    cjson_t *jcolors;
    struct property_detail *dt;
    cstring_t *fg, *bg;

    jcolors = grc_get_object(grc, OBJ_COLORS);

    if (NULL == jcolors) {
        al_set_errno(AL_ERROR_COLORS_BLOCK_NOT_FOUND);
        return -1;
    }

    /* foreground */
    dt = get_property_detail(AL_GRC_JOBJ_FOREGROUND);

    if (NULL == dt)
        goto unknown_grc_key_block;

    fg = grc_get_object_str(jcolors, property_detail_string(dt));

    /* background */
    dt = get_property_detail(AL_GRC_JOBJ_BACKGROUND);

    if (NULL == dt)
        goto unknown_grc_key_block;

    bg = grc_get_object_str(jcolors, property_detail_string(dt));

    /*
     * Global colors. We don't call get_color_depth() from Allegro because
     * at this point it hasn't been initialized yet.
     */
    grc->fg = color_grc_to_al(info_color_depth(grc), cstring_valueof(fg));
    grc->bg = color_grc_to_al(info_color_depth(grc), cstring_valueof(bg));

    cstring_unref(bg);
    cstring_unref(fg);

    return 0;

unknown_grc_key_block:
    al_set_errno(AL_ERROR_UNDEFINED_GRC_KEY);
    return -1;
}

int color_get_global_fg(struct al_grc *grc)
{
    if (NULL == grc)
        return -1;

    return grc->fg;
}

int color_get_global_bg(struct al_grc *grc)
{
    if (NULL == grc)
        return -1;

    return grc->bg;
}

