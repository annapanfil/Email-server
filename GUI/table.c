#include <gtk/gtk.h>
#include <stdlib.h>

#define LABEL_NUM 8 
#define REGISTER_LENGTH 400
#define REGISTER_WIDTH 600


static GtkWidget *window;

User user_log = {.id = 3, .username="", .password=""};

static void logoutuser(){
	user_log.id=3;
	send(server_out_user_socket, &user_log, sizeof(user_log), 0);
		gtk_widget_hide_all(window);
			main();
	
}

static void sendus(){
	sendmail(user_log);
	
}


void user_account(User user)
{
	strcpy(user_log.username,user.username);
	strcpy(user_log.password, user.password);
	int i;
	GtkWidget *vbox;
	GtkWidget *widget_label[LABEL_NUM];
	GtkWidget *hbox[LABEL_NUM + 1], *inbox_button,*send_button,*logout_button;
	rWindow rwindow;

	char *label[LABEL_NUM] = {
		"Inbox", "Send","Logout"
	};

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window), "delete_event",
					 G_CALLBACK(gtk_main_quit), NULL);
	gtk_window_set_title(GTK_WINDOW(window), user_log.username);
	gtk_window_set_default_size(GTK_WINDOW(window), 
				REGISTER_LENGTH, REGISTER_WIDTH);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window), 20);
	gtk_window_set_modal(GTK_WINDOW(window), TRUE);
	

	vbox = gtk_vbox_new(TRUE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	
	for (i = 0; i < LABEL_NUM + 1; i++)
	{
		hbox[i] = gtk_hbox_new(TRUE, 0);
		gtk_box_pack_start(GTK_BOX(vbox), hbox[i], FALSE, FALSE, 5);
	}

	for (i = 0; i < LABEL_NUM; i++)
	{
		widget_label[i] = gtk_label_new(label[i]);
		gtk_box_pack_start(GTK_BOX(hbox[i]), widget_label[i], FALSE, FALSE, 5);
	}

	inbox_button = gtk_button_new_with_label("Inbox");
	gtk_box_pack_start(GTK_BOX(hbox[0]), inbox_button, FALSE, FALSE, 5);
	send_button = gtk_button_new_with_label("Send");
	gtk_box_pack_start(GTK_BOX(hbox[1]), send_button, FALSE, FALSE, 5);
	g_signal_connect(G_OBJECT(send_button), "clicked",
							  G_CALLBACK(sendus), NULL);
	logout_button = gtk_button_new_with_label("Logout");
	gtk_box_pack_start(GTK_BOX(hbox[2]), logout_button, FALSE, FALSE, 5);
	g_signal_connect(G_OBJECT(logout_button), "clicked",
							  G_CALLBACK(logoutuser), NULL);

							  
	gtk_widget_show_all(window);
	gtk_main();
	}

