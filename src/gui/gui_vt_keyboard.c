
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Mon Dec 22 11:43:11 2014
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

/*
 * Layout (1):
 *
 * Q W E R T Y U I O P
 * A S D F G H J K L
 * (shift) Z X C V B N M (backspace)
 * (change fmt) ?? (spacebar 5x) ?? (enter)
 *
 * Layout (2):
 *
 * 1! 2@ 3# 4$ 5% 6^ 7& 8* 9( 0)
 * -_ =+ [{ ]} \| ~` ;: '" ,<
 * (shift) .> /? ?? ?? ?? ?? ?? (enter)
 * (change fmt) ?? (spacebar 5x) ?? (enter)
 */

/* Total de linhas do teclado virtual */
#define TOTAL_LINES             4

/* Numero maximo de layouts de teclado para o usuario */
#define MAX_FORMATS             2

/* Quantidade de teclas de cada linha do teclado */
#define LINE1_NKEYS             10
#define LINE2_NKEYS             9
#define LINE3_NKEYS             9
#define LINE4_NKEYS             5

#define MAX_SCREEN_KEYS         LINE1_NKEYS

/*
 * Posicao relativa do inicio da primeira tecla com relacao ao proprio
 * objeto.
 */
#define RELATIVE_INITAL_POS     4

/* Special keys tokens */
#define SHIFT_TOK               "SHIFT"
#define CHANGE_FMT_TOK          "CHANGE_FMT"
#define BACKSPACE_TOK           "BACKSPACE"
#define SPACEBAR_TOK            "SPACEBAR"
#define ENTER_TOK               "ENTER"

enum special_key_value {
    SPK_SHIFT,
    SPK_CHANGE_FMT,
    SPK_BACKSPACE,
    SPK_SPACEBAR,
    SPK_ENTER
};

/* bitfield flags para o campo d2 do objeto */
#define DLG_SPK_SHIFT           1

struct special_key {
    char                    token[128];     /* identificacao interna */
    char                    user_key[16];   /* formato exibido para usuario */
    enum special_key_value  value;          /* valor interno */
};

struct key {
    char    key[16];
    int     scancode;
};

struct btn_key_area {
    int         x;
    int         y;
    int         w;
    int         h;
};

struct screen_key {
    struct key          key[2];
    struct btn_key_area area;
    enum al_flag        selected;
};

struct screen_line {
    int                 n;
    struct screen_key   keys[MAX_SCREEN_KEYS];
};

struct keyboard_layout {
    int                 format;
    struct screen_line  line[TOTAL_LINES];
};

static struct keyboard_layout __klayout[] = {
    /* formato 1 */
    {
        .format = 1,

        .line = {
            /* linha 1 */
            {
                LINE1_NKEYS,
                {
                    {.key = {{.key = "q", .scancode = KEY_Q },
                               {.key = "Q", .scancode = KEY_Q }}},
                    {.key = {{.key = "w", .scancode = KEY_W },
                               {.key = "W", .scancode = KEY_W }}},
                    {.key = {{.key = "e", .scancode = KEY_E },
                               {.key = "E", .scancode = KEY_E }}},
                    {.key = {{.key = "r", .scancode = KEY_R },
                               {.key = "R", .scancode = KEY_R }}},
                    {.key = {{.key = "t", .scancode = KEY_T },
                               {.key = "T", .scancode = KEY_T }}},
                    {.key = {{.key = "y", .scancode = KEY_Y },
                               {.key = "Y", .scancode = KEY_Y }}},
                    {.key = {{.key = "u", .scancode = KEY_U },
                               {.key = "U", .scancode = KEY_U }}},
                    {.key = {{.key = "i", .scancode = KEY_I },
                               {.key = "I", .scancode = KEY_I }}},
                    {.key = {{.key = "o", .scancode = KEY_O },
                               {.key = "O", .scancode = KEY_O }}},
                    {.key = {{.key = "p", .scancode = KEY_P },
                               {.key = "P", .scancode = KEY_P }}}
                }
            },

            /* linha 2 */
            {
                LINE2_NKEYS,
                {
                    {.key = {{.key = "a", .scancode = KEY_A },
                               {.key = "A", .scancode = KEY_A }}},
                    {.key = {{.key = "s", .scancode = KEY_S },
                               {.key = "S", .scancode = KEY_S }}},
                    {.key = {{.key = "d", .scancode = KEY_D },
                               {.key = "D", .scancode = KEY_D }}},
                    {.key = {{.key = "f", .scancode = KEY_F },
                               {.key = "F", .scancode = KEY_F }}},
                    {.key = {{.key = "g", .scancode = KEY_G },
                               {.key = "G", .scancode = KEY_G }}},
                    {.key = {{.key = "h", .scancode = KEY_H },
                               {.key = "H", .scancode = KEY_H }}},
                    {.key = {{.key = "j", .scancode = KEY_J },
                               {.key = "J", .scancode = KEY_J }}},
                    {.key = {{.key = "k", .scancode = KEY_K },
                               {.key = "K", .scancode = KEY_K }}},
                    {.key = {{.key = "l", .scancode = KEY_L },
                               {.key = "L", .scancode = KEY_L }}}
                }
            },

            /* linha 3 */
            {
                LINE3_NKEYS,
                {
                    {.key = {{.key = SHIFT_TOK, .scancode = KEY_LSHIFT },
                               {.key = SHIFT_TOK, .scancode = KEY_LSHIFT }}},
                    {.key = {{.key = "z", .scancode = KEY_Z },
                               {.key = "Z", .scancode = KEY_Z }}},
                    {.key = {{.key = "x", .scancode = KEY_X },
                               {.key = "X", .scancode = KEY_X }}},
                    {.key = {{.key = "c", .scancode = KEY_C },
                               {.key = "C", .scancode = KEY_C }}},
                    {.key = {{.key = "v", .scancode = KEY_V },
                               {.key = "V", .scancode = KEY_V }}},
                    {.key = {{.key = "b", .scancode = KEY_B },
                               {.key = "B", .scancode = KEY_B }}},
                    {.key = {{.key = "n", .scancode = KEY_N },
                               {.key = "N", .scancode = KEY_N }}},
                    {.key = {{.key = "m", .scancode = KEY_M },
                               {.key = "M", .scancode = KEY_M }}},
                    {.key = {{.key = BACKSPACE_TOK, .scancode = KEY_BACKSPACE },
                               {.key = BACKSPACE_TOK, .scancode = KEY_BACKSPACE }}}
                }
            },

            /* linha 4 */
            {
                LINE4_NKEYS,
                {
                    {.key = {{.key = CHANGE_FMT_TOK, .scancode = KEY_LWIN },
                               {.key = CHANGE_FMT_TOK, .scancode = KEY_LWIN }}},
                    {.key = {{.key = "", .scancode = KEY_UNKNOWN1 },
                               {.key = "", .scancode = KEY_UNKNOWN1 }}},
                    {.key = {{.key = SPACEBAR_TOK, .scancode = KEY_SPACE },
                               {.key = SPACEBAR_TOK, .scancode = KEY_SPACE }}},
                    {.key = {{.key = "", .scancode = KEY_UNKNOWN1 },
                               {.key = "", .scancode = KEY_UNKNOWN1 }}},
                    {.key = {{.key = ENTER_TOK, .scancode = KEY_ENTER },
                               {.key = ENTER_TOK, .scancode = KEY_ENTER }}}
                }
            },
        },
    },

    /* formato 2 */
    {
        .format = 2,

        .line = {
            /* linha 1 */
            {
                LINE1_NKEYS,
                {
                    {.key = {{.key = "1", .scancode = KEY_1 },
                               {.key = "!", .scancode = KEY_EXCLAMATION }}},
                    {.key = {{.key = "2", .scancode = KEY_2 },
                               {.key = "@", .scancode = KEY_AT }}},
                    {.key = {{.key = "3", .scancode = KEY_3 },
                               {.key = "#", .scancode = KEY_SHARP }}},
                    {.key = {{.key = "4", .scancode = KEY_4 },
                               {.key = "$", .scancode = KEY_DOLLAR }}},
                    {.key = {{.key = "5", .scancode = KEY_5 },
                               {.key = "%", .scancode = KEY_PERCENT }}},
                    {.key = {{.key = "6", .scancode = KEY_6 },
                               {.key = "^", .scancode = KEY_CIRCUMFLEX }}},
                    {.key = {{.key = "7", .scancode = KEY_7 },
                               {.key = "&", .scancode = KEY_AMPERSAND }}},
                    {.key = {{.key = "8", .scancode = KEY_8 },
                               {.key = "*", .scancode = KEY_ASTERISK }}},
                    {.key = {{.key = "9", .scancode = KEY_9 },
                               {.key = "(", .scancode = KEY_LPARENTHESIS }}},
                    {.key = {{.key = "0", .scancode = KEY_0 },
                               {.key = ")", .scancode = KEY_RPARENTHESIS }}}
                }
            },

            /* linha 2 */
            {
                LINE2_NKEYS,
                {
                    {.key = {{.key = "-", .scancode = KEY_MINUS },
                               {.key = "_", .scancode = KEY_UNDERLINE }}},
                    {.key = {{.key = "=", .scancode = KEY_EQUALS },
                               {.key = "+", .scancode = KEY_PLUS }}},
                    {.key = {{.key = "[", .scancode = KEY_LBRACKET },
                               {.key = "{", .scancode = KEY_OPENBRACE }}},
                    {.key = {{.key = "]", .scancode = KEY_RBRACKET },
                               {.key = "}", .scancode = KEY_CLOSEBRACE }}},
                    {.key = {{.key = "\\", .scancode = KEY_BACKSLASH },
                               {.key = "|", .scancode = KEY_PIPE }}},
                    {.key = {{.key = "`", .scancode = KEY_GRAVE_ACCENT },
                               {.key = "~", .scancode = KEY_TILDE }}},
                    {.key = {{.key = ";", .scancode = KEY_SEMICOLON },
                               {.key = ":", .scancode = KEY_COLON }}},
                    {.key = {{.key = "'", .scancode = KEY_QUOTE },
                               {.key = "\"", .scancode = KEY_DOUBLE_QUOTE }}},
                    {.key = {{.key = ",", .scancode = KEY_COMMA },
                               {.key = "<", .scancode = KEY_LESS }}}
                }
            },

            /* linha 3 */
            {
                LINE3_NKEYS,
                {
                    {.key = {{.key = SHIFT_TOK, .scancode = KEY_LSHIFT },
                               {.key = SHIFT_TOK, .scancode = KEY_LSHIFT }}},
                    {.key = {{.key = ".", .scancode = KEY_STOP },
                               {.key = ">", .scancode = KEY_GREATER }}},
                    {.key = {{.key = "/", .scancode = KEY_SLASH },
                               {.key = "?", .scancode = KEY_INTERROGATION }}},
                    {.key = {{.key = "", .scancode = KEY_UNKNOWN1 },
                               {.key = "", .scancode = KEY_UNKNOWN1 }}},
                    {.key = {{.key = "", .scancode = KEY_UNKNOWN1 },
                               {.key = "", .scancode = KEY_UNKNOWN1 }}},
                    {.key = {{.key = "", .scancode = KEY_UNKNOWN1 },
                               {.key = "", .scancode = KEY_UNKNOWN1 }}},
                    {.key = {{.key = "", .scancode = KEY_UNKNOWN1 },
                               {.key = "", .scancode = KEY_UNKNOWN1 }}},
                    {.key = {{.key = "", .scancode = KEY_UNKNOWN1 },
                               {.key = "", .scancode = KEY_UNKNOWN1 }}},
                    {.key = {{.key = BACKSPACE_TOK, .scancode = KEY_BACKSPACE },
                               {.key = BACKSPACE_TOK, .scancode = KEY_BACKSPACE }}}
                }
            },

            /* linha 4 */
            {
                LINE4_NKEYS,
                {
                    {.key = {{.key = CHANGE_FMT_TOK, .scancode = KEY_LWIN },
                               {.key = CHANGE_FMT_TOK, .scancode = KEY_LWIN }}},
                    {.key = {{.key = "", .scancode = KEY_UNKNOWN1 },
                               {.key = "", .scancode = KEY_UNKNOWN1 }}},
                    {.key = {{.key = SPACEBAR_TOK, .scancode = KEY_SPACE },
                               {.key = SPACEBAR_TOK, .scancode = KEY_SPACE }}},
                    {.key = {{.key = "", .scancode = KEY_UNKNOWN1 },
                               {.key = "", .scancode = KEY_UNKNOWN1 }}},
                    {.key = {{.key = ENTER_TOK, .scancode = KEY_ENTER },
                               {.key = ENTER_TOK, .scancode = KEY_ENTER }}}
                }
            },
        },
    }
};

/* Teclas especiais do teclado virtual */
static struct special_key __special_keys[] = {
    { SHIFT_TOK,        "S",        SPK_SHIFT       },
    { CHANGE_FMT_TOK,   "12#",      SPK_CHANGE_FMT  },
    { BACKSPACE_TOK,    "B",        SPK_BACKSPACE   },
    { SPACEBAR_TOK,     "ESPACO",   SPK_SPACEBAR    },
    { ENTER_TOK,        "E",        SPK_ENTER       }
};

#define TOTAL_SPECIAL_KEYS      \
    sizeof(__special_keys) / sizeof(__special_keys[0])

static struct special_key *search_special_key(const char *btn_text)
{
    unsigned int i;

    for (i = 0; i < TOTAL_SPECIAL_KEYS; i++)
        if (!strcmp(__special_keys[i].token, btn_text))
            return &__special_keys[i];

    return NULL;
}

static const char *translate_special_key(const char *btn_text)
{
    struct special_key *spk;

    spk = search_special_key(btn_text);

    if (NULL == spk)
        return NULL;

    return spk->user_key;
}

static void calc_buttons_area(DIALOG *d, int keyb_layout)
{
    int i, j, x, y, w, btn_height;
    struct keyboard_layout *k = &__klayout[keyb_layout];
    struct screen_key *sk;
    struct special_key *spk=NULL;

    btn_height = d->h / TOTAL_LINES;
    w = (d->w / MAX_SCREEN_KEYS) - 2;

    for (i = 0; i < TOTAL_LINES; i++) {
        x = d->x + 1 + RELATIVE_INITAL_POS;

        /* Centraliza as linhas seguintes por possuirem menos botoes */
        if (i > 0)
            x += (w / 2) + 1;

        for (j = 0, y = (btn_height * i) + d->y + 1;
             j < k->line[i].n;
             j++, x += sk->area.w + 1)
        {
            sk = &k->line[i].keys[j];
            spk = search_special_key(sk->key[0].key);

            sk->area.x = x;
            sk->area.y = y;
            sk->area.w = w;

            /* O botao da barra de status ocupa o lugar de 3 botoes normais */
            if ((spk != NULL) && (spk->value == SPK_SPACEBAR)) {
                sk->area.w *= 5;
                sk->area.w += 4;
            }

            sk->area.h = btn_height;
        }
    }
}

static void draw_special_key(struct screen_key *key, struct special_key *spk,
    int fg, int bg)
{
    int x1, y1, x2, y2;
    BITMAP *b, *gui_bmp;

    gui_bmp = gui_get_screen();
    b = create_bitmap_ex(get_color_depth(), key->area.w - 1, key->area.h - 1);
    clear_to_color(b, bg);

    switch (spk->value) {
        case SPK_SHIFT:
            /*
             * Deveria sair desenhado algo como, proporcional ao tamanho do
             * botao:
             *
             *    /|\
             *   / | \
             *     |
             *     |
             */

            /* -- 1 -- */
            x1 = key->area.w / 2;
            y1 = key->area.h / 4;
            y2 = y1 + (key->area.h / 2);
            vline(b, x1, y1, y2, fg);

            /* -- 2 -- */
            x1 = key->area.w / 2;
            y1 = key->area.h / 4;
            x2 = key->area.w / 4;
            y2 = y1 + (key->area.h / 4);
            line(b, x1, y1, x2, y2, fg);

            x2 = key->area.w - (key->area.w / 4);
            y2 += 1;
            line(b, x1, y1, x2, y2, fg);

            blit(b, gui_bmp, 0, 0, key->area.x + 1, key->area.y + 1, b->w, b->h);
            break;

        case SPK_ENTER:
            /*
             * Deveria sair desenhado algo como, proporcional ao tamanho do
             * botao:
             *
             *                  |
             *      /           |
             *      -------------
             *      \
             *
             */
            /* -- 1 -- */
            x1 = key->area.w / 4;
            x2 = key->area.w - x1;
            y1 = key->area.h / 2;
            hline(b, x1, y1, x2, fg);

            /* -- 2 -- */
            x1 = key->area.w / 2;
            y1 = key->area.h / 4;
            x2 = key->area.w / 4;
            y2 = key->area.h / 2;
            line(b, x1, y1, x2, y2, fg);

            x1 = key->area.w / 2;
            y1 = key->area.h - (key->area.h / 4);
            x2 = key->area.w / 4;
            y2 = key->area.h / 2;
            line(b, x1, y1, x2, y2, fg);

            /* -- 3 -- */
            x1 = key->area.w - (key->area.w / 4);
            y1 = key->area.h / 4;
            y2 = key->area.h / 2;
            vline(b, x1, y1, y2, fg);

            blit(b, gui_bmp, 0, 0, key->area.x + 1, key->area.y + 1, b->w, b->h);
            break;

        case SPK_BACKSPACE:
            /*
             * Deveria sair desenhado algo como, proporcional ao tamanho do
             * botao:
             *
             *       /
             *      /________
             *      \
             *       \
             */
            /* -- 1 -- */
            x1 = key->area.w / 4;
            x2 = key->area.w - x1;
            y1 = key->area.h / 2;
            hline(b, x1, y1, x2, fg);

            /* -- 2 -- */
            x1 = key->area.w / 2;
            y1 = key->area.h / 4;
            x2 = key->area.w / 4;
            y2 = key->area.h / 2;
            line(b, x1, y1, x2, y2, fg);

            x1 = key->area.w / 2;
            y1 = key->area.h - (key->area.h / 4);
            x2 = key->area.w / 4;
            y2 = key->area.h / 2;
            line(b, x1, y1, x2, y2, fg);

            blit(b, gui_bmp, 0, 0, key->area.x + 1, key->area.y + 1, b->w, b->h);
            break;

        default:
            break;
    }

    destroy_bitmap(b);
}

static void draw_keyboard(DIALOG *d, int keyb_layout)
{
    int i, btn_height, j, half_letter = text_height(font) / 2, fg, bg;
    struct keyboard_layout *k = &__klayout[keyb_layout];
    struct screen_key *sk, *dotted_key=NULL;
    struct special_key *spk;
    char *btn_text;
    BITMAP *gui_bmp;

    gui_bmp = gui_get_screen();

    /* Fundo e moldura principal */
    rectfill(gui_bmp, d->x, d->y, d->x + d->w - 1, d->y + d->h - 1, d->bg);
    rect(gui_bmp, d->x, d->y, d->x + d->w - 1, d->y + d->h - 1, d->fg);

    /* Desenha teclas */
    btn_height = d->h / TOTAL_LINES;

    /* linhas */
    for (i = 0; i < TOTAL_LINES; i++) {
        for (j = 0; j < k->line[i].n; j++) {
            fg = d->fg;
            bg = d->bg;
            sk = &k->line[i].keys[j];

            /* Inverte as cores do botao que foi clicado */
            if (sk->selected == AL_TRUE) {
                fg = d->bg;
                bg = d->fg;
                sk->selected = AL_FALSE;
                dotted_key = sk;
            }

            rectfill(gui_bmp, sk->area.x, sk->area.y, sk->area.x + sk->area.w,
                     sk->area.y + btn_height, bg);

            rect(gui_bmp, sk->area.x, sk->area.y, sk->area.x + sk->area.w,
                 sk->area.y + btn_height, fg);

            btn_text = sk->key[d->d2 & DLG_SPK_SHIFT].key;
            spk = search_special_key(btn_text);

            if ((NULL == spk) || (spk->value == SPK_SPACEBAR) ||
                (spk->value == SPK_CHANGE_FMT))
            {
                if (strlen(btn_text) > 1)
                    btn_text = (char *)translate_special_key(btn_text);

                textout_centre_ex(gui_bmp, font, btn_text,
                                  sk->area.x + (sk->area.w / 2) + 1,
                                  sk->area.y + (btn_height / 2) - half_letter,
                                  fg, bg);
            } else
                draw_special_key(sk, spk, fg, bg);
        }
    }

    /* Coloca uma borda de efeito no botao que acabou de ser pressionado */
    if ((d->flags & D_GOTFOCUS) &&
        (!(d->flags & D_SELECTED) || !(d->flags & D_EXIT)) &&
        (dotted_key != NULL))
    {
        dotted_rect(dotted_key->area.x, dotted_key->area.y,
                    dotted_key->area.x + dotted_key->area.w,
                    dotted_key->area.y + dotted_key->area.h,
                    d->fg, d->bg);
    }
}

static struct screen_key *find_clicked_button(int x, int y, int keyb_layout)
{
    struct screen_key *key=NULL, *sk;
    struct keyboard_layout *k = &__klayout[keyb_layout];
    int i, j;

    for (i = 0; i < TOTAL_LINES; i++) {
        for (j = 0; j < k->line[i].n; j++) {
            sk = &k->line[i].keys[j];

            if ((x >= sk->area.x) && (y >= sk->area.y) &&
                (x < (sk->area.x + sk->area.w)) &&
                (y < (sk->area.y + sk->area.h)))
            {
                key = sk;
                return key;
            }
        }
    }

    return NULL;
}

static char *add_char(const char *s, unsigned int pos, char *c)
{
    size_t l;
    cstring_t *tmp = NULL;
    char *p = NULL;

    l = strlen(s);

    /* Esta inserindo no final? */
    if (pos == l) {
        tmp = cstring_create("%s", s);
        cstring_cat(tmp, "%s", c);
        p = strdup(cstring_valueof(tmp));
        cstring_unref(tmp);

        return p;
    }

    p = strdup(s);
    p[pos] = '\0';

    tmp = cstring_create("%s%s%s", p, c, s + pos);
    free(p);
    p = strdup(cstring_valueof(tmp));
    cstring_unref(tmp);

    return p;
}

static void run_edit_callback(DIALOG *edit)
{
    struct al_callback_data *acd = edit->dp3;

    if (NULL == acd)
        return;

    acd->value_string = (char *)edit->dp;
    run_callback(acd);
}

static int update_last_edit_object_value(DIALOG *d, struct al_grc *grc,
    struct screen_key *key)
{
    struct special_key *spk=NULL;
    char *tmp=NULL, *ed_value;
    DIALOG *edit;
    unsigned int ed_pos=0;

    if (NULL == grc->last_edit_object)
        return D_O_K;

    /* Obtem o objeto 'edit' que sera manipulado */
    edit = grc->last_edit_object;

    /* Trata teclas especiais */
    spk = search_special_key(key->key[d->d2 & DLG_SPK_SHIFT].key);

    if (spk != NULL) {
        switch (spk->value) {
            case SPK_CHANGE_FMT:
                if (d->d1 == GRC_KLAYOUT_LETTERS)
                    d->d1 = GRC_KLAYOUT_NUMBERS;
                else if (d->d1 == GRC_KLAYOUT_NUMBERS)
                    d->d1 = GRC_KLAYOUT_LETTERS;

                return D_REDRAWME;

            case SPK_SHIFT:
                if (d->d2 & DLG_SPK_SHIFT)
                    d->d2 &= ~DLG_SPK_SHIFT;
                else
                    d->d2 |= DLG_SPK_SHIFT;

                return D_REDRAWME;

            case SPK_ENTER:
                /*
                 * Chama funcao de callback do objeto 'edit' que esta sendo
                 * manipulado.
                 */
                run_edit_callback(edit);
                return D_O_K;

            default:
                break;
        }
    }

    ed_value = edit->dp;

    /* Nao faz nada caso atinja o limite da string do objeto */
    if (strlen(ed_value) >= (size_t)edit->d1)
        return D_O_K;

    if (spk != NULL) {
        tmp = strdup(ed_value);

        switch (spk->value) {
            case SPK_BACKSPACE:
                /* Nao faz nada com o cursor na posicao inicial da string */
                if (edit->d2 == 0)
                    return D_O_K;

                /* FIXME: We need to rewrite this... */
                {
                    cstring_t *__p = cstring_create("%s", tmp);
                    cstring_idchr(__p, edit->d2 - 1);
                    free(tmp);
                    tmp = strdup(cstring_valueof(__p));
                }

                ed_pos = edit->d2 - 1;
                break;

            case SPK_SPACEBAR:
                tmp = add_char(ed_value, edit->d2, " ");
                ed_pos = edit->d2 + 1;
                break;

            default:
                break;
        }
    } else {
        tmp = add_char(ed_value, edit->d2,
                       key->key[d->d2 & DLG_SPK_SHIFT].key);

        ed_pos = edit->d2 + 1;
    }

    strcpy(ed_value, tmp);
    free(tmp);

    /* Atualiza o cursor, para fazer o scroll correto na linha editada */
    object_message(edit, MSG_UPDATE_CURSOR_POSITION, ed_pos);

    /* Atualiza o conteudo do objeto para o usuario */
    object_message(edit, MSG_DRAW, 0);

    return D_O_K;
}

/*
 * d1 - Layout do teclado
 * d2 - Flags internos (shift key, ...)
 * dp3 - callback padrao
 */
int gui_d_vt_keyboard_proc(int msg, DIALOG *d, int c __attribute__((unused)))
{
    struct screen_key *key;
    struct al_callback_data *acd = d->dp3;
    struct al_grc *grc = acd->grc;
    enum grc_keyboard_layout klayout = d->d1;
    int mx, my, ret, draw=AL_FALSE;

    switch (msg) {
        case MSG_START:
            /*
             * Calcula a area de cada botao para facilitar a validacao do
             * clique do mouse.
             */
            calc_buttons_area(d, GRC_KLAYOUT_LETTERS);
            calc_buttons_area(d, GRC_KLAYOUT_NUMBERS);
            break;

        case MSG_DRAW:
            draw_keyboard(d, klayout);
            break;

        case MSG_WANTFOCUS:
            return D_WANTFOCUS;

        case MSG_CLICK:
            mx = gui_mouse_x();
            my = gui_mouse_y();
            key = find_clicked_button(mx, my, klayout);

            while (gui_mouse_b()) {
                /* Precisa redesenhar o botao clicado */
                if ((key != NULL) && (draw == AL_FALSE)) {
                    key->selected = AL_TRUE;
                    object_message(d, MSG_DRAW, 0);
                    draw = AL_TRUE;
                }

                /* Permite continuar desenhando os outros objetos */
                broadcast_dialog_message(MSG_IDLE, 0);
            }

            if (key != NULL) {
                ret = update_last_edit_object_value(d, grc, key);

                if (ret != D_O_K)
                    return ret;

                /*
                 * Precisa redesenhar para dar a impressao de que um botao
                 * foi clicado.
                 */
                object_message(d, MSG_DRAW, 0);

                /* Disponibiliza qual tecla foi ativada para o usuario */
                acd->value_int = key->key[d->d2 & DLG_SPK_SHIFT].scancode;

                /* Executa funcao de callback do objeto */
                run_callback(acd);
            }

            break;
    }

    return D_O_K;
}

