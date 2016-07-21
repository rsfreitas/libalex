
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Sun Dec 14 00:29:57 2014
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
 * Para utilizar corretamente este objeto o usuario precisa criar uma funcao
 * do tipo:
 *
 * char *foo(int index, int *list_size);
 *
 * sendo que, quando index for >= 0, a funcao deve retornar um ponteiro
 * para a string que deve ser exibida na lista na posicao index. Caso
 * contrario, deve retornar NULL e *list_size deve conter o tamanho total
 * da lista de itens.
 *
 * Esta funcao deve ficar em d->dp.
 */
int gui_d_list_proc(int msg, DIALOG *d, int c)
{
    struct al_callback_data *acd = d->dp3;
    int ret;

    ret = d_list_proc(msg, d, c);

    if (ret == D_CLOSE) {
        if (d->dp3 != NULL) {
            acd->value_int = d->d1;
            run_callback(acd);

            /*
             * Nao deixa fechar a interface, caso o usuario esqueca o retorno
             * correto.
             */
            ret = D_O_K;
        } else
            return D_REDRAWME;
    }

    return ret;
}

