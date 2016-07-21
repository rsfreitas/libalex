
/*
 * Description: Funcoes para controle do objeto de exibicao do
 *              relogio atual do equipamento.
 *
 * Author: Rodrigo Freitas
 * Created at: Tue Dec  9 22:42:19 2014
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
 *
 * Importante
 * ----------
 *
 *  Nao deve-se utilizar mais do que um objeto deste por DIALOG.
 */

#include <time.h>

#include "libalex.h"

/*
 * Funcao para construir um objeto do tipo relogio na tela. O relogio e do tipo
 * digital exibindo data e hora, com atualizacao a cada segundo.
 */
int gui_clock_proc(int msg, DIALOG *d, int c)
{
    struct al_callback_data *acd = d->dp3;
    struct al_grc *grc = NULL;
    int ret;
    struct tm *t;
    time_t tp;

    grc = acd->grc;
    ret = d_text_proc(msg, d, c);

    switch (msg) {
        case MSG_START:
            time(&tp);
            localtime_r(&tp, &grc->dlg_tm);
            memset(grc->dlg_clock_str, 0, sizeof(grc->dlg_clock_str));
            break;

        case MSG_IDLE:
            time(&tp);
            t = localtime(&tp);

           if ((grc->dlg_tm.tm_sec != t->tm_sec) ||
               (grc->dlg_tm.tm_min != t->tm_min) ||
               (grc->dlg_tm.tm_hour != t->tm_hour))
            {
                grc->dlg_tm = *t;
                object_message(d, MSG_DRAW, 0);
            }

            break;

        case MSG_DRAW:
            sprintf(grc->dlg_clock_str, "%02d/%02d/%04d %02d:%02d:%02d",
                    grc->dlg_tm.tm_mday, grc->dlg_tm.tm_mon + 1,
                    grc->dlg_tm.tm_year + 1900, grc->dlg_tm.tm_hour,
                    grc->dlg_tm.tm_min, grc->dlg_tm.tm_sec);

            break;
    }

    return ret;
}

