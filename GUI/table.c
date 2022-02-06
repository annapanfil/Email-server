#include <gtk/gtk.h>
#include <stdlib.h>
#define WINDOW_LENGTH 320
#define WINDOW_WIDTH 300


typedef struct user_window {
	GtkWidget *account_edit, *passwd_edit;
}user_window;
static GtkWidget *window;
struct sockaddr_in server_addr;
int server_out_mail_socket, server_out_user_socket,server_in_socket;


void user_account(GtkWidget *button, gpointer data,User user)
{
	int i;
	GtkWidget *vbox;
	GtkWidget *widget_label[LABEL_NUM];
	GtkWidget *hbox[LABEL_NUM + 1], *inbox_button,*sent_button,*logout_button;
	user_window user_window;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window), "delete_event",
					 G_CALLBACK(gtk_main_quit), NULL);

	gtk_window_set_title(GTK_WINDOW(window), "User");
	gtk_window_set_default_size(GTK_WINDOW(window), 
				REGISTER_LENGTH, REGISTER_WIDTH);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window), 20);
	gtk_window_set_modal(GTK_WINDOW(window), TRUE);

	vbox = gtk_vbox_new(TRUE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	//submit
	sent_button = gtk_button_new_with_label("Inbox");
	gtk_box_pack_start(GTK_BOX(hbox[8]), inbox_button, FALSE, FALSE, 5);
	g_signal_connect(G_OBJECT(inbox_button), "clicked",
							  G_CALLBACK(verify), NULL);
	sent_button = gtk_button_new_with_label("Sent");
	gtk_box_pack_start(GTK_BOX(hbox[8]), sent_button, FALSE, FALSE, 5);
	g_signal_connect(G_OBJECT(sent_button), "clicked",
							  G_CALLBACK(verify), NULL);
	logout_button = gtk_button_new_with_label("Logout");
	gtk_box_pack_start(GTK_BOX(hbox[8]), logout_button, FALSE, FALSE, 5);
	g_signal_connect(G_OBJECT(logout_button), "clicked",
							  G_CALLBACK(verify), NULL);

	gtk_widget_show_all(window);
	gtk_main();
}

