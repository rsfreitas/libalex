
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Sun Aug 24 21:05:01 2014
 * Project: libgrc
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

#ifndef _LIBGRC_API_H
#define _LIBGRC_API_H                            1

#ifndef LIBGRC_COMPILE
# ifndef _LIBGRC_H
#  error "Never use <api.h> directly; include <libgrc.h> instead."
# endif
#endif

/**
 * @name grc_init_from_file
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
 * @return Returns a 'grc_t' to handle UI calls on success or NULL
 *         otherwise.
 */
grc_t *grc_init_from_file(const char *grc_file, bool gfx);

/**
 * @name grc_init_from_mem
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
 * @return Returns a 'grc_t' to handle UI calls on success or NULL
 *         otherwise.
 */
grc_t *grc_init_from_mem(const char *data, bool gfx);

/**
 * @name grc_create
 * @brief Initialize library environment to create UI in runtime.
 *
 * This functions provides the user a way to define and initialize an UI
 * in runtime. The creation of this UI is done by functions with prefix
 * 'grc_GRC_'.
 *
 * @return Returns a 'grc_t' to handle UI calls on success or NULL
 *         otherwise.
 */
grc_t *grc_create(void);

/**
 * @name grc_init_from_bare_data
 * @brief Initialize UI from runtime informations.
 *
 * @param [in] grc: Previously created UI structure.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int grc_init_from_bare_data(grc_t *grc);

/**
 * @name grc_uninit
 * @brief Shutdown library environment.
 *
 * This functions returns GFX to text mode.
 *
 * @param [in] grc: UI structure which will be released.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int grc_uninit(grc_t *grc);

/**
 * @name grc_prepare_dialog
 * @brief Starts the next DIALOG to be on the screen.
 *
 * This functions is responsible to make all initialization Allegro needs to
 * use the DIALOG defined in a previously loaded GRC file. All custom
 * adjustments required by the user must happen before a call to it.
 *
 * @param [in] grc: Previously created UI structure.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int grc_prepare_dialog(grc_t *grc);

/**
 * @name grc_do_dialog
 * @brief Puts a DIALOG to run.
 *
 * All custom adjust in objects from the DIALOG must be made before a call to
 * this function.
 *
 * @param [in] grc: Previously created UI structure.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int grc_do_dialog(grc_t *grc);

/**
 * @name grc_set_callback
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
int grc_set_callback(grc_t *grc, const char *object_name,
                     int (*callback)(grc_callback_data_t *), void *arg);

/**
 * @name grc_get_callback_data
 * @brief Gets the return value from a callback function call.
 *
 * @param [in] acd: Callback function argument.
 * @param [in] member: Type of the data.
 * @param [out] ...: Custom data.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int grc_get_callback_data(grc_callback_data_t *acd,
                          enum grc_object_member member, ...);

/**
 * @name grc_get_callback_user_arg
 * @brief Gets the custom data passed to a callback function.
 *
 * @param [in] acd: Callback function argument.
 *
 * @return On success returns a pointer the the data passed as argument
 *         to a callback function or NULL otherwise.
 */
void *grc_get_callback_user_arg(grc_callback_data_t *acd);

/**
 * @name grc_get_callback_grc
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
grc_t *grc_get_callback_grc(grc_callback_data_t *acd);

/* TODO: change data to const */
/**
 * @name grc_object_set_data
 * @brief Initialized object members that can receive variable data.
 *
 * @param [in] grc: Previously created UI structure.
 * @param [in] object_name: The object name.
 * @param [in] member: Object data type which will be initialized.
 * @param [in] data: Value which will be assigned to the object member.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int grc_object_set_data(grc_t *grc, const char *object_name,
                        enum grc_object_member member, void *data);

/**
 * @name grc_object_set_proc
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
int grc_object_set_proc(grc_t *grc, const char *object_name,
                        int (*function)(int, DIALOG *, int));

/**
 * @name grc_object_send_message
 * @brief Send a message to an object.
 *
 * @param [in] grc: Previously created UI structure.
 * @param [in] object_name: The object name.
 * @param [in] msg: Sended message.
 * @param [in] c: Message value.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int grc_object_send_message(grc_t *grc, const char *object_name,
                            int msg, int c);

/* TODO: Change this to use a cvalue_t */
/**
 * @name grc_object_get_data
 * @brief Get current value from specific objects.
 *
 * @param [in] grc: Previously created UI structure.
 * @param [in] object_name: The object name.
 * @param [in] member: Object data type which will be initialized.
 * @param [out] ...: Data to store the extracted value.
 *
 * @return If the object return value is not a int, on success returns the value
 *         or NULL otherwise. If the object returns a int value, on success
 *         returns a valid pointer or NULL otherwise.
 */
void *grc_object_get_data(grc_t *grc, const char *object_name,
                          enum grc_object_member member, ...);

/**
 * @name grc_object_hide
 * @brief Hide an object on the screen.
 *
 * @param [in] grc: Previously created UI structure.
 * @param [in] object_name: The object name.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int grc_object_hide(grc_t *grc, const char *object_name);

/**
 * @name grc_object_show
 * @brief Show a previously hidden object on the screen.
 *
 * @param [in] grc: Previously created UI structure.
 * @param [in] object_name: The object name.
 *
 * @return On success returns 0 or -1 otherwise.
 */
int grc_object_show(grc_t *grc, const char *object_name);

/**
 * @name grc_log
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
int grc_log(grc_t *grc, const char *object_name, const char *msg,
            const char *color);

/**
 * @name grc_list_get_selected_index
 * @brief Gets the selected item index from a 'list' object.
 *
 * @param [in] grc: Previously created UI structure.
 * @param [in] object_name: The object name.
 *
 * @return On success returns the item index or -1 otherwise.
 */
int grc_list_get_selected_index(grc_t *grc, const char *object_name);

/**
 * @name grc_checkbox_get_status
 * @brief Gets the current state of a 'checkbox' object.
 *
 * @param [in] grc: Previously created UI structure.
 * @param [in] object_name: The object name.
 *
 * @return On success return the object current state (1 - selected, 0 -
 *         unselected) or -1 otherwise.
 */
int grc_checkbox_get_status(grc_t *grc, const char *object_name);

/**
 * @name grc_radio_get_status
 * @brief Gets the current state of a 'radio' object.
 *
 * @param [in] grc: Previously created UI structure.
 * @param [in] object_name: The object name.
 *
 * @return On success return the object current state (1 - selected, 0 -
 *         unselected) or -1 otherwise.
 */
int grc_radio_get_status(grc_t *grc, const char *object_name);

/**
 * @name grc_edit_get_data
 * @brief Gets the current value of an 'edit' object.
 *
 * @param [in] grc: Previously created UI structure.
 * @param [in] object_name: The object name.
 *
 * @return On success returns the object value or NULL othewise.
 */
char *grc_edit_get_data(grc_t *grc, const char *object_name);

#endif

