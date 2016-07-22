
/*
 * Description: Main library API.
 *
 * Author: Rodrigo Freitas
 * Created at: Sat Dec 13 22:21:54 2014
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
#include "gui/objects.h"

#define LOAD_FROM_MEM       0
#define LOAD_FROM_FILE      1
#define LOAD_BARE_DATA      2

static int start_grc(struct al_grc *grc)
{
    if (grc->use_gfx == true) {
        if (gui_change_resolution(grc) < 0)
            return -1;
    }

    if (gui_load_colors(grc) < 0)
        return -1;;

    return 0;
}

static struct al_grc *al_grc_init(const char *grc_data, int load_mode,
    bool gfx)
{
    struct al_grc *grc;
    int ret = 0;

    al_errno_clear();
    grc = new_grc();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_MEMORY);
        return NULL;
    }

    grc->use_gfx = gfx;

    /* Makes parse from a file or from a buffer */
    if (load_mode == LOAD_FROM_FILE)
        ret = grc_parse_file(grc, grc_data);
    else if (load_mode == LOAD_FROM_MEM)
        ret = grc_parse_mem(grc, grc_data);
    else if (load_mode == LOAD_BARE_DATA) {
        /*
         * We allow creating an object in this mode only when creating a
         * GRC file in memory.
         */
        return grc;
    } else {
        al_set_errno(AL_ERROR_INVALID_LOAD_MODE);
        goto end_block;
    }

    if (ret < 0) {
        al_set_errno(AL_ERROR_PARSE_GRC);
        goto end_block;
    }

    if (start_grc(grc) < 0)
        goto end_block;

    return grc;

end_block:
    destroy_grc(grc);
    return NULL;
}

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
struct al_grc LIBEXPORT *al_grc_init_from_file(const char *grc_file,
    bool gfx)
{
    return al_grc_init(grc_file, LOAD_FROM_FILE, gfx);
}

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
struct al_grc LIBEXPORT *al_grc_init_from_mem(const char *data,
    bool gfx)
{
    return al_grc_init(data, LOAD_FROM_MEM, gfx);
}

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
struct al_grc LIBEXPORT *al_grc_create(void)
{
    return al_grc_init(NULL, LOAD_BARE_DATA, 0);
}

/**
 * @name al_grc_init_from_bare_data
 * @brief Inicializa a interface grafica a partir de informacoes criadas em
 *        tempo de execucao.
 *
 * @param [in] grc: Estrutura principal da biblioteca, previamente inicializada.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int LIBEXPORT al_grc_init_from_bare_data(struct al_grc *grc)
{
    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    return start_grc(grc);
}

/**
 * @name al_grc_uninit
 * @brief Finaliza a interface grafica.
 *
 * @param [in] grc: Estrutura da interface grafica que sera finalizada.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int LIBEXPORT al_grc_uninit(struct al_grc *grc)
{
    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    /* Return the gfx to text mode */
    if (grc->use_gfx == true)
        gui_reset_resolution();

    /* Free the object */
    destroy_grc(grc);

    return 0;
}

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
int LIBEXPORT al_grc_prepare_dialog(struct al_grc *grc)
{
    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    return create_DIALOG(grc);
}

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
int LIBEXPORT al_grc_do_dialog(struct al_grc *grc)
{
    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    run_DIALOG(grc);

    return 0;
}

static int object_has_callback_data(struct al_grc *grc, DIALOG *d)
{
    struct al_callback_data *acd = NULL;

    acd = d->dp3;

    if (NULL == acd)
        return 0;

    /*
     * Nao deveria, mas pode estar apontando para uma area qualquer ao inves
     * do que nos realmente queremos.
     */
    return (acd->grc == grc) ? 1 : 0;
}

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
 *  menu (itens)
 *
 * @param [in] grc: Estrutura da interface grafica previamente inicializada.
 * @param [in] object_name: Nome do objeto que sera atualizado.
 * @param [in] callback: Ponteiro para a funcao de resposta do objeto.
 * @param [in] arg: Argumento para a funcao de resposta.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int LIBEXPORT al_grc_set_callback(struct al_grc *grc, const char *object_name,
    int (*callback)(struct al_callback_data *), void *arg)
{
    DIALOG *d;
    struct al_callback_data *acd = NULL;
    MENU *m;

    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    d = get_DIALOG_from_grc(grc, object_name);

    if (NULL == d) {
        /* Verifica se nao e um item de um menu */
        m = get_MENU_from_grc(grc, object_name);

        if (NULL == m)
            return -1;

        /*
         * Apesar das funcoes de callback receberem uma estrutura como
         * argumento, no caso de um item de um menu, ela nao recebe nenhum,
         * devido uma implementacao interna da Allegro.
         */
        m->proc = (int (*)(void))callback;
        return 0;
    }

    /*
     * O campo @dp3 e utilizado pela biblioteca como mecanismo para armazenar
     * as informacoes relacionadas a funcao de callback do objeto.
     */

    if (object_has_callback_data(grc, d) == 0) {
        acd = new_callback_data();

        if (NULL == acd) {
            al_set_errno(AL_ERROR_MEMORY);
            return -1;
        }

        acd->grc = (void *)grc;
        d->dp3 = acd;

        /* lock */
        grc->callback_data = cdll_unshift(grc->callback_data, acd);
        /* unlock */
    } else
        acd = d->dp3;

    acd->callback = callback;
    acd->user_arg = arg;

    return 0;
}

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
int LIBEXPORT al_grc_get_callback_data(struct al_callback_data *acd,
    enum al_data_type data, ...)
{
    const char *ifmt="%d\0", *sfmt="%[^|]";
    va_list ap;
    char tmp[512]={0};

    al_errno_clear();

    if (NULL == acd) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    va_start(ap, NULL);

    switch (data) {
        case AL_DT_KEY_SCANCODE:
        case AL_DT_SLIDER_POSITION:
        case AL_DT_CHECKBOX_STATE:
        case AL_DT_RADIO_STATE:
        case AL_DT_LIST_POSITION:
            snprintf(tmp, sizeof(tmp) - 1, ifmt, acd->value_int);
            vsscanf(tmp, ifmt, ap);
            break;

        case AL_DT_EDIT_VALUE:
            snprintf(tmp, sizeof(tmp) - 1, "%s", acd->value_string);
            vsscanf(tmp, sfmt, ap);
            break;

        default:
            al_set_errno(AL_ERROR_UNSUPPORTED_DATA_TYPE);
            return -1;
    }

    va_end(ap);

    return 0;
}

/**
 * @name al_grc_get_callback_user_arg
 * @brief Obtem o dado passado ao registrar a funcao de callback do objeto.
 *
 * @param [in] acd: Argumento da funcao de callback.
 *
 * @return Retorna o dado passado ao registrar a funcao de callback ou
 *         NULL em caso de erro.
 */
void LIBEXPORT *al_grc_get_callback_user_arg(struct al_callback_data *acd)
{
    al_errno_clear();

    if (NULL == acd) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return NULL;
    }

    return acd->user_arg;
}

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
struct al_grc LIBEXPORT *al_grc_get_callback_grc(struct al_callback_data *acd)
{
    al_errno_clear();

    if (NULL == acd) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return NULL;
    }

    return acd->grc;
}

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
int LIBEXPORT al_grc_object_set_data(struct al_grc *grc, const char *object_name,
    enum al_data_type type, void *data)
{
    DIALOG *d;
    int s;
    struct al_callback_data *acd = NULL;

    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    d = get_DIALOG_from_grc(grc, object_name);

    if (NULL == d)
        return -1;

    switch (type) {
        case AL_DT_RADIO_STATE:
        case AL_DT_CHECKBOX_STATE:
            s = *((int *)&data);

            if (s == true)
                d->flags |= D_SELECTED;
            else
                d->flags &= ~D_SELECTED;

            break;

        case AL_DT_D1:
        case AL_DT_SLIDER_LIMIT:
            d->d1 = *((int *)&data);
            break;

        case AL_DT_D2:
        case AL_DT_SLIDER_POSITION:
            d->d2 = *((int *)&data);
            break;

        case AL_DT_DP:
        case AL_DT_EDIT_VALUE:
        case AL_DT_TEXT:
        case AL_DT_LIST_CONTENT_BUILD:
        case AL_DT_ICON:
            d->dp = data;
            break;

        case AL_DT_DP2:
            d->dp2 = data;
            break;

        case AL_DT_DP3:
            /*
             * Para manter o padrao nos objetos implementados internamente
             * manipula a estrutura de callback do objeto.
             */
            acd = d->dp3;

            if (NULL == acd) {
                acd = new_callback_data();

                if (NULL == acd) {
                    al_set_errno(AL_ERROR_MEMORY);
                    return -1;
                }

                acd->grc = (void *)grc;
                d->dp3 = acd;
            }

            acd->user_arg = data;
            break;

        default:
            al_set_errno(AL_ERROR_UNSUPPORTED_DATA_TYPE);
            return -1;
    }

    return 0;
}

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
int LIBEXPORT al_grc_object_set_proc(struct al_grc *grc,
    const char *object_name, int (*function)(int, DIALOG *, int))
{
    DIALOG *d;

    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    d = get_DIALOG_from_grc(grc, object_name);

    if (NULL == d)
        return -1;

    d->proc = function;

    return 0;
}

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
int LIBEXPORT al_grc_object_send_message(struct al_grc *grc,
    const char *object_name, int msg, int c)
{
    DIALOG *d;

    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    d = get_DIALOG_from_grc(grc, object_name);

    if (NULL == d)
        return -1;

    object_message(d, msg, c);

    return 0;
}

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
void LIBEXPORT *al_grc_object_get_data(struct al_grc *grc,
    const char *object_name, enum al_data_type type, ...)
{
    const char *ifmt="%d\0";
    va_list ap;
    char tmp[512]={0};
    void *data = NULL;
    int value=-1;
    DIALOG *d;

    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return NULL;
    }

    d = get_DIALOG_from_grc(grc, object_name);

    if (NULL == d)
        return NULL;

    va_start(ap, NULL);

    switch (type) {
        case AL_DT_RADIO_STATE:
        case AL_DT_CHECKBOX_STATE:
            if (d->flags & D_SELECTED)
                value = 1;
            else
                value = 0;

            break;

        case AL_DT_D1:
        case AL_DT_SLIDER_LIMIT:
        case AL_DT_LIST_POSITION:
            value = d->d1;
            break;

        case AL_DT_D2:
        case AL_DT_SLIDER_POSITION:
            value = d->d2;
            break;

        case AL_DT_DP:
        case AL_DT_EDIT_VALUE:
        case AL_DT_TEXT:
            data = d->dp;
            break;

        case AL_DT_DP2:
            data = d->dp2;
            break;

        case AL_DT_DP3:
            data = d->dp3;
            break;

        default:
            al_set_errno(AL_ERROR_UNSUPPORTED_DATA_TYPE);
            return NULL;
    }

    if (value != -1) {
        snprintf(tmp, sizeof(tmp) - 1, ifmt, value);
        vsscanf(tmp, ifmt, ap);

        /*
         * Ajusta o retorno para um ponteiro valido, assim o usuario consegue
         * validar a funcao.
         */
        data = grc;
    }

    va_end(ap);

    return data;
}

/**
 * @name al_grc_object_hide
 * @brief Esconde um determinado objeto.
 *
 * @param [in] grc: Estrutura da interface grafica previamente inicializada.
 * @param [in] object_name: Nome do objeto que sera atualizado.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int LIBEXPORT al_grc_object_hide(struct al_grc *grc, const char *object_name)
{
    DIALOG *d;

    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    d = get_DIALOG_from_grc(grc, object_name);

    if (NULL == d)
        return -1;

    d->flags |= D_HIDDEN;

    return 0;
}

/**
 * @name al_grc_object_show
 * @brief Exibe um determinado objeto (previamente escondido).
 *
 * @param [in] grc: Estrutura da interface grafica previamente inicializada.
 * @param [in] object_name: Nome do objeto que sera atualizado.
 *
 * @return Retorna 0 em caso de sucesso ou -1 em caso de erro.
 */
int LIBEXPORT al_grc_object_show(struct al_grc *grc, const char *object_name)
{
    DIALOG *d;

    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    d = get_DIALOG_from_grc(grc, object_name);

    if (NULL == d)
        return -1;

    d->flags &= ~D_HIDDEN;

    return 0;
}

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
int LIBEXPORT al_grc_log(struct al_grc *grc, const char *object_name,
    const char *msg, const char *color)
{
    DIALOG *d;

    al_errno_clear();

    if (NULL == grc) {
        al_set_errno(AL_ERROR_NULL_ARG);
        return -1;
    }

    d = get_DIALOG_from_grc(grc, object_name);

    if (NULL == d)
        return -1;

    /* Ajusta a mensagem internamente */
    gui_messages_set(d->d1, d->fg, msg, color);

    /* Avisa o objeto sobre ela */
    object_message(d, MSG_NEW_LOG_TEXT, 0);

    return 0;
}

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
int LIBEXPORT al_grc_list_get_selected_index(struct al_grc *grc,
    const char *object_name)
{
    int p=-1;

    if ((al_grc_object_get_data(grc, object_name, AL_DT_LIST_POSITION,
                                &p) == NULL) &&
        (al_get_last_error() != AL_NO_ERROR))
    {
        return -1;
    }

    return p;
}

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
int LIBEXPORT al_grc_checkbox_get_status(struct al_grc *grc,
    const char *object_name)
{
    int st=-1;

    if ((al_grc_object_get_data(grc, object_name, AL_DT_CHECKBOX_STATE,
                                &st) == NULL) &&
        (al_get_last_error() != AL_NO_ERROR))
    {
        return -1;
    }

    return st;
}

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
int LIBEXPORT al_grc_radio_get_status(struct al_grc *grc,
    const char *object_name)
{
    int st=-1;

    if ((al_grc_object_get_data(grc, object_name, AL_DT_RADIO_STATE,
                                &st) == NULL) &&
        (al_get_last_error() != AL_NO_ERROR))
    {
        return -1;
    }

    return st;
}

/**
 * @name al_grc_edit_get_data
 * @brief Obtem o valor atual de um objeto do tipo 'edit'.
 *
 * @param [in] grc: Estrutura da interface grafica previamente inicializada.
 * @param [in] object_name: Nome do objeto que sera procurado.
 *
 * @return Retorna o valor atual do objeto ou NULL em caso de erro.
 */
char LIBEXPORT *al_grc_edit_get_data(struct al_grc *grc,
    const char *object_name)
{
    char *v = NULL;

    v = (char *)al_grc_object_get_data(grc, object_name, AL_DT_EDIT_VALUE,
                                       NULL);

    return v;
}

/*int al_grc_set_bitmap_mem(struct al_grc *grc, const char *object_name,
    unsigned char *bimg, unsigned int bsize, int image_fmt)
{
//    object_message(d, MSG_LOAD_IMAGE, c);
    return -1;
}

int al_grc_set_bitmap(struct al_grc *grc, const char *object_name,
    const char *filename, int image_fmt)
{
    return -1;
}*/

