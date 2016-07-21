
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Wed Dec 10 17:18:41 2014
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

static const char *__error_descriptions[] = {
    "Ok",
    "Argumento invalido",
    "Falha ao alocar memoria interna",
    "Falha ao carregar dados GRC",
    "Objeto nao definido internamente",
    "Bloco de informacoes do ambiente grafico nao encontrado",
    "Bloco de cores do ambiente grafico nao encontrado",
    "Falha ao ajustar a resolucao de tela",
    "Falha ao inicializar biblioteca Allegro",
    "Falha ao inicializar o teclado",
    "Bloco de objetos do ambiente grafico nao encontrado",
    "Nenhum objeto foi encontrado",
    "Falha ao criar referencia interna do objeto",
    "Nenhuma tecla foi encontrada",
    "Objeto nao encontrado",
    "Objeto desconhecido",
    "Modo de inicializacao da biblioteca invalido",
    "Quantidade de caracteres nao suportada pelo objeto",
    "Cores nao suportadas",
    "Quantidade de cores nao suportada",
    "Propriedade desconhecida",
    "Tipo de dado nao suportado"
};

static int __al_errno;
static const char *__unknown_error = "Erro desconhecido";

void al_errno_clear(void)
{
    __al_errno = AL_NO_ERROR;
}

void al_set_errno(enum al_error_code code)
{
    __al_errno = code;
}

/**
 * @name al_get_last_error
 * @brief Obtem o codigo de erro atual da biblioteca.
 *
 * @return Retorna o codigo de erro atual da biblioteca.
 */
enum al_error_code LIBEXPORT al_get_last_error(void)
{
    return __al_errno;
}

/**
 * @name al_strerror
 * @brief Obtem uma descricao a respeito de um codigo de erro.
 *
 * @param [in] code: Codigo de erro.
 *
 * @return Retorna uma string contendo a descricao do codigo de erro informado.
 */
const char LIBEXPORT *al_strerror(enum al_error_code code)
{
    if (code >= AL_MAX_ERROR_CODE)
        return __unknown_error;

    return __error_descriptions[code];
}

