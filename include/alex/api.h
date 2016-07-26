
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
 * @brief Initialize library environment and Allegro.
 *
 * All UI informations are loaded from a file of GRC type, passed in
 * \a grc_file.
 *
 * The \a gfx argument allows that other GRC files be loaded in the same
 * application, making it to have as many DIALOS as needed.
 *
 * @param [in] grc_file: Complete path of the GRC file which will be loaded.
 * @param [in] gfx: Flag to initialize internal Allegro gfx routines.
 *
 * @return Returns a 'struct al_grc' to handle UI calls on success or NULL
 *         otherwise.
 */
struct al_grc *al_grc_init_from_file(const char *grc_file, bool gfx);

/**
 * @name al_grc_init_from_mem
 * @brief Initialize library environment and Allegro.
 *
 * All UI informations are loaded from a memory buffer containing a previously
 * loaded GRC file.
 *
 * The \a gfx argument allows that other GRC files be loaded in the same
 * application, making it to have as many DIALOS as needed.
 *
 *
 * @param [in] data: The buffer containing a previously loaded GRC file.
 * @param [in] gfx: Flag to initialize internal Allegro gfx routines.
 *
 * @return Returns a 'struct al_grc' to handle UI calls on success or NULL
 *         otherwise.
 */
struct al_grc *al_grc_init_from_mem(const char *data, bool gfx);

/**
 * @name al_grc_create
 * @brief Initialize library environment to create UI in runtime.
 *
 * This functions provides the user a way to define and initialize an UI
 * in runtime. The creation of this UI is done by functions with prefix
 * 'al_grc_GRC_'.
 *
 * @return Returns a 'struct al_grc' to handle UI calls on success or NULL
 *         otherwise.
 */
struct al_grc *al_grc_create(void);

/**
 * @name al_grc_init_from_bare_data
 * @brief Initialize UI from runtime informations.
 *
 * @param [in] grc: Previously created UI structure.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int al_grc_init_from_bare_data(struct al_grc *grc);

/**
 * @name al_grc_uninit
 * @brief Shutdown library environment.
 *
 * This functions returns GFX to text mode.
 *
 * @param [in] grc: UI structure which will be released.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int al_grc_uninit(struct al_grc *grc);

/**
 * @name al_grc_prepare_dialog
 * @brief Starts the next DIALOG to be on the screen.
 *
 * This functions is responsible to make all initialization needed by a DIALOG,
 * defined in a previously loaded GRC file. All custom adjusts by the user must
 * happen after a call to it.
 *
 * @param [in] grc: Previously created UI structure.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int al_grc_prepare_dialog(struct al_grc *grc);

/**
 * @name al_grc_do_dialog
 * @brief Puts a DIALOG to run.
 *
 * All custom adjust in objects from the DIALOG must be made before a call to
 * this function.
 *
 * @param [in] grc: Previously created UI structure.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int al_grc_do_dialog(struct al_grc *grc);

/**
 * @name al_grc_set_callback
 * @brief Assigns a callback function to an object.
 *
 * The callback function must receive two arguments. The first may vary
 * according the object and the second is designated by the user through
 * \a arg.
 *
 * The list of objects that may have a callback function:
 *
 *  button
 *  key
 *  edit
 *  slider
 *  check
 *  radio
 *  list
 *
 * @param [in] grc: Previously created UI structure.
 * @param [in] object_name: The object name.
 * @param [in] callback: Pointer the real function.
 * @param [in] arg: Custom argument of the function.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int al_grc_set_callback(struct al_grc *grc, const char *object_name,
                        int (*callback)(struct al_callback_data *), void *arg);

/**
 * @name al_grc_get_callback_data
 * @brief Gets the return value from a callback function call.
 *
 * @param [in] acd: Callback function argument.
 * @param [in] data: Type of the data.
 * @param [out] ...: Custom data.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int al_grc_get_callback_data(struct al_callback_data *acd,
                             enum al_data_type data, ...);

/**
 * @name al_grc_get_callback_user_arg
 * @brief Gets the custom data passed to a callback function.
 *
 * @param [in] acd: Callback function argument.
 *
 * @return On success returns a pointer the the data passed as argument
 *         to a callback function or NULL otherwise.
 */
void *al_grc_get_callback_user_arg(struct al_callback_data *acd);

/**
 * @name al_grc_get_callback_grc
 * @brief Gets a pointer the the main library object.
 *
 * This function allows a callback function get a pointer to the main library
 * object.
 *
 * @param [in] acd: Callback function argument.
 *
 * @return On success returns a pointer to the main library object or NULL
 *         otherwise.
 */
struct al_grc *al_grc_get_callback_grc(struct al_callback_data *acd);

/* TODO: change data to const */
/**
 * @name al_grc_object_set_data
 * @brief Initialized object members that can receive variable data.
 *
 * @param [in] grc: Previously created UI structure.
 * @param [in] object_name: The object name.
 * @param [in] type: Object data type which will be initialized.
 * @param [in] data: Value which will be assigned to the object member.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int al_grc_object_set_data(struct al_grc *grc, const char *object_name,
                           enum al_data_type type, void *data);

/**
 * @name al_grc_object_set_proc
 * @brief Set an object main function.
 *
 * This function assigns the main function of a custom object.
 *
 * @param [in] grc: Previously created UI structure.
 * @param [in] object_name: The object name.
 * @param [in] function: Pointer to the function.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int al_grc_object_set_proc(struct al_grc *grc, const char *object_name,
                           int (*function)(int, DIALOG *, int));

/**
 * @name al_grc_object_send_message
 * @brief Send a message to an object.
 *
 * @param [in] grc: Previously created UI structure.
 * @param [in] object_name: The object name.
 * @param [in] msg: Sended message.
 * @param [in] c: Message value.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int al_grc_object_send_message(struct al_grc *grc, const char *object_name,
                               int msg, int c);

/* TODO: Change this to use a cvalue_t */
/**
 * @name al_grc_object_get_data
 * @brief Get current value from specific objects.
 *
 * @param [in] grc: Previously created UI structure.
 * @param [in] object_name: The object name.
 * @param [in] type: Object data type which will be initialized.
 * @param [out] ...: Data to store the extracted value.
 *
 * @return If the object return value is not a int, on success returns the value
 *         or NULL otherwise. If the object returns a int value, on success
 *         returns a valid pointer or NULL otherwise.
 */
void *al_grc_object_get_data(struct al_grc *grc, const char *object_name,
                             enum al_data_type type, ...);

/**
 * @name al_grc_object_hide
 * @brief Hide an object on the screen.
 *
 * @param [in] grc: Previously created UI structure.
 * @param [in] object_name: The object name.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int al_grc_object_hide(struct al_grc *grc, const char *object_name);

/**
 * @name al_grc_object_show
 * @brief Show a previously hidden object on the screen.
 *
 * @param [in] grc: Previously created UI structure.
 * @param [in] object_name: The object name.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int al_grc_object_show(struct al_grc *grc, const char *object_name);

/**
 * @name al_grc_log
 * @brief Add a message in a 'message_log_box' object.
 *
 * @param [in] grc: Previously created UI structure.
 * @param [in] object_name: The object name.
 * @param [in] msg: The message.
 * @param [in] color: Message color. If it's NULL, a default color will be
 *                    used.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int al_grc_log(struct al_grc *grc, const char *object_name, const char *msg,
               const char *color);

/**
 * @name al_grc_list_get_selected_index
 * @brief Gets the selected item index from a 'list' object.
 *
 * @param [in] grc: Previously created UI structure.
 * @param [in] object_name: The object name.
 *
 * @return On success returns the item index or -1 otherwise.
 */
int al_grc_list_get_selected_index(struct al_grc *grc, const char *object_name);

/**
 * @name al_grc_checkbox_get_status
 * @brief Gets the current state of a 'checkbox' object.
 *
 * @param [in] grc: Previously created UI structure.
 * @param [in] object_name: The object name.
 *
 * @return On success return the object current state (1 - selected, 0 -
 *         unselected) or -1 otherwise.
 */
int al_grc_checkbox_get_status(struct al_grc *grc, const char *object_name);

/**
 * @name al_grc_radio_get_status
 * @brief Gets the current state of a 'radio' object.
 *
 * @param [in] grc: Previously created UI structure.
 * @param [in] object_name: The object name.
 *
 * @return On success return the object current state (1 - selected, 0 -
 *         unselected) or -1 otherwise.
 */
int al_grc_radio_get_status(struct al_grc *grc, const char *object_name);

/**
 * @name al_grc_edit_get_data
 * @brief Gets the current value of an 'edit' object.
 *
 * @param [in] grc: Previously created UI structure.
 * @param [in] object_name: The object name.
 *
 * @return On success returns the object value or NULL othewise.
 */
char *al_grc_edit_get_data(struct al_grc *grc, const char *object_name);

#endif

