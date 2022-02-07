#include <gtk/gtk.h>
#include <stdlib.h>
#include "reg.c"
#include "table.c"
#define WINDOW_LENGTH 320
#define WINDOW_WIDTH 300




typedef struct {
	GtkWidget *account_edit, *passwd_edit;
}login_window;
static GtkWidget *window;
struct sockaddr_in server_addr;
 int server_out_mail_socket, server_out_user_socket,server_in_socket;
int z=0;
User user = {.id = 2, .username="", .password=""};
Feedback feedback;




static void login_verify(GtkWidget *widget, gpointer data)
{
	login_window *lwindow = (login_window*)data;
	const gchar *account, *passwd;
	GtkWidget *dialog;
	account = gtk_entry_get_text(GTK_ENTRY(lwindow->account_edit));
	passwd = gtk_entry_get_text(GTK_ENTRY(lwindow->passwd_edit));
	if (strlen(account) == 0 || strlen(passwd) == 0)
	{
		dialog = gtk_message_dialog_new(GTK_WINDOW(window), 
				GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING,
				GTK_BUTTONS_CLOSE, "Account/passwd is not completed!");
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		return;
	}

	strcpy(user.username, account);
	strcpy(user.password, passwd);
	send(server_out_user_socket, &user, sizeof(user), 0);
	bzero(&feedback, sizeof(feedback));
	recv(server_out_user_socket, &feedback, sizeof(feedback), 0);
	if (feedback.feedback == 1)
	{
		dialog = gtk_message_dialog_new(GTK_WINDOW(window), 
				GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING,
				GTK_BUTTONS_CLOSE, "Login is incorrect!");
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		return;
	}
	if (feedback.feedback == 2)
	{
		dialog = gtk_message_dialog_new(GTK_WINDOW(window), 
				GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING,
				GTK_BUTTONS_CLOSE, "Password is incorrect!");
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		return;
	}
	if (feedback.feedback == 0)
	{
		dialog = gtk_message_dialog_new(GTK_WINDOW(window), 
				GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING,
				GTK_BUTTONS_CLOSE, "Login success!");
		gtk_dialog_run(GTK_DIALOG(dialog));

		gtk_widget_destroy(dialog);
		z=1;
			user_account(user);
		
	}
}

static void delete_window(GtkWidget *widget, gpointer data)
{
	exit(0);
}

int main(int argc, char *argv[])
{
	GtkWidget *register_button, *login_button, *enter_button,*dialog;
	GtkWidget *account_box, *passwd_box, *button_box;
	GtkWidget *account_label, *passwd_label;
	GtkWidget *vbox;
	login_window lwindow;

	//Create window
	gtk_init(&argc, &argv);
	

  	create_socket(SERVER_OUT_ADDR, SERVER_OUT_PORT_MAIL, &server_addr, &server_out_mail_socket);

  	connect(server_out_mail_socket, (struct sockaddr *) &server_addr, sizeof server_addr);

  	create_socket(SERVER_OUT_ADDR, SERVER_OUT_PORT_USER, &server_addr, &server_out_user_socket);

  	connect(server_out_user_socket, (struct sockaddr *) &server_addr, sizeof server_addr);

  	create_socket(SERVER_IN_ADDR, SERVER_IN_PORT_PULL_MAIL, &server_addr, &server_in_socket);

  	connect(server_in_socket, (struct sockaddr *) &server_addr, sizeof server_addr);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	//Close window
	g_signal_connect(G_OBJECT(window), "delete_event",
					 G_CALLBACK(delete_window), NULL);
	/*Window attribute*/
	gtk_window_set_title(GTK_WINDOW(window), "Sign in");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window), 20);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	gtk_window_set_default_size(GTK_WINDOW(window), 
								WINDOW_LENGTH, WINDOW_WIDTH);

	/*First-floor box*/
	//Set largest box
	vbox = gtk_vbox_new(TRUE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	//Push account box
	account_box = gtk_hbox_new(TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), account_box, FALSE, FALSE, 40);
	//Push password box
	passwd_box  = gtk_hbox_new(TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), passwd_box, FALSE, FALSE, 40);
	//Push button box
	button_box  = gtk_hbox_new(TRUE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), button_box, FALSE, FALSE, 40);

	/*Second-floor box*/
	//Set account box
	gchar *markup = "<span foreground=\"red\">account</span>";
	account_label = gtk_label_new("account");
	gtk_label_set_markup(GTK_LABEL(account_label), markup);
	gtk_box_pack_start(GTK_BOX(account_box), account_label, FALSE, FALSE, 5);
	lwindow.account_edit = gtk_entry_new_with_max_length(50);
	gtk_box_pack_start(GTK_BOX(account_box), lwindow.account_edit, FALSE, FALSE, 5);

	//Set password box
	markup = "<span foreground=\"red\">password</span>";
	passwd_label  = gtk_label_new("password");
	gtk_label_set_markup(GTK_LABEL(passwd_label), markup);
	gtk_box_pack_start(GTK_BOX(passwd_box), passwd_label, FALSE, FALSE, 5);
	lwindow.passwd_edit = gtk_entry_new_with_max_length(50);
	gtk_entry_set_visibility(GTK_ENTRY(lwindow.passwd_edit), FALSE);
	gtk_box_pack_start(GTK_BOX(passwd_box), lwindow.passwd_edit, FALSE, FALSE, 5);

	//Set button box
	login_button = gtk_button_new_with_label("Login");
	gtk_box_pack_start(GTK_BOX(button_box), login_button, FALSE, FALSE, 5);
	g_signal_connect(G_OBJECT(login_button), "clicked",
					 G_CALLBACK(login_verify), (gpointer)&lwindow);
	register_button = gtk_button_new_with_label("Register");
	gtk_box_pack_start(GTK_BOX(button_box), register_button, FALSE, FALSE, 5);
	g_signal_connect(G_OBJECT(register_button), "clicked",
					 G_CALLBACK(register_account), NULL);
	

	/*Show*/
	gtk_widget_show_all(window);
	gtk_main();

	close(server_out_mail_socket);
  	close(server_out_user_socket);
  	close(server_in_socket);
			  
	return 0;
}

