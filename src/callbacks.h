/*
 * callbacks.h
 *
 *  Created on: Aug 23, 2015
 *      Author: Ney Fränz
 */

#ifndef CALLBACKS_H_
#define CALLBACKS_H_

/******************************************************************* INCLUDES */
#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <cairo.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "rs232.h"
#include "list.h"

/********************************************************* TYPEDEFS & STRUCTS */

/******************************************************************** DEFINES */

/******************************************************** FUNCTION PROTOTYPES */
G_MODULE_EXPORT void
on_bt_reset_clicked( GtkButton *button, gpointer   data );

G_MODULE_EXPORT void
bt_con_decon_clicked_cb( GtkButton *button, gpointer   data );

G_MODULE_EXPORT void
bt_cancel_clicked_cb( GtkButton *button, gpointer   data );

G_MODULE_EXPORT void
button_port_clicked_cb( GtkButton *button, gpointer   data );

G_MODULE_EXPORT void
bt_about_clicked_cb( GtkButton *button, gpointer   data );

G_MODULE_EXPORT void
switch1_state_set_cb( GtkSwitch *switch1, gpointer   data );

G_MODULE_EXPORT void
bt_ok_port_clicked_cb( GtkButton *button, gpointer   data );

G_MODULE_EXPORT void
bt_file_clicked_cb( GtkButton *button, gpointer   data );

G_MODULE_EXPORT void
bt_file_cancel_clicked_cb( GtkButton *button, gpointer   data );

G_MODULE_EXPORT void
bt_file_save_clicked_cb( GtkButton *button, gpointer   data );

G_MODULE_EXPORT void
bt_error_ok_clicked_cb( GtkButton *button, gpointer   data );

G_MODULE_EXPORT void
aboutdialog1_button_press_event_cb( GtkAboutDialog *dialog, gpointer   data );

G_MODULE_EXPORT void
filechooserdialog1_current_folder_changed_cb( GtkFileChooser *file_chooser, gpointer   data );

G_MODULE_EXPORT void
bt_edit_clicked_cb( GtkButton *button, gpointer   data );

G_MODULE_EXPORT void
bt_list_ok_clicked_cb(  GtkButton *button, gpointer   data );

G_MODULE_EXPORT void
window1_delete_event_cb(  GtkWindow *window, gpointer   data );


#endif /* CALLBACKS_H_ */

/** EOF */
