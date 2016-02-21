/*
 * list.c
 *
 *  Created on: Sep 02, 2015
 *      Author: Ney Fränz
 */

/******************************************************************* INCLUDES */
#include "list.h"
/*********************************************************** GLOBAL VARIABLES */
extern GtkWidget 		*window;
extern GtkWidget 		*entry_name_position;
extern GtkDialog 		*dialog_error, *dialog_name_exists;
extern GtkTreeView 		*treeview;
extern GtkListStore 	*model;
extern GtkAdjustment  	*adjustment1,*adjustment2,*adjustment3,*adjustment4,
						*adjustment5,*adjustment6;
extern GArray 			*articles;
extern GIOChannel 		*channel;
extern gchar 			*filename, *name;
extern Item 			pos;
/******************************************************************** DEFINES */

/******************************************************** FUNCTION PROTOTYPES */

/**
 * @brief Save Position.
 */
G_MODULE_EXPORT void
bt_save_position_clicked_cb( GtkButton *button,
            gpointer   data )
{
	const gchar *position_name;
	char  		*str;
	GtkTreeIter iter;
	guint 		len, i;
	GError 		*error = NULL;
	gsize 		bytes_written;
	gchar 		msg[30];
	gchar 		header[] = "Name;S1_Pos;S2_Pos;S3_Pos;S4_Pos;S5_Pos;S6_Pos\n";

	position_name = gtk_entry_get_text (GTK_ENTRY(entry_name_position));
	str = g_strdup (position_name);

	/*Check if Name already exist*/
	len = articles->len;
	for(i=0;i<len;i++){
		if(g_strcmp0((g_array_index (articles, Item, i).name) ,position_name)== 0){
			gtk_entry_set_text (GTK_ENTRY(entry_name_position), "");
			gtk_widget_show_all (GTK_WIDGET(dialog_name_exists));
			return;
		}
	}

	if(strlen(position_name) == 0){
		gtk_widget_show_all (GTK_WIDGET(dialog_error));
	}else{
		g_print("Position Name= %s\n",position_name);
		gtk_entry_set_text (GTK_ENTRY(entry_name_position), "");

		/*****************************fill structure*/
		pos.name = g_strdup (str);
		pos.s1_pos = (gint)gtk_adjustment_get_value(GTK_ADJUSTMENT(adjustment1));
		pos.s2_pos = (gint)gtk_adjustment_get_value(GTK_ADJUSTMENT(adjustment2));;
		pos.s3_pos = (gint)gtk_adjustment_get_value(GTK_ADJUSTMENT(adjustment3));;
		pos.s4_pos = (gint)gtk_adjustment_get_value(GTK_ADJUSTMENT(adjustment4));;
		pos.s5_pos = (gint)gtk_adjustment_get_value(GTK_ADJUSTMENT(adjustment5));;
		pos.s6_pos = (gint)gtk_adjustment_get_value(GTK_ADJUSTMENT(adjustment6));;

		gtk_list_store_append(GTK_LIST_STORE (model), &iter);

		/* Set the data for the new row */
  		gtk_list_store_set (GTK_LIST_STORE (model), &iter,
                      		COLUMN_ITEM_NAME, pos.name,
                      		COLUMN_ITEM_S1_POS, pos.s1_pos,
                     		COLUMN_ITEM_S2_POS, pos.s2_pos,
 							COLUMN_ITEM_S3_POS, pos.s3_pos,
                      		COLUMN_ITEM_S4_POS, pos.s4_pos,
 							COLUMN_ITEM_S5_POS, pos.s5_pos,
                      		COLUMN_ITEM_S6_POS, pos.s6_pos,
                      		-1);

		g_array_append_vals (articles, &pos, 1);

		channel = g_io_channel_new_file (filename, "w+", NULL);

		/********************************/
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
}

/**
 * @brief Remove item from list.
 */
G_MODULE_EXPORT void
bt_list_remove_clicked_cb (GtkButton *button,
            gpointer   data )
{
	GtkTreeIter  iter;
	GtkTreeSelection *selection = gtk_tree_view_get_selection (treeview);

	if (gtk_tree_selection_get_selected (selection, NULL, &iter))
    {
		GtkTreePath *path;
		gint i;

      	path = gtk_tree_model_get_path (GTK_TREE_MODEL(model), &iter);
      	i = gtk_tree_path_get_indices (path)[0];
      	gtk_list_store_remove (GTK_LIST_STORE (model), &iter);

		g_array_remove_index (articles, i);

      	gtk_tree_path_free (path);
    }
}

/**
 * @brief Open .csv file.
 */
void create_open_file(void){

	GError *error = NULL;
	GIOStatus ret;
	gchar *msg;
  	gsize len;
	gchar * pch;
	gint i=0;
	GtkTreeIter  iter;

	if(articles != NULL){
		g_print("fuck\n");
		g_array_free (articles,TRUE);
		//g_free(articles);
		articles = g_array_sized_new (FALSE, FALSE, sizeof (pos), 1);
    	gtk_list_store_clear(GTK_LIST_STORE (model));
	}

  	articles = g_array_sized_new (FALSE, FALSE, sizeof (pos), 1);

	/*Ignore first Line header*/
	ret = g_io_channel_read_line (channel, &msg, &len, NULL, &error);

	ret = g_io_channel_read_line (channel, &msg, &len, NULL, &error);
	if (ret == G_IO_STATUS_ERROR)
  		g_error ("Error reading: %s\n", error->message);

	while(ret != G_IO_STATUS_EOF ){
		pch = strtok (msg,";");
  		while (pch != NULL)  {
			switch(i++)
			{
			case 0:
				pos.name = g_strdup (pch);
			break;
			case 1:
				pos.s1_pos = atoi (pch);
			break;
			case 2:
				pos.s2_pos = atoi (pch);
			break;
			case 3:
				pos.s3_pos = atoi (pch);
			break;
			case 4:
				pos.s4_pos = atoi (pch);
			break;
			case 5:
				pos.s5_pos = atoi (pch);
			break;
			case 6:
				pos.s6_pos = atoi (pch);
			break;
			}
    	pch = strtok (NULL, ";\n");
		  }
		gtk_list_store_append(GTK_LIST_STORE (model), &iter);

		/* Set the data for the new row */
  		gtk_list_store_set (GTK_LIST_STORE (model), &iter,
                      		COLUMN_ITEM_NAME, pos.name,
                      		COLUMN_ITEM_S1_POS, pos.s1_pos,
                      		COLUMN_ITEM_S2_POS, pos.s2_pos,
 							COLUMN_ITEM_S3_POS, pos.s3_pos,
                      		COLUMN_ITEM_S4_POS, pos.s4_pos,
 							COLUMN_ITEM_S5_POS, pos.s5_pos,
                      		COLUMN_ITEM_S6_POS, pos.s6_pos,
                      		-1);

		g_array_append_vals (articles, &pos, 1);

		i = 0;
		ret = g_io_channel_read_line (channel, &msg, &len, NULL, &error);
		if (ret == G_IO_STATUS_ERROR)
  		g_error ("Error reading: %s\n", error->message);

	}
	g_free (msg);
}


/** EOF */
