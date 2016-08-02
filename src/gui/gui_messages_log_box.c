
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Wed Dec 10 15:58:59 2014
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

#include <string.h>

#include <pthread.h>

#include "libalex.h"

#define MAX_LINE_CHARS      256
#define WIDEST_CHAR         "W"

struct line {
    struct line *next;
    char        s[MAX_LINE_CHARS];
    int         fg;
};

struct messages {
    unsigned int    c; /* total columns */
    unsigned int    l; /* total lines */
    struct line     *lines;
    char            **tmp;
};

static struct messages __mlog;
static pthread_mutex_t __m_lines = PTHREAD_MUTEX_INITIALIZER;

static struct line *new_line(const char *msg, int fg_color, const char *color)
{
    struct line *l = NULL;

    l = calloc(1, sizeof(struct line));

    if (NULL == l)
        return NULL;

    strcpy(l->s, msg);

    if (NULL == color)
        l->fg = fg_color;
    else
        l->fg = color_grc_to_al(get_color_depth(), color);

    l->next = NULL;

    return l;
}

static void init_messages(DIALOG *d)
{
    unsigned int i;

    __mlog.c = (d->w - d->x) / text_length(font, WIDEST_CHAR);
    __mlog.l = d->h / (text_height(font) + 2);

    __mlog.lines = NULL;

    /* This buffer is for new messages... */
    __mlog.tmp = calloc(__mlog.l + 1, sizeof(char *));

    for (i = 0; i < __mlog.l; i++)
        __mlog.tmp[i] = calloc(MAX_LINE_CHARS + 1, sizeof(char));
}

static void uninit_messages(void)
{
    unsigned int i;

    for (i = 0; i < __mlog.l; i++)
        free(__mlog.tmp[i]);

    free(__mlog.tmp);
    cdll_free(__mlog.lines, NULL);
}

static void print_messages(DIALOG *d)
{
    int y = d->y;
    struct line *p;

    for (p = __mlog.lines; p; p = p->next, y += text_height(font) + 2) {
        textprintf_ex(gui_get_screen(), font, d->x, y, d->bg, d->bg, "%*c",
                      __mlog.c, ' ');

        textprintf_ex(gui_get_screen(), font, d->x, y, p->fg, d->bg, "%s", p->s);
    }
}

static void clear_messages(DIALOG *d)
{
    unsigned int i;
    int y;

    pthread_mutex_lock(&__m_lines);

    for (i = 0, y = d->y; i < __mlog.l; i++, y += text_height(font) + 2) {
        textprintf_ex(gui_get_screen(), font, d->x, y, d->bg, d->bg, "%*c",
                      __mlog.c, ' ');
    }

    cdll_free(__mlog.lines, NULL);
    __mlog.lines = NULL;

    pthread_mutex_unlock(&__m_lines);
}

static int split_line(enum al_grc_line_break lbreak, const char *msg)
{
    size_t l;
    int i, exact_l = 0, splitted = 0, p = 0, r;

    l = strlen(msg);
    exact_l = l / __mlog.c;

    for (i = 0; i < exact_l; i++, splitted++) {
        if ((unsigned int)splitted >= __mlog.l)
            break;

        memset(__mlog.tmp[i], 0, MAX_LINE_CHARS);
        strncpy(__mlog.tmp[i], msg + p, __mlog.c);

        /*
         * To make this way of breaking the columns really efficient, we
         * cannot have words bigger than the maximum supported columns,
         * because in this case we will not be able to break it.
         */
        if (lbreak == AL_GRC_LBREAK_SMART) {
            int n, x;
            char *s;

            s = strrchr(__mlog.tmp[i], ' ');

            if (s) {
                n = strlen(s);
                x = strlen(__mlog.tmp[i]);
                __mlog.tmp[i][x - n] = '\0';
                p++;
            }
        }

        p += strlen(__mlog.tmp[i]);
    }

    r = l - p;

    if (r > 0) {
        memset(__mlog.tmp[i], 0, MAX_LINE_CHARS);
        strncpy(__mlog.tmp[i], msg + p, r);
        splitted++;
    }

    return splitted;
}

void gui_messages_set(enum al_grc_line_break lbreak, int fg_color,
    const char *msg, const char *color)
{
    size_t l;
    unsigned int msg_lines = 0, i = 0;
    int rm_lines = 0;
    struct line *line;

    pthread_mutex_lock(&__m_lines);

    l = strlen(msg);

    if (l > __mlog.c) {
        /*
         * We need to split the line because it exceeded the object column
         * limit.
         */
        msg_lines = split_line(lbreak, msg);
    } else {
        msg_lines = 1;
        strcpy(__mlog.tmp[0], msg);
        __mlog.tmp[0][l] = '\0';
    }

    /* Remove lines that reach the limit */
    rm_lines = (cdll_size(__mlog.lines) + msg_lines) - __mlog.l;

    if (rm_lines > 0) {
        for (i = 0; i < (unsigned int)rm_lines; i++) {
            line = cdll_pop(&__mlog.lines);
            free(line);
        }
    }

    /* Add new lines */
    for (i = 0; i < msg_lines; i++) {
        line = new_line(__mlog.tmp[i], fg_color, color);
        __mlog.lines = cdll_unshift(__mlog.lines, line);
    }

    pthread_mutex_unlock(&__m_lines);
}

int gui_messages_log_proc(int msg, DIALOG *d, int c __attribute__((unused)))
{
    switch (msg) {
        case MSG_START:
            init_messages(d);
            break;

        case MSG_END:
            uninit_messages();
            break;

        case MSG_NEW_LOG_TEXT:
            /*
             * Message indicating that there is a new message to be processed
             * and displayed.
             */
            print_messages(d);
            break;

        case MSG_CLEAR_LOG_TEXT:
            clear_messages(d);
            break;
    }

    return D_O_K;
}

