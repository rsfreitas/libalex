
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

#ifndef _LIBALEX_API_H
#define _LIBALEX_API_H                            1

#ifndef LIBALEX_COMPILE
# ifndef _LIBALEX_H
#  error "Never use <api.h> directly; include <libalex.h> instead."
# endif
#endif

/**
 * @name al_grc_init_from_file
 * @brief Inicializa o ambiente e a interface grafica.
 *
 * As informacoes a respeito da interface grafica sao carregadas a partir de
 * um arquivo do tipo GRC informado em \a grc_file.
 *
 * O argumento \a gfx permite que varios arquivos GRC sejam carregados numa
 * mesma aplicacao, fazendo com que ela possua quantos DIALOGs forem
 * necessarios.
 *
 * @param [in] grc_file: Caminho do arquivo GRC contendo a interface grafica
 *                       que sera inicializada.
 * @param [in] gfx: Flag indicando se o ambiente grafico sera inicializado
 *                  ou nao.
 *
 * @return Retorna uma estrutura para manipular a interface grafica em caso de
 *         sucesso ou NULL em caso de erro.
 */
struct al_grc *al_grc_init_from_file(const char *grc_file, bool gfx);

/**
 * @name al_grc_init_from_mem
 * @brief Inicializa o ambiente e a interface grafica.
 *
 * As informacoes a respeito da interface grafica sao carregadas a partir de
 * um buffer em memoria contendo um arquivo GRC previamente carregado.
 *
 * O argumento \a gfx permite que varios arquivos GRC sejam carregados numa
 * mesma aplicacao, fazendo com que ela possua quantos DIALOGs forem
 * necessarios.
 *
 * @param [in] data: Buffer contendo o arquivo GRC previamente carregado.
 * @param [in] gfx: Flag indicando se o ambiente grafico sera inicializado
 *                  ou nao.
 *
 * @return Retorna uma estrutura para manipular a interface grafica em caso de
 *         sucesso ou NULL em caso de erro.
 */
struct al_grc *al_grc_init_from_mem(const char *data, bool gfx);

/**
 * @name al_grc_create
 * @brief Inicializa o ambiente para a criacao da interface grafica em tempo
 *        de execucao.
 *
 * Esta funcao permite que o usuario defina e inicialize o ambiente de uma
 * interface grafica em tempo de execucao. A definicao da interface e realizada
 * atraves das funcoes com prefixo 'al_grc_GRC_'.
 *
 * @return Retorna uma estrutura para manipular a interface grafica em caso de
 *         sucesso ou NULL em caso de erro.
 */
struct al_grc *al_grc_create(void);

/**
 * @name al_grc_init_from_bare_data
 * @brief Inicializa a interface grafica a partir de informacoes criadas em
 *        tempo de execucao.
 *
 * @param [in] grc: Estrutura principal da biblioteca, previamente inicializada.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int al_grc_init_from_bare_data(struct al_grc *grc);

/**
 * @name al_grc_uninit
 * @brief Finaliza a interface grafica.
 *
 * @param [in] grc: Estrutura da interface grafica que sera finalizada.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int al_grc_uninit(struct al_grc *grc);

/**
 * @name al_grc_prepare_dialog
 * @brief Inicializa o DIALOG que sera exibido na interface grafica.
 *
 * Esta funcao e responsavel por realizar toda inicializacao possivel do DIALOG
 * definido no arquivo GRC carregado previamente. Todo ajuste customizado pelo
 * usuario deve acontecer apos a sua chamada.
 *
 * @param [in] grc: Estrutura da interface grafica previamente inicializada.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso erro.
 */
int al_grc_prepare_dialog(struct al_grc *grc);

/**
 * @name al_grc_do_dialog
 * @brief Coloca o DIALOG em execucao.
 *
 * Todo ajuste customizado em objetos do DIALOG deve ser realizado antes da
 * chamada desta funcao. Para que, quando ele for montado para exibicao na
 * tela, tudo ja esteja corretamente inicializado.
 *
 * @param [in] grc: Estrutura da interface grafica previamente inicializada.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int al_grc_do_dialog(struct al_grc *grc);

/**
 * @name al_grc_set_callback
 * @brief Atribui uma funcao para ser chamada quando um objeto for ativado.
 *
 * A funcao de resposta do objeto deve receber dois argumentos, sendo que o
 * primeiro pode variar de acordo com o objeto e o segundo indicado pelo
 * proprio usuario no argumento \a arg.
 *
 * Objetos com suporte a funcao de resposta e seu respectivo argumento:
 *
 *  button
 *  key
 *  edit
 *  slider
 *  check
 *  radio
 *  list
 *
 * @param [in] grc: Estrutura da interface grafica previamente inicializada.
 * @param [in] object_name: Nome do objeto que sera atualizado.
 * @param [in] callback: Ponteiro para a funcao de resposta do objeto.
 * @param [in] arg: Argumento para a funcao de resposta.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int al_grc_set_callback(struct al_grc *grc, const char *object_name,
                        int (*callback)(struct al_callback_data *), void *arg);

/**
 * @name al_grc_get_callback_data
 * @brief Obtem dados do objeto de dentro da funcao de callback.
 *
 * @param [in] acd: Argumento da funcao de callback.
 * @param [in] data: Tipo de dado que sera extraido.
 * @param [out] ...: Variavel para armazenar o dado extraido.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int al_grc_get_callback_data(struct al_callback_data *acd,
                             enum al_data_type data, ...);

/**
 * @name al_grc_get_callback_user_arg
 * @brief Obtem o dado passado ao registrar a funcao de callback do objeto.
 *
 * @param [in] acd: Argumento da funcao de callback.
 *
 * @return Retorna o dado passado ao registrar a funcao de callback ou
 *         NULL em caso de erro.
 */
void *al_grc_get_callback_user_arg(struct al_callback_data *acd);

/**
 * @name al_grc_get_callback_grc
 * @brief Obtem a estrutura principal da biblioteca de dentro de um funcao de
 *        callback.
 *
 * @param [in] acd: Argumento da funcao de callback.
 *
 * @return Retorna um ponteiro para a estrutura principal da biblioteca ou NULL
 *         em caso de erro.
 */
struct al_grc *al_grc_get_callback_grc(struct al_callback_data *acd);

/**
 * @name al_grc_object_set_data
 * @brief Inicializa membros do objeto capazes de receberem dados variaveis.
 *
 * @param [in] grc: Estrutura da interface grafica previamente inicializada.
 * @param [in] object_name: Nome do objeto que sera atualizado.
 * @param [in] type: Tipo de dado que sera inicializado, referente ao membro
 *                   da estrutura do objeto.
 * @param [in] data: Valor que sera atribuido ao membro definido.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int al_grc_object_set_data(struct al_grc *grc, const char *object_name,
                           enum al_data_type type, void *data);

/**
 * @name al_grc_object_set_proc
 * @brief Atribui a funcao principal de um objeto.
 *
 * Esta funcao e responsavel por atribuir a funcao principal de objetos
 * customizados.
 *
 * @param [in] grc: Estrutura da interface grafica previamente inicializada.
 * @param [in] object_name: Nome do objeto que sera atualizado.
 * @param [in] function: Ponteiro para a funcao do objeto.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int al_grc_object_set_proc(struct al_grc *grc, const char *object_name,
                           int (*function)(int, DIALOG *, int));

/**
 * @name al_grc_object_send_message
 * @brief Envia uma mensagem para um determinado objeto.
 *
 * @param [in] grc: Estrutura da interface grafica previamente inicializada.
 * @param [in] object_name: Nome do objeto que sera atualizado.
 * @param [in] msg: Mensagem que esta sendo enviada.
 * @param [in] c: Argumento da mensagem.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int al_grc_object_send_message(struct al_grc *grc, const char *object_name,
                               int msg, int c);

/**
 * @name al_grc_object_get_data
 * @brief Obtem valores atuais de objetos especificos.
 *
 * @param [in] grc: Estrutura da interface grafica previamente inicializada.
 * @param [in] object_name: Nome do objeto que sera atualizado.
 * @param [in] type: Tipo de dado que sera inicializado, referente ao membro
 *                   da estrutura do objeto.
 * @param [out] ...: Variavel para armazenar o dado extraido.
 *
 * @return No caso de objetos com valores diferentes de numeros inteiros, retorna
 *         o valor ou NULL em caso de erro. No caso de objetos com valores sendo
 *         numeros inteiros, preenche um argumento com o valor e retorna um dado
 *         valido (que o usuario pode somente validar) ou NULL em caso de erro.
 */
void *al_grc_object_get_data(struct al_grc *grc, const char *object_name,
                             enum al_data_type type, ...);

/**
 * @name al_grc_object_hide
 * @brief Esconde um determinado objeto.
 *
 * @param [in] grc: Estrutura da interface grafica previamente inicializada.
 * @param [in] object_name: Nome do objeto que sera atualizado.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int al_grc_object_hide(struct al_grc *grc, const char *object_name);

/**
 * @name al_grc_object_show
 * @brief Exibe um determinado objeto (previamente escondido).
 *
 * @param [in] grc: Estrutura da interface grafica previamente inicializada.
 * @param [in] object_name: Nome do objeto que sera atualizado.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int al_grc_object_show(struct al_grc *grc, const char *object_name);

/**
 * @name al_grc_log
 * @brief Insere uma mensagem num objeto do tipo 'message_log_box'.
 *
 * @param [in] grc: Estrutura da interface grafica previamente inicializada.
 * @param [in] object_name: Nome do objeto que sera atualizado.
 * @param [in] msg: Mensagem que sera inserida.
 * @param [in] color: Cor da mensagem. Se for NULL, a cor default do objeto
 *                    sera utilizada.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int al_grc_log(struct al_grc *grc, const char *object_name, const char *msg,
               const char *color);

/**
 * @name al_grc_list_get_selected_index
 * @brief Obtem o indice do item selecionado atualmente num objeto do tipo
 *        'list'.
 *
 * @param [in] grc: Estrutura da interface grafica previamente inicializada.
 * @param [in] object_name: Nome do objeto que sera procurado.
 *
 * @return Retorna o indice do item selecionado (que deve ser >= 0) ou -1 em
 *         caso de erro.
 */
int al_grc_list_get_selected_index(struct al_grc *grc, const char *object_name);

/**
 * @name al_grc_checkbox_get_status
 * @brief Obtem o estado atual de um objeto do tipo 'checkbox'.
 *
 * @param [in] grc: Estrutura da interface grafica previamente inicializada.
 * @param [in] object_name: Nome do objeto que sera procurado.
 *
 * @return Retorna o estado do objeto, sendo 1 para selecionado e 0 nao
 *         selecionado, ou -1 em caso de erro.
 */
int al_grc_checkbox_get_status(struct al_grc *grc, const char *object_name);

/**
 * @name al_grc_radio_get_status
 * @brief Obtem o estado atual de um objeto do tipo 'radio'.
 *
 * @param [in] grc: Estrutura da interface grafica previamente inicializada.
 * @param [in] object_name: Nome do objeto que sera procurado.
 *
 * @return Retorna o estado do objeto, sendo 1 para selecionado e 0 nao
 *         selecionado, ou -1 em caso de erro.
 */
int al_grc_radio_get_status(struct al_grc *grc, const char *object_name);

/**
 * @name al_grc_edit_get_data
 * @brief Obtem o valor atual de um objeto do tipo 'edit'.
 *
 * @param [in] grc: Estrutura da interface grafica previamente inicializada.
 * @param [in] object_name: Nome do objeto que sera procurado.
 *
 * @return Retorna o valor atual do objeto ou NULL em caso de erro.
 */
char *al_grc_edit_get_data(struct al_grc *grc, const char *object_name);

#endif

