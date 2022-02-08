#include <gtk/gtk.h>
#include <stdlib.h>
#include "../server/config.h"
#include "../server/mail.h"
#include "../server/server_base.c"
#include "../server/user.h"
#define CONTENT_LENGTH 500
#define CONTENT_WIDTH  480

static int selected_row = -1;
static const gchar *recv_account;
extern int sfd;
extern int server_in_socket;

typedef struct {
	GtkWidget *outbox_window;
	GtkWidget *outbox_show_button;
	GtkWidget *outbox_hbox1,*outbox_hbox2,*outbox_hbox3,*outbox_vbox1,*outbox_vbox2,*outbox_vbox3,*outbox_hbox4, *outbox_trash_button;
	GtkWidget *outbox_clist,*outbox_user_label,*outbox_scrolled,*outbox_hbox5,*outbox_hbox6;
}recvWindow;

void refresh_list(recvWindow *p)
{
	gchar tmpstr1[64], tmpstr2[64], tmpstr3[64], tmpstr4[64];
	gchar *infostr[4] = {tmpstr1, tmpstr2, tmpstr3, tmpstr4};
	Feedback feedback;
        send(server_in_socket, &user.username, sizeof(user.username), 0);
            printf("sent\n");
        recv(server_in_socket, &feedback, sizeof(feedback), 0);
          printf("Feedback %d %s\n", feedback.feedback, feedback.message);
          gtk_clist_unselect_all(GTK_CLIST(p->outbox_clist));
	gtk_clist_clear(GTK_CLIST(p->outbox_clist));
          if (feedback.feedback == 0){
                    Mail mail_rcv;
 		    strcpy(mail_rcv.to,user.username);
                    printf("Reading mails...\n");

                    while(strcmp(mail_rcv.to, "STOP") != 0){
                              if (recv(server_in_socket, &mail_rcv, sizeof(mail_rcv), 0) > 0 && (strcmp(mail_rcv.to, "STOP") != 0)){
                                sprintf(infostr[0], "%s", mail_rcv.topic);
				sprintf(infostr[1], "%s", mail_rcv.from);
				gtk_clist_append(GTK_CLIST(p->outbox_clist), infostr);
      }
	//gtk_clist_freeze(GTK_CLIST(p->outbox_clist));
	gtk_clist_unselect_all(GTK_CLIST(p->outbox_clist));
	gtk_clist_clear(GTK_CLIST(p->outbox_clist));
}

void show_recv_window(GtkWidget *widget, gpointer data)
{
	if (selected_row == -1 )
		return;
	gchar *topic,*from,*content;	
	GtkWidget *window,*label1,*vbox,*label2,*label4;
	recvWindow *p = (recvWindow*)data;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window), "delete_event",
					 G_CALLBACK(gtk_main_quit), NULL);
	gtk_window_set_title(GTK_WINDOW (window),"Mail content");
	gtk_window_set_default_size(GTK_WINDOW(window), CONTENT_LENGTH, CONTENT_WIDTH);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_modal(GTK_WINDOW(window), TRUE);
	gtk_widget_show(window);

	gtk_clist_get_text(GTK_CLIST(p->outbox_clist),selected_row,0,&topic);
	gtk_clist_get_text(GTK_CLIST(p->outbox_clist),selected_row,1,&from);

//	c2s_t snd_msg;
//	s2c_view_t rcv_msg;
//	snd_msg.type = C2S_SHOWCONTENT;
//	snd_msg.mailtype = C2S_RECV_MAIL;
//	strcpy(snd_msg.info.account, recv_account);
//	strcpy(snd_msg.mail.date, date);
//	send(sfd, &snd_msg, sizeof(snd_msg), 0);
//	recv(sfd, &rcv_msg, sizeof(rcv_msg), 0);
//	content = rcv_msg.mail.content;

	label1 = gtk_label_new(topic);
	label2 = gtk_label_new(from);
	label4 = gtk_label_new(content);

	vbox = gtk_vbox_new(FALSE,20);
	gtk_box_pack_start(GTK_BOX(vbox), label1, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), label2, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), label4, FALSE, FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(vbox));
	gtk_widget_show_all(window);
	gtk_main();
}


void unget_recv_row(GtkCList *clist,gint row,gint column,GdkEventButton *event,gpointer user_data)
{
	selected_row = -1;	
}

void get_recv_row(GtkCList *clist,gint row,gint column,GdkEventButton *event,gpointer user_data){
	selected_row = row;
}

void quit_recvwindow(GtkWidget *widget, gpointer data)
{

	gtk_widget_hide_all(((recvWindow*)data)->outbox_window);
}

void recv_mail(const gchar *account)
{
	recvWindow rwindow;
	gchar *title[] = {"Title","Sender","Date"};
	recv_account = account;
	
	rwindow.outbox_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW (rwindow.outbox_window),"Inbox");
	gtk_widget_set_usize((rwindow.outbox_window),800,650);
	gtk_widget_set_uposition((rwindow.outbox_window),200,200);
	gtk_window_set_modal(GTK_WINDOW(rwindow.outbox_window), TRUE);
	g_signal_connect(G_OBJECT(rwindow.outbox_window), "delete_event",
					 G_CALLBACK(gtk_main_quit), (gpointer)&rwindow);

	gchar welcome[60];
	sprintf(welcome, "Hello, %s", account);
	rwindow.outbox_user_label = gtk_accel_label_new (welcome);
	rwindow.outbox_show_button = gtk_button_new_with_label("Show");
	rwindow.outbox_scrolled = gtk_scrolled_window_new(NULL,NULL);
	rwindow.outbox_hbox1 = gtk_hbox_new(TRUE,0);
	     
	rwindow.outbox_hbox3 = gtk_hbox_new(TRUE,20);
	gtk_box_pack_start(GTK_BOX(rwindow.outbox_hbox3), rwindow.outbox_show_button, FALSE, FALSE, 0);
	
	rwindow.outbox_hbox5 = gtk_hbox_new(TRUE,0);
	gtk_container_set_border_width(GTK_CONTAINER(rwindow.outbox_hbox5), 20);
	gtk_box_pack_start(GTK_BOX(rwindow.outbox_hbox5),rwindow.outbox_hbox1,FALSE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(rwindow.outbox_hbox5),rwindow.outbox_hbox2,TRUE,TRUE,0);
	
	rwindow.outbox_hbox6 = gtk_hbox_new(TRUE,0);	
	gtk_container_set_border_width(GTK_CONTAINER(rwindow.outbox_hbox6), 5);
	gtk_box_pack_start(GTK_BOX(rwindow.outbox_hbox6),rwindow.outbox_hbox3,FALSE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(rwindow.outbox_hbox6),rwindow.outbox_hbox4,TRUE,TRUE,0);

	rwindow.outbox_vbox1 = gtk_vbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(rwindow.outbox_vbox1),rwindow.outbox_hbox5,FALSE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(rwindow.outbox_vbox1),rwindow.outbox_hbox6,TRUE,TRUE,0);

	rwindow.outbox_vbox2 = gtk_vbox_new(FALSE,0);
	rwindow.outbox_clist = gtk_clist_new_with_titles(3,title);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(rwindow.outbox_scrolled),GTK_POLICY_NEVER,GTK_POLICY_ALWAYS);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW(rwindow.outbox_scrolled),rwindow.outbox_clist);
	gtk_clist_set_column_width (GTK_CLIST(rwindow.outbox_clist),0,150);
	gtk_clist_set_column_width (GTK_CLIST(rwindow.outbox_clist),1,150);
	gtk_clist_set_column_width (GTK_CLIST(rwindow.outbox_clist),2,150);
	gtk_box_pack_start(GTK_BOX(rwindow.outbox_vbox2),rwindow.outbox_scrolled,TRUE,TRUE,20);
	
	rwindow.outbox_vbox3 = gtk_vbox_new(FALSE,0);
	gtk_box_pack_start(GTK_BOX(rwindow.outbox_vbox3),rwindow.outbox_vbox1,FALSE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(rwindow.outbox_vbox3),rwindow.outbox_vbox2,TRUE,TRUE,0);
	gtk_container_add(GTK_CONTAINER(rwindow.outbox_window), GTK_WIDGET(rwindow.outbox_vbox3));

	g_signal_connect(G_OBJECT(rwindow.outbox_show_button),"clicked",G_CALLBACK(show_recv_window), (gpointer)&rwindow);
	g_signal_connect(G_OBJECT(rwindow.outbox_clist),"select_row",G_CALLBACK(get_recv_row), NULL);
	g_signal_connect(G_OBJECT(rwindow.outbox_clist), "unselect_row", G_CALLBACK(unget_recv_row), NULL);
	refresh_list(&rwindow);

	gtk_widget_show_all(rwindow.outbox_window);
	gtk_main();
}
