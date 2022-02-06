#include <gtk/gtk.h>
#include <string.h>
#include "../server/config.h"
#include "../server/mail.h"
#include "../server/server_base.c"
#include "../server/user.h"

#define LABEL_NUM 8 
#define REGISTER_LENGTH 400
#define REGISTER_WIDTH 600

static GtkWidget *window;
extern int server_out_user_socket;

typedef struct rWindow{
	GtkWidget *account_edit, *passwd_edit;
}rWindow;




static void verify(GtkWidget *widget,gpointer data)
{

	rWindow *p = (rWindow*)data;
	typedef const gchar* pgchar;
	pgchar account, passwd;
	GtkWidget *dialog;
	account = gtk_entry_get_text(GTK_ENTRY(p->account_edit));
	passwd = gtk_entry_get_text(GTK_ENTRY(p->passwd_edit));
	if (!strlen(account) || !strlen(passwd))
	{
		//MessageBox not complete
		dialog = gtk_message_dialog_new(GTK_WINDOW(window), 
				GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING,
				GTK_BUTTONS_CLOSE, "Information doesn't complete!");
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		return;
	}


	User user = {.id = 1, .username="", .password=""};
    	Feedback feedback;
	strcpy(user.username, account);
	strcpy(user.password, passwd);
	send(server_out_user_socket, &user, sizeof(user), 0);
	bzero(&feedback, sizeof(feedback));
	recv(server_out_user_socket, &feedback, sizeof(feedback), 0);
	if (feedback.feedback==1)
	{
		dialog = gtk_message_dialog_new(GTK_WINDOW(window), 
				GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING,
				GTK_BUTTONS_CLOSE, "Account has already been registered!");
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		return;
	}

	if (feedback.feedback==0)
	{
		dialog = gtk_message_dialog_new(GTK_WINDOW(window), 
				GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING,
				GTK_BUTTONS_CLOSE, "Register successed!");
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
	}
	gtk_widget_hide_all(window);
}

void register_account(GtkWidget *button, gpointer data)
{
	int i;
	GtkWidget *vbox;
	GtkWidget *widget_label[LABEL_NUM];
	GtkWidget *hbox[LABEL_NUM + 1], *submit_button;
	rWindow rwindow;

	char *label[LABEL_NUM] = {
		"Account", "Password"
	};

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window), "delete_event",
					 G_CALLBACK(gtk_main_quit), NULL);

	gtk_window_set_title(GTK_WINDOW(window), "register");
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
	//account
	rwindow.account_edit = gtk_entry_new_with_max_length(50);
	gtk_box_pack_start(GTK_BOX(hbox[0]), rwindow.account_edit, FALSE, FALSE, 5);

	//passwd
	rwindow.passwd_edit = gtk_entry_new_with_max_length(50);
	gtk_box_pack_start(GTK_BOX(hbox[1]), rwindow.passwd_edit, FALSE, FALSE, 5);
	gtk_entry_set_visibility(GTK_ENTRY(rwindow.passwd_edit), FALSE);

	submit_button = gtk_button_new_with_label("submit");
	gtk_box_pack_start(GTK_BOX(hbox[8]), submit_button, FALSE, FALSE, 5);
	g_signal_connect(G_OBJECT(submit_button), "clicked",
							  G_CALLBACK(verify), (gpointer)&rwindow);
	gtk_widget_show_all(window);
	gtk_main();
}

