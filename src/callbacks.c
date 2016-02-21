/*
 * 	callbacks.c
 *
 *  Created on: Aug 23, 2015
 *      Author: Ney Fränz
 */

/******************************************************************* INCLUDES */
#include "callbacks.h"

/*********************************************************** GLOBAL VARIABLES */

extern GtkAdjustment  	*adjustment1, *adjustment2, *adjustment3, *adjustment4,
						*adjustment5, *adjustment6;
extern GtkImage  		*image_connect;
extern GtkDialog 		*dialog_port, *dialog_about, *dialog_file,
						*dialog_error, *dialog_error_port, *dialog_list,
						*dialog_name_exists;
extern GtkWidget 		*window;
extern GtkWidget 		*combobox1, *combobox2, *combobox3, *combobox4,
						*combobox5;
extern GtkWidget 		*bt_con_decon, *entry_name_position, *button_file,
						*button_save, *bt_file_save, *bt_ok_error_port,
						*bt_edit,* bt_open_file;
extern GtkWidget 		*switch1;
extern GArray 			*articles;
extern GIOChannel 		*channel;
extern gint 			com_device[];
extern gint 			device;
extern Item 			pos;
extern gboolean 		con;

/****************************************************************** VARIABLES */
gchar *filename, *name;

/********************************************************* CALLBACK FUNCTIONS */

/**
 * @brief Set the 6 Scales to 0°.
 */
G_MODULE_EXPORT void
on_bt_reset_clicked( GtkButton *button,
            gpointer   data )
{

	gtk_adjustment_set_value (GTK_ADJUSTMENT(adjustment1), 0);
	gtk_adjustment_set_value (GTK_ADJUSTMENT(adjustment2), 0);
	gtk_adjustment_set_value (GTK_ADJUSTMENT(adjustment3), 0);
	gtk_adjustment_set_value (GTK_ADJUSTMENT(adjustment4), 0);
	gtk_adjustment_set_value (GTK_ADJUSTMENT(adjustment5), 0);
	gtk_adjustment_set_value (GTK_ADJUSTMENT(adjustment6), 0);

}


/**
 * @brief Connect Serial Port with given Port Settings.
 */
G_MODULE_EXPORT void
bt_con_decon_clicked_cb( GtkButton *button,
            gpointer   data )
{

	gint 	com_baut_rate[9]={	9600, 19200, 38400, 57600, 115200, 128000,
								256000, 500000, 1000000};
	gchar com_data_bits[4][3]={"5","6","7","8"};
	gchar com_stop_bits[2][3]={"1","2"};
	gchar com_parity[3][3]={"N","O","E"};
	gint 	baud_rate, data_bits, stop_bits, parity;
	gchar mode[5];
	static gboolean connect;

	if(!connect){

		device 		= gtk_combo_box_get_active(GTK_COMBO_BOX (combobox1));
		baud_rate = gtk_combo_box_get_active(GTK_COMBO_BOX (combobox2));
  		data_bits = gtk_combo_box_get_active(GTK_COMBO_BOX (combobox3));
  		stop_bits = gtk_combo_box_get_active(GTK_COMBO_BOX (combobox4));
  		parity 		= gtk_combo_box_get_active(GTK_COMBO_BOX (combobox5));

		sprintf(mode,"%s%s%s",com_data_bits[data_bits],com_parity[parity],
				com_stop_bits[stop_bits]);

		if(RS232_OpenComport(com_device[device],
							 com_baut_rate[baud_rate], mode))
  		{
    		g_printf("Can not open comport\n");
			gtk_widget_show_all (GTK_WIDGET(dialog_error_port));
			gtk_button_set_label (GTK_BUTTON(button),  "Connect...");
			connect = FALSE;
			con 	= FALSE;
  		}else{
			gtk_button_set_label (GTK_BUTTON(button),  "Disconnect");
			gtk_image_set_from_file (GTK_IMAGE(image_connect),
									  "../glade/img10.png");
			gtk_switch_set_active (GTK_SWITCH(switch1), FALSE);
			gtk_widget_set_sensitive (GTK_WIDGET(switch1), TRUE);
			g_print("Connect\n");
			connect = TRUE;
		}
	}else{
		device = gtk_combo_box_get_active(GTK_COMBO_BOX (combobox1));
		RS232_CloseComport(com_device[device]);
		gtk_button_set_label (GTK_BUTTON(button),  "Connect...");
		gtk_image_set_from_file (GTK_IMAGE(image_connect),
								  "../glade/img1.png");
		gtk_widget_set_sensitive (GTK_WIDGET(switch1), FALSE);
		g_print("Disconnect\n");
		connect = FALSE;
		con 	= FALSE;
	}
}

/**
 * @brief Hide dialog_port when cancel clicked.
 */
G_MODULE_EXPORT void
bt_cancel_clicked_cb( GtkButton *button,
            gpointer   data )
{

  gtk_widget_hide (GTK_WIDGET (dialog_port));

}

/**
 * @brief Show dialog_port when button_port clicked.
 */
G_MODULE_EXPORT void
button_port_clicked_cb( GtkButton *button,
            gpointer   data )
{

	gtk_widget_show_all (GTK_WIDGET(dialog_port));

}

/**
 * @brief Show dalog_about when bt_about clicked.
 */
G_MODULE_EXPORT void
bt_about_clicked_cb( GtkButton *button,
            gpointer   data )
{

	gtk_widget_show_all (GTK_WIDGET(dialog_about));

}

/**
 * @brief Enable data transfer when switch on.
 *				Disable data transfer when switch off.
 */
G_MODULE_EXPORT void
switch1_state_set_cb( GtkSwitch *switch1,
            gpointer   data )
{

	if (gtk_switch_get_active (GTK_SWITCH (switch1))){
		con = TRUE;
  	}else{
		con = FALSE;
	}

}

/**
 * @brief Get Port Settings when bt_ok_port clicked.
 */
G_MODULE_EXPORT void
bt_ok_port_clicked_cb( GtkButton *button,
            gpointer   data )
{

	gint device, baud_rate, data_bits, stop_bits, parity;

	device = gtk_combo_box_get_active(GTK_COMBO_BOX (combobox1));
	g_print("Device=%d\n",device);
	baud_rate = gtk_combo_box_get_active(GTK_COMBO_BOX (combobox2));
	g_print("Baud Rate=%d\n",baud_rate);
  	data_bits = gtk_combo_box_get_active(GTK_COMBO_BOX (combobox3));
	g_print("Data Bits=%d\n",data_bits);
  	stop_bits = gtk_combo_box_get_active(GTK_COMBO_BOX (combobox4));
	g_print("Stop Bits=%d\n",stop_bits);
  	parity = gtk_combo_box_get_active(GTK_COMBO_BOX (combobox5));
	g_print("Parity=%d\n",parity);

	gtk_widget_hide (GTK_WIDGET (dialog_port));

}

/**
 * @brief Show dialog_file when bt_file_clicked.
 */
G_MODULE_EXPORT void
bt_file_clicked_cb( GtkButton *button,
            gpointer   data )
{
	gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER(dialog_file),
                                   "servo.csv");
	gtk_widget_set_sensitive (GTK_WIDGET(bt_file_save), FALSE);
	gtk_widget_show_all (GTK_WIDGET(dialog_file));

}

/**
 * @brief hide dialog_file when bt_file_cancel clicked.
 */
G_MODULE_EXPORT void
bt_file_cancel_clicked_cb( GtkButton *button,
            gpointer   data )
{

	gtk_widget_hide (GTK_WIDGET (dialog_file));
	gtk_button_set_label (GTK_BUTTON(bt_file_save),"Save");

}

/**
 * @brief Create .csv file and check if already a file exits.
 */
G_MODULE_EXPORT void
bt_file_save_clicked_cb( GtkButton *button,
            gpointer   data )
{
	GError *error = NULL;

 	filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER(dialog_file));
	name = gtk_file_chooser_get_current_name (GTK_FILE_CHOOSER(dialog_file));

	if(filename!= NULL){
		channel = g_io_channel_new_file (filename, "a+", NULL);

		create_open_file();
		g_io_channel_shutdown (channel,TRUE,&error); /*test*/

		gtk_button_set_label (GTK_BUTTON(button_file), name);

		gtk_widget_set_sensitive (GTK_WIDGET(entry_name_position), TRUE);
		gtk_widget_set_sensitive (GTK_WIDGET(button_save), TRUE);
		gtk_widget_set_sensitive (GTK_WIDGET(bt_edit), TRUE);

 		if(channel == NULL)
    	{
      		g_error("failed to create new file:'%s' (channel) for log",
					filename);
			gtk_widget_set_sensitive (GTK_WIDGET(entry_name_position), FALSE);
			gtk_widget_set_sensitive (GTK_WIDGET(button_save), FALSE);
			gtk_widget_set_sensitive (GTK_WIDGET(bt_edit), FALSE);
    	}

 	}else{
		gtk_widget_set_sensitive (GTK_WIDGET(entry_name_position), FALSE);
		gtk_widget_set_sensitive (GTK_WIDGET(button_save), FALSE);
		gtk_widget_set_sensitive (GTK_WIDGET(bt_edit), FALSE);
		gtk_button_set_label (GTK_BUTTON(button_file), "(None)");
	}

	gtk_widget_hide (GTK_WIDGET (dialog_file));
	gtk_button_set_label (GTK_BUTTON(bt_file_save),"Save");

}

/**
 * @brief Hide dialog_error when bt_error_ok clicked.
 */
G_MODULE_EXPORT void
bt_error_ok_clicked_cb( GtkButton *button,
            gpointer   data )
{

	gtk_widget_hide (GTK_WIDGET (dialog_error));

}

/**
 * @brief Hide dialog_about when aboutdialog1_button clicked.
 */
G_MODULE_EXPORT void
aboutdialog1_button_press_event_cb( GtkAboutDialog *dialog,
            gpointer   data )
{

	gtk_widget_hide (GTK_WIDGET (dialog_about));

}

/**
 * @brief Set bt_file_save widget sensetive.
 */
G_MODULE_EXPORT void
filechooserdialog1_current_folder_changed_cb( GtkFileChooser *file_chooser,
            gpointer   data )
{

	gtk_widget_set_sensitive (GTK_WIDGET(bt_file_save), TRUE);

}

/**
 * @brief Hide dialog_error_port when bt_ok_error_port clicked.
 */
G_MODULE_EXPORT void
bt_ok_error_port_clicked_cb( GtkFileChooser *file_chooser,
            gpointer   data )
{

	gtk_widget_hide (GTK_WIDGET (dialog_error_port));

}

/**
 * @brief Show dialog_list when bt_edit clicked.
 */
G_MODULE_EXPORT void
bt_edit_clicked_cb(  GtkButton *button,
            gpointer   data )
{

	gtk_widget_show_all (GTK_WIDGET(dialog_list));

}

/**
 * @brief Save settings when list was edited.
 */
G_MODULE_EXPORT void
bt_list_ok_clicked_cb(  GtkButton *button,
            gpointer   data )
{

	guint len, i;
	GError *error = NULL;
	gsize bytes_written;
	gchar msg[30];
	gchar header[] = "Name;S1_Pos;S2_Pos;S3_Pos;S4_Pos;S5_Pos;S6_Pos\n";

	gtk_widget_hide (GTK_WIDGET (dialog_list));

	channel = g_io_channel_new_file (filename, "w+", NULL);
	g_io_channel_write_chars (channel, header ,-1,&bytes_written,&error);

	if(articles!=NULL){
		len = articles->len;

		for(i=0;i<len;i++){

			g_sprintf(msg,"%s;%02d;%02d;%02d;%02d;%02d;%02d\n",
								g_array_index (articles, Item, i).name,
								g_array_index (articles, Item, i).s1_pos,
								g_array_index (articles, Item, i).s2_pos,
								g_array_index (articles, Item, i).s3_pos,
								g_array_index (articles, Item, i).s4_pos,
								g_array_index (articles, Item, i).s5_pos,
								g_array_index (articles, Item, i).s6_pos);

			g_io_channel_write_chars (channel,msg ,-1,&bytes_written,&error);
		}
		g_io_channel_shutdown (channel,TRUE,&error);
	}
}

/**
 * @brief When user will open file change button label.
 */
G_MODULE_EXPORT  void
filechooserdialog1_file_activated_cb(  GtkFileChooser *chooser,
            gpointer   data )
{

	gtk_button_set_label (GTK_BUTTON(bt_file_save),"Open");

}

/**
 * @brief Close dialog_name_exists when bt_name_exists_ok was clicked.
 */
G_MODULE_EXPORT void
bt_name_exists_ok_clicked_cb(  GtkWindow *window,
            gpointer   data )
{

	gtk_widget_hide (GTK_WIDGET (dialog_name_exists));

}

/**
 * @brief Destroy window and quit main.
 */
G_MODULE_EXPORT void
window1_delete_event_cb(  GtkWindow *window,
            gpointer   data )
{

  	g_free (filename);
  	gtk_window_set_destroy_with_parent (window,TRUE);
	gtk_main_quit();

}

/** EOF */
