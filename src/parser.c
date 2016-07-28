
/*
 * Description: Functions to parse an GRC UI file.
 *
 * Author: Rodrigo Freitas
 * Created at: Tue Dec  9 22:42:05 2014
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

#include <stdlib.h>
#include <string.h>

#include "libalex.h"

static struct grc_json_key __entries[] = {
    { OBJ_WIDTH,                AL_GRC_JOBJ_WIDTH,              GRC_NUMBER  },
    { OBJ_HEIGHT,               AL_GRC_JOBJ_HEIGHT,             GRC_NUMBER  },
    { OBJ_COLOR_DEPTH,          AL_GRC_JOBJ_COLOR_DEPTH,        GRC_NUMBER  },
    { OBJ_FOREGROUND,           AL_GRC_JOBJ_FOREGROUND,         GRC_STRING  },
    { OBJ_BACKGROUND,           AL_GRC_JOBJ_BACKGROUND,         GRC_STRING  },
    { OBJ_BLOCK_EXIT_KEYS,      AL_GRC_JOBJ_BLOCK_EXIT_KEYS,    GRC_BOOL    },
    { OBJ_MOUSE,                AL_GRC_JOBJ_MOUSE,              GRC_BOOL    },
    { OBJ_TYPE,                 AL_GRC_JOBJ_TYPE,               GRC_STRING  },
    { OBJ_POS_X,                AL_GRC_JOBJ_POS_X,              GRC_NUMBER  },
    { OBJ_POS_Y,                AL_GRC_JOBJ_POS_Y,              GRC_NUMBER  },
    { OBJ_NAME,                 AL_GRC_JOBJ_NAME,               GRC_STRING  },
    { OBJ_PARENT,               AL_GRC_JOBJ_PARENT,             GRC_STRING  },
    { OBJ_KEY,                  AL_GRC_JOBJ_KEY,                GRC_STRING  },
    { OBJ_TEXT,                 AL_GRC_JOBJ_TEXT,               GRC_STRING  },
    { OBJ_HIDE,                 AL_GRC_JOBJ_HIDE,               GRC_BOOL    },
    { OBJ_LINE_BREAK,           AL_GRC_JOBJ_LINE_BREAK,         GRC_STRING  },
    { OBJ_IGNORE_ESC_KEY,       AL_GRC_JOBJ_IGNORE_ESC_KEY,     GRC_BOOL    },
    { OBJ_INPUT_LENGTH,         AL_GRC_JOBJ_INPUT_LENGTH,       GRC_NUMBER  },
    { OBJ_RADIO_GROUP,          AL_GRC_JOBJ_RADIO_GROUP,        GRC_NUMBER  },
    { OBJ_RADIO_TYPE,           AL_GRC_JOBJ_RADIO_TYPE,         GRC_STRING  },
    { OBJ_PASSWORD,             AL_GRC_JOBJ_PASSWORD_MODE,      GRC_BOOL    },
    { OBJ_HORIZONTAL_POSITION,  AL_GRC_JOBJ_H_POSITION,         GRC_STRING  },
    { OBJ_FPS,                  AL_GRC_JOBJ_FPS,                GRC_NUMBER  },
    { OBJ_DEVICES,              AL_GRC_JOBJ_DEVICES,            GRC_NUMBER  }
};

#define MAX_GRC_ENTRIES             \
    (sizeof(__entries) / sizeof(__entries[0]))

/*
 * ------- GRC object handling -------
 */

/*
 * Search for an information structure from a JSON object in the supported
 * objects list.
 */
struct grc_json_key *get_grc_json_key(enum al_grc_object_property prop)
{
    unsigned int i;
    struct grc_json_key *e = NULL;

    for (i = 0; i < MAX_GRC_ENTRIES; i++)
        if (__entries[i].prop == prop) {
            e = &__entries[i];
            break;
        }

    return e;
}

/*
 * THE parse ;-) (from a file)
 */
int grc_parse_file(struct al_grc *grc, const char *grc_filename)
{
    grc->jgrc = cjson_read_file(grc_filename);

    if (NULL == grc->jgrc)
        return -1;

    return 0;
}

/*
 * THE parse ;-) (from a buffer)
 */
int grc_parse_mem(struct al_grc *grc, const char *data)
{
    grc->jgrc = cjson_parse(data);

    if (NULL == grc->jgrc)
        return -1;

    return 0;
}

/*
 * Get the object value from within a GRC file, expecting to be a GRC_NUMBER
 * or a GRC_BOOL object.
 */
int grc_get_object_value(cjson_t *object, const char *object_name,
    int default_value)
{
    cjson_t *n = NULL;
    enum cjson_type type;
    cstring_t *s = NULL;
    int v = 0;

    n = cjson_get_object_item(object, object_name);

    if (NULL == n)
        return default_value;

    type = cjson_get_object_type(n);

    if (type == CJSON_NUMBER) {
        s = cjson_get_object_value(n);
        v = cstring_to_int(s);
        cstring_unref(s);

        return v;
    }

    return (type == CJSON_TRUE ? true : false);
}

/*
 * Get the object value from within a GRC file, expecting to be a GRC_STRING
 * object.
 */
char *grc_get_object_str(cjson_t *object, const char *object_name)
{
    cjson_t *n = NULL;
    cstring_t *s = NULL;
    char *p = NULL;

    n = cjson_get_object_item(object, object_name);

    if (NULL == n)
        return NULL;

    s = cjson_get_object_value(n);
    p = strdup(cstring_valueof(s));
    cstring_unref(s);

    return p;
}

