
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Dec 18 15:59:14 2014
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

#ifndef _LIBALEX_WRITE_H
#define _LIBALEX_WRITE_H			1

#ifndef LIBALEX_COMPILE
# ifndef _LIBALEX_H
#  error "Never use <write.h> directly; include <libalex.h> instead."
# endif
#endif

/**
 * @name al_grc_GRC_create_colors
 * @brief Cria o objeto com as cores principais do DIALOG.
 *
 * @param [in,out] grc: Estrutura de um arquivo GRC previamente inicializada.
 * @param [in] foreground: String contendo a cor do primeiro plano.
 * @param [in] background: String contendo a cor de fundo.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int al_grc_GRC_create_colors(struct al_grc *grc, const char *foreground,
                             const char *background);

/**
 * @name al_grc_GRC_create_info
 * @brief Cria o objeto com as informacoes principais do DIALOG.
 *
 * @param [in,out] grc: Estrutura de um arquivo GRC previamente inicializada.
 * @param [in] width: Largura da janela.
 * @param [in] height: Altura da janela.
 * @param [in] color: Quantidade de cores da janela.
 * @param [in] block_exit_keys: Flag indicando o bloqueio ou nao das teclas
 *                              ctrl+alt+end.
 * @param [in] mouse: Flag indicando ou nao o uso do mouse.
 * @param [in] ignore_esc_key: Flag indicando ou nao se a tecla ESC sera
 *                             ignorada.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int al_grc_GRC_create_info(struct al_grc *grc, unsigned int width,
                           unsigned int height, enum al_grc_color_depth color,
                           enum al_flag block_exit_keys, enum al_flag mouse,
                           enum al_flag ignore_esc_key);

/**
 * @name al_grc_GRC_keys_start
 * @brief Inicializa o objeto de teclas suportadas pelo DIALOG.
 *
 * @param [in,out] grc: Estrutura de um arquivo GRC previamente inicializada.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int al_grc_GRC_keys_start(struct al_grc *grc);

/**
 * @name al_grc_GRC_add_key
 * @brief Adiciona uma tecla no objeto principal de teclas do DIALOG.
 *
 * @param [in,out] grc: Estrutura de um arquivo GRC previamente inicializada.
 * @param [in] key: Tecla que sera suportada pelo DIALOG. Deve ser uma string
 *                  no mesmo padrao do scancode da Allegro. Exemplo: KEY_A.
 * @param [in] name: Nome de identificacao do objeto para o usuario.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int al_grc_GRC_add_key(struct al_grc *grc, const char *key, const char *name);

/**
 * @name al_grc_GRC_keys_finish
 * @brief Finaliza o objeto de teclas suportadas pelo DIALOG.
 *
 * @param [in,out] grc: Estrutura de um arquivo GRC previamente inicializada.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int al_grc_GRC_keys_finish(struct al_grc *grc);

/**
 * @name al_grc_GRC_objects_start
 * @brief Inicializa o objeto que contera os objetos suportados pelo DIALOG.
 *
 * @param [in,out] grc: Estrutura de um arquivo GRC previamente inicializada.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int al_grc_GRC_objects_start(struct al_grc *grc);

/**
 * @name al_grc_GRC_create_object
 * @brief Inicializa um objeto para ser inserido no DIALOG.
 *
 * @param [in,out] grc: Estrutura de um arquivo GRC previamente inicializada.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int al_grc_GRC_create_object(struct al_grc *grc);

/**
 * @name al_grc_GRC_finish_object
 * @brief Finaliza o objeto que sera inserido no DIALOG.
 *
 * @param [in,out] grc: Estrutura de um arquivo GRC previamente inicializada.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int al_grc_GRC_finish_object(struct al_grc *grc);

/**
 * @name al_grc_GRC_set_object_property
 * @brief Insere uma propriedade no objeto recem criado.
 *
 * @param [in,out] grc: Estrutura de um arquivo GRC previamente inicializada.
 * @param [in] prop: Propriedade que sera inserida no objeto.
 * @param [in] ...: Valor da propriedade.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int al_grc_GRC_set_object_property(struct al_grc *grc,
                                   enum al_grc_object_property prop, ...);

/**
 * @name al_grc_GRC_objects_finish
 * @brief Finaliza o objeto contendo os objetos principais suportados pelo DIALOG.
 *
 * @param [in,out] grc: Estrutura de um arquivo GRC previamente inicializada.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int al_grc_GRC_objects_finish(struct al_grc *grc);

#endif

