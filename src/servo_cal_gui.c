
/**
 * @mainpage
 * @brief Servo-Calibration Program. A maximum of 6 Servos can control with this
 *				GUI Application. A Position can be named and saved
 * 				into a .csv file.
 *
 * @aurthor Ney Fränz
 * @date    2016
 *
 * gcc -Wall `pkg-config --cflags --libs gtk+-3.0` servo_cal_gui.c callbacks.c
 *				rs232.c list.c -o servo_cal_gui -lm -Wall -rdynamic --pedantic
 *
 */

/******************************************************************* INCLUDES */
#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <cairo.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "callbacks.h"
#include "rs232.h"
#include "list.h"

/******************************************************************** DEFINES */
#define UI_FILE "../glade/servo_calibration.glade"

/******************************************************** FUNCTION PROTOTYPES */
static gpointer guiThread(gpointer data);
static gpointer dataThread(gpointer data);
static gboolean send_data_to_hardware(gpointer data);

/*********************************************************** GLOBAL VARIABLES */
GtkApplication 	*app;
GtkAdjustment  	*adjustment1, *adjustment2, *adjustment3, *adjustment4,
				*adjustment5, *adjustment6;
GtkImage  		*image_connect;
GtkDialog 		*dialog_port, *dialog_about, *dialog_file, *dialog_error,
				*dialog_error_port, *dialog_list, *dialog_name_exists;
GtkWidget 		*combobox1, *combobox2, *combobox3, *combobox4, *combobox5;
GtkWidget 		*bt_con_decon, *entry_name_position, *button_file, *button_save,
				*bt_file_save, *bt_ok_error_port, *bt_edit,* bt_open_file;
GtkWidget 		*switch1;
GtkTreeView 	*treeview;
GtkListStore 	*model;
GtkWidget 		*scale1, *scale2, *scale3, *scale4, *scale5, *scale6;

GArray 			*articles = NULL;
GIOChannel 		*channel = NULL;
gchar 			*filename, *name;
gint 			com_device[8]={0,1,2,3,16,17,18,19};
gint 			device;
gboolean 		con;
Item 			pos;
/****************************************************************** FUNCTIONS */

/**
 * @brief data thread to communicate with the Hardware.
 */
static gpointer
dataThread(gpointer data)
{
	/*Add a timeout function to periodically send data to Hardware*/
	g_timeout_add(150, (GSourceFunc)send_data_to_hardware, NULL);

 	return NULL;
}

/**
 * @brief Convert degree to ms.
 */
int degree_to_ms(int degree){

	int ms;
	float step= (float)500/(float)90;

	if(degree > 90 || degree < -90)
		return -1;

	if(degree > 0){
		ms = (1500+(step*degree));
	}else if(degree == 0){
		ms = 1500;
	}else if(degree < 0){
		ms = 1500+(step*degree);
	}

	return ms;

}

/**
 * @brief Get Servo Values and send to Hardware.
 */
static gboolean
send_data_to_hardware(gpointer data)
{
	gint i=0;
	gint val;
	GtkAdjustment *adjustment[] = {adjustment1, adjustment2, adjustment3,
									adjustment4, adjustment5, adjustment6 };

	/*If Connection is stable send the 6 Servo Positions to Hardware*/
	if(con == TRUE){

		while(i<6){
			/*Get Value from Scale*/
			val = gtk_adjustment_get_value(GTK_ADJUSTMENT(adjustment[i++]));
			/*Convert degree to ms*/
			val = degree_to_ms(val);
			/*Send to Hardware*/
			send_to_hardware(i,val);
		}
 	}

  	return TRUE;
}



/**
 * @brief GUI thread load UI File and create the GUI
 */
static gpointer
guiThread(gpointer data)
{

	GtkBuilder 	*builder;
	GtkWidget 	*window;
	GError 		*error = NULL;

	/* create new GtkBuilder object */
	builder = gtk_builder_new ();

	/* load UI from file. If error occurs, report it and quit application. */
	if (!gtk_builder_add_from_file(builder, UI_FILE, &error)) {
		g_warning("error loading glade file: %s", error->message);
		g_free(error);
		return NULL;
	}

	/*Get objects from UI */

 	window = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
  	scale1 = GTK_WIDGET (gtk_builder_get_object (builder, "scale1"));
  	scale2 = GTK_WIDGET (gtk_builder_get_object (builder, "scale2"));
  	scale3 = GTK_WIDGET (gtk_builder_get_object (builder, "scale3"));
  	scale4 = GTK_WIDGET (gtk_builder_get_object (builder, "scale4"));
  	scale5 = GTK_WIDGET (gtk_builder_get_object (builder, "scale5"));
  	scale6 = GTK_WIDGET (gtk_builder_get_object (builder, "scale6"));

	adjustment1 = GTK_ADJUSTMENT (gtk_builder_get_object (builder,
															"adjustment1"));
	adjustment2 = GTK_ADJUSTMENT (gtk_builder_get_object (builder,
														  	"adjustment2"));
	adjustment3 = GTK_ADJUSTMENT (gtk_builder_get_object (builder,
														  	"adjustment3"));
	adjustment4 = GTK_ADJUSTMENT (gtk_builder_get_object (builder,
														  	"adjustment4"));
	adjustment5 = GTK_ADJUSTMENT (gtk_builder_get_object (builder,
														  	"adjustment5"));
	adjustment6 = GTK_ADJUSTMENT (gtk_builder_get_object (builder,
														  	"adjustment6"));

	dialog_port = GTK_DIALOG (gtk_builder_get_object (builder,
													  	"dialog1"));
	dialog_about = GTK_DIALOG (gtk_builder_get_object (builder,
													  	"aboutdialog1"));
	dialog_file = GTK_DIALOG (gtk_builder_get_object (builder,
													  	"filechooserdialog1"));
	dialog_error = GTK_DIALOG (gtk_builder_get_object (builder,
													  	"dialog_error"));
	dialog_error_port = GTK_DIALOG (gtk_builder_get_object (builder,
														"dialog_error_port"));
	dialog_list = GTK_DIALOG (gtk_builder_get_object (builder,
													  	"dialog_list"));
	dialog_name_exists = GTK_DIALOG (gtk_builder_get_object (builder,
													 	"dialog_name_exists"));

	combobox1 = GTK_WIDGET (gtk_builder_get_object (builder, "comboboxtext1"));
	combobox2 = GTK_WIDGET (gtk_builder_get_object (builder, "comboboxtext2"));
	combobox3 = GTK_WIDGET (gtk_builder_get_object (builder, "comboboxtext3"));
	combobox4 = GTK_WIDGET (gtk_builder_get_object (builder, "comboboxtext4"));
	combobox5 = GTK_WIDGET (gtk_builder_get_object (builder, "comboboxtext5"));

	bt_con_decon = GTK_WIDGET (gtk_builder_get_object (builder,
													   	"bt_con_decon"));
	button_save = GTK_WIDGET (gtk_builder_get_object (builder,
													  	"bt_save_position"));
	button_file = GTK_WIDGET (gtk_builder_get_object (builder,
													  	"bt_file"));
	bt_file_save = GTK_WIDGET (gtk_builder_get_object (builder,
													   	"bt_file_save"));
  	image_connect =  GTK_IMAGE (gtk_builder_get_object (builder,
													  	"image_connect"));
	entry_name_position = GTK_WIDGET (gtk_builder_get_object (builder,
														"entry_name_position"));
	bt_ok_error_port = GTK_WIDGET (gtk_builder_get_object (builder,
														"bt_ok_error_port"));
	switch1 = GTK_WIDGET (gtk_builder_get_object (builder,
												  		"switch1"));
	bt_edit = GTK_WIDGET (gtk_builder_get_object (builder,
												  		"bt_edit"));
	bt_open_file = GTK_WIDGET (gtk_builder_get_object (builder,
													   	"bt_open_file"));


	treeview = GTK_TREE_VIEW (gtk_builder_get_object (builder, "treeview"));
	model = GTK_LIST_STORE (gtk_builder_get_object (builder, "model"));

	/*************************************************/

	gtk_builder_connect_signals (builder, NULL);
  	g_object_unref (G_OBJECT (builder));

  	gtk_window_set_application (GTK_WINDOW (window), GTK_APPLICATION (app));
  	gtk_window_set_title (GTK_WINDOW (window), "Servo Calibration");
	gtk_window_set_default_size (GTK_WINDOW (window), 700,530);
  	gtk_window_set_default_icon_from_file("../glade/img8.png", NULL);
  	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);

	gtk_window_set_transient_for (GTK_WINDOW (dialog_port),
								  	GTK_WINDOW (window));
	gtk_window_set_transient_for (GTK_WINDOW (dialog_about),
								  	GTK_WINDOW (window));
	gtk_window_set_transient_for (GTK_WINDOW (dialog_file),
								  	GTK_WINDOW (window));
	gtk_window_set_transient_for (GTK_WINDOW (dialog_error),
								  	GTK_WINDOW (window));
	gtk_window_set_transient_for (GTK_WINDOW (dialog_error_port),
								  	GTK_WINDOW (window));
	gtk_window_set_transient_for (GTK_WINDOW (dialog_list),
								  	GTK_WINDOW (window));
	gtk_window_set_transient_for (GTK_WINDOW (dialog_name_exists),
								 	GTK_WINDOW (window));

  	gtk_scale_add_mark (GTK_SCALE(scale1), 0, GTK_POS_LEFT ,"0°");
	gtk_scale_add_mark (GTK_SCALE(scale2), 0, GTK_POS_LEFT ,"0°");
	gtk_scale_add_mark (GTK_SCALE(scale3), 0, GTK_POS_LEFT ,"0°");
	gtk_scale_add_mark (GTK_SCALE(scale4), 0, GTK_POS_LEFT ,"0°");
	gtk_scale_add_mark (GTK_SCALE(scale5), 0, GTK_POS_LEFT ,"0°");
	gtk_scale_add_mark (GTK_SCALE(scale6), 0, GTK_POS_LEFT ,"0°");

	gtk_widget_set_sensitive (GTK_WIDGET(entry_name_position), FALSE);
	gtk_widget_set_sensitive (GTK_WIDGET(button_save), FALSE);
	gtk_widget_set_sensitive (GTK_WIDGET(switch1), FALSE);
	gtk_widget_set_sensitive (GTK_WIDGET(bt_edit), FALSE);


  	gtk_widget_show_all(GTK_WIDGET(window));
  	gtk_main();
  	return NULL;
}


/**
 * @brief main only creates two threads
 */
int
main (int argc, char *argv[])
{
  GThread* GuiThread;
  GThread* DataThread;

  gtk_init(&argc, &argv);

  /* create two threads: one for the GUI and the DATA, respectively */
  GuiThread = g_thread_new("Thread GUI", (GThreadFunc)guiThread, (void *)NULL);
  DataThread = g_thread_new("Thread DATA", (GThreadFunc)dataThread, (void *)NULL);

  g_thread_join(DataThread);
  g_thread_join(GuiThread);
  return 0;
}

/** EOF */


