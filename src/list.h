/*
 * list.h
 *
 *  Created on: Sep 02, 2015
 *      Author: Ney Fränz
 */

#ifndef LIST_H_
#define LIST_H_

/******************************************************************* INCLUDES */
#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <cairo.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rs232.h"

/********************************************************* TYPEDEFS & STRUCTS */

typedef struct
{
	gchar *name;
  	gint 	s1_pos;
	gint 	s2_pos;
	gint 	s3_pos;
	gint 	s4_pos;
	gint 	s5_pos;
	gint 	s6_pos;
}
Item;

enum
{
	COLUMN_ITEM_NAME,
	COLUMN_ITEM_S1_POS,
	COLUMN_ITEM_S2_POS,
	COLUMN_ITEM_S3_POS,
	COLUMN_ITEM_S4_POS,
	COLUMN_ITEM_S5_POS,
	COLUMN_ITEM_S6_POS,
	NUM_ITEM_COLUMNS
};

enum
{
	COLUMN_NUMBER_TEXT,
	NUM_NUMBER_COLUMNS
};

/******************************************************************** DEFINES */

/******************************************************** FUNCTION PROTOTYPES */
G_MODULE_EXPORT void
bt_save_position_clicked_cb( GtkButton *button, gpointer data );

G_MODULE_EXPORT void
bt_list_remove_clicked_cb (GtkButton *button, gpointer   data );

void create_open_file(void);

#endif /* LIST_H_ */

/** EOF */
