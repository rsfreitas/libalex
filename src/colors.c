
/*
 * Description: Functions to handle colors inside the library.
 *
 * Author: Rodrigo Freitas
 * Created at: Fri Jul 29 09:50:59 2016
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

struct gfx_color_s {
    cstring_t   *s_fg;
    cstring_t   *s_bg;
    int         fg;
    int         bg;
};

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
    { GRC_BLACK,             { 0,    0,      0   }},
    { GRC_WHITE,             { 255,  255,    255 }},
    { GRC_LIGHT_RED,         { 255,  0,      0   }},
    { GRC_LIGHT_GREEN,       { 0,    255,    0   }},
    { GRC_LIGHT_BLUE,        { 0,    0,      255 }},
    { GRC_YELLOW,            { 240,  240,    0   }},
    { GRC_CYAN,              { 0,    255,    255 }},
    { GRC_MAGENTA,           { 255,  0,      255 }},
    { GRC_SILVER,            { 192,  192,    192 }},
    { GRC_GRAY,              { 128,  128,    128 }},
    { GRC_MAROON,            { 128,  0,      0   }},
    { GRC_OLIVE,             { 128,  128,    0   }},
    { GRC_PURPLE,            { 128,  0,      128 }},
    { GRC_TEAL,              { 0,    128,    128 }},
    { GRC_NAVY,              { 0,    0,      128 }},
    { GRC_BLUE,              { 0,    0,      150 }},
    { GRC_GREEN,             { 0,    150,    0   }},
    { GRC_RED,               { 150,  0,      0   }},
    { GRC_ORANGE_RED,        { 255,  69,     0   }},
    { GRC_ORANGE,            { 255,  165,    0   }},
    { GRC_KHAKI,             { 240,  230,    140 }},
    { GRC_BROWN,             { 165,  42,     42  }},
    { GRC_SADDLE_BROWN,      { 139,  69,     19  }},
    { GRC_BAKERS_CHOCOLATE,  { 92,   51,     23  }},
    { GRC_IVORY,             { 255,  255,    240 }},
    { GRC_SNOW,              { 255,  250,    250 }},
    { GRC_GHOST_WHITE,       { 248,  248,    255 }},
    { GRC_INDIGO,            { 75,   0,      130 }},
    { GRC_DARK_SLATE_BLUE,   { 72,   61,     139 }},
    { GRC_SKY_BLUE,          { 135,  206,    235 }},
    { GRC_SEA_GREEN,         { 46,   139,    87  }},
    { GRC_DARK_GREEN,        { 0,    100,    0   }},
    { GRC_FOREST_GREEN,      { 34,   139,    24  }},
    { GRC_GOLD,              { 255,  215,    0   }},
    { GRC_GOLDEN_ROD,        { 218,  165,    32  }},
    { GRC_SALMON,            { 250,  128,    114 }},
    { GRC_CORAL,             { 255,  127,    80  }},
    { GRC_TOMATO,            { 255,  99,     71  }},
    { GRC_DARK_SEA_GREEN,    { 143,  188,    143 }},
    { GRC_DARK_CYAN,         { 0,    139,    139 }},
    { GRC_TURQUOISE,         { 64,   224,    208 }},
    { GRC_DARK_TURQUOISE,    { 0,    206,    209 }},
    { GRC_CADET_BLUE,        { 95,   158,    160 }},
    { GRC_OLIVE_DRAB,        { 107,  142,    35  }},
    { GRC_STEEL_BLUE,        { 70,   130,    180 }},
    { GRC_DODGER_BLUE,       { 30,   144,    255 }},
    { GRC_MIDNIGHT_BLUE,     { 25,   25,     112 }},
    { GRC_SLATE_BLUE,        { 106,  90,     205 }},
    { GRC_SIENNA,            { 160,  82,     45  }},
    { GRC_PERU,              { 205,  133,    63  }},
    { GRC_TAN,               { 210,  180,    140 }},
    { GRC_SLATE_GRAY,        { 112,  128,    144 }},
    { GRC_DARK_KHAKI,        { 189,  183,    107 }},
    { GRC_INDIAN_RED,        { 205,  92,     92  }}
};

#define MAX_COLORS              \
    (sizeof(__colors) / sizeof(__colors[0]))

struct gfx_color_s *color_start(void)
{
    struct gfx_color_s *c = NULL;

    c = calloc(1, sizeof(struct gfx_color_s));

    if (NULL == c)
        return NULL;

    return c;
}

void color_finish(struct gfx_color_s *color)
{
    if (NULL == color)
        return;

    cstring_unref(color->s_fg);
    cstring_unref(color->s_bg);
    free(color);
}

int color_get(struct gfx_color_s *color, enum gfx_color type)
{
    if (NULL == color)
        return -1;

    switch (type) {
        case COLOR_FG:
            return color->fg;

        case COLOR_BG:
            return color->bg;
    }

    return -1;
}

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

int color_parse(struct grc_s *grc)
{
    cjson_t *jcolors;
    struct property_detail *dt;
    struct gfx_color_s *color;

    color = grc->color;
    jcolors = grc_get_object(grc, OBJ_COLORS);

    if (NULL == jcolors) {
        grc_set_errno(GRC_ERROR_COLORS_BLOCK_NOT_FOUND);
        return -1;
    }

    /* foreground */
    dt = get_property_detail(GRC_PROPERTY_FOREGROUND);

    if (NULL == dt)
        goto unknown_grc_key_block;

    color->s_fg = grc_get_object_str(jcolors, property_detail_string(dt));

    /* background */
    dt = get_property_detail(GRC_PROPERTY_BACKGROUND);

    if (NULL == dt)
        goto unknown_grc_key_block;

    color->s_bg = grc_get_object_str(jcolors, property_detail_string(dt));

    /*
     * Global colors. We don't call get_color_depth() from Allegro because
     * at this point it hasn't been initialized yet.
     */
    color->fg = color_grc_to_al(info_color_depth(grc),
                                cstring_valueof(color->s_fg));

    color->bg = color_grc_to_al(info_color_depth(grc),
                                cstring_valueof(color->s_bg));

    return 0;

unknown_grc_key_block:
    grc_set_errno(GRC_ERROR_UNDEFINED_GRC_KEY);
    return -1;
}

int color_get_global_fg(struct grc_s *grc)
{
    if (NULL == grc)
        return -1;

    return color_get(grc->color, COLOR_FG);
}

int color_get_global_bg(struct grc_s *grc)
{
    if (NULL == grc)
        return -1;

    return color_get(grc->color, COLOR_BG);
}

