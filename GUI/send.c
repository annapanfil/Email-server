#include <gtk/gtk.h>
#include <string.h>

#define LABEL_NUM 8 
#define REGISTER_LENGTH 400
#define REGISTER_WIDTH 600

static GtkWidget *window;
extern int server_out_mail_socket;

typedef struct sWindow{
	GtkWidget *receiver_edit, *topic_edit,*text_edit;
}sWindow;


  Mail mail = {.from = "", .to = "", .topic = "", .text = ""};

static void verify_mail(GtkWidget *widget,gpointer data)
{

	sWindow *p = (sWindow*)data;
	typedef const gchar* pgchar;
	pgchar receiver, topic,text;
	GtkWidget *dialog;
	receiver = gtk_entry_get_text(GTK_ENTRY(p->receiver_edit));
	topic = gtk_entry_get_text(GTK_ENTRY(p->topic_edit));
	text = gtk_entry_get_text(GTK_ENTRY(p->text_edit));
	if (!strlen(receiver) || !strlen(topic)||!strlen(text))
	{
		//MessageBox not complete
		dialog = gtk_message_dialog_new(GTK_WINDOW(window), 
				GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING,
				GTK_BUTTONS_CLOSE, "Information isn't complete!");
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		return;
	}



    	Feedback feedback;

	strcpy(mail.to, receiver);
	strcpy(mail.topic, topic);
	strcpy(mail.text, text);
	send(server_out_mail_socket, &mail, sizeof(mail), 0);
	bzero(&feedback, sizeof(feedback));
	recv(server_out_mail_socket, &feedback, sizeof(feedback), 0);
	if (feedback.feedback==1)
	{
		dialog = gtk_message_dialog_new(GTK_WINDOW(window), 
				GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING,
				GTK_BUTTONS_CLOSE, "Wrong address!");
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		return;
	}

	if (feedback.feedback==0)
	{
		dialog = gtk_message_dialog_new(GTK_WINDOW(window), 
				GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_WARNING,
				GTK_BUTTONS_CLOSE, "Mail succesfully sent!");
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
	}
	gtk_widget_hide_all(window);

}

void sendmail(User user)
{
    	strcpy(mail.from, user.username);
	int i;
	GtkWidget *vbox;
	GtkWidget *widget_label[LABEL_NUM];
	GtkWidget *hbox[LABEL_NUM + 1], *submit_button;
	sWindow swindow;

	char *label[LABEL_NUM] = {
		"Receiver", "Topic", "Text"
	};

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window), "delete_event",
					 G_CALLBACK(gtk_main_quit), NULL);

	gtk_window_set_title(GTK_WINDOW(window), "Sending Mail");
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
	swindow.receiver_edit = gtk_entry_new_with_max_length(50);
	gtk_box_pack_start(GTK_BOX(hbox[0]), swindow.receiver_edit, FALSE, FALSE, 5);

	//passwd
	swindow.topic_edit = gtk_entry_new_with_max_length(50);
	gtk_box_pack_start(GTK_BOX(hbox[1]), swindow.topic_edit, FALSE, FALSE, 5);
	
	swindow.text_edit = gtk_entry_new_with_max_length(50);
	gtk_box_pack_start(GTK_BOX(hbox[2]), swindow.text_edit, FALSE, FALSE, 5);

	submit_button = gtk_button_new_with_label("Send");
	gtk_box_pack_start(GTK_BOX(hbox[8]), submit_button, FALSE, FALSE, 5);
	g_signal_connect(G_OBJECT(submit_button), "clicked",
							  G_CALLBACK(verify_mail), (gpointer)&swindow);
	gtk_widget_show_all(window);
	gtk_main();
}
