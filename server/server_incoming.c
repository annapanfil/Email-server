/*Accepts connections from multiple clients, adds sender to the address book, read receiver's address from it and passes it to the other server. */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <pthread.h>
#include <errno.h>

#include "mail.h"
#include "config.h"
#include "server_base.c"
#include "user.h"
#include "server_incoming_inner.c"
#include "server_incoming_world.c"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


Mailbox* add_to_mailbox(Mail mail, char* username){
  printf("\n\e[0;36mⓘ Add to mailbox\e[m\n");
  Mailbox* mailbox = find_mailbox(username, true);
  RcvdMail* new_mail = (RcvdMail*) malloc(sizeof(RcvdMail));
  new_mail->mail = *(Mail*) malloc(sizeof(mail));
  strcpy(new_mail->mail.topic, mail.topic);
  // new_mail->mail = mail;
  new_mail->next = mailbox->mails;
  mailbox->mails = new_mail;

  return mailbox;
}


void* get_mail(void *arg)
{
  int new_socket = *((int *)arg);
  Mail mail;
  recv(new_socket, &mail, sizeof(mail), 0);

  printf("Got message: \"%s\"\n", mail.topic);
  char* recipient = get_recipient(&mail);
  Mailbox* mailbox = add_to_mailbox(mail, recipient);
  printf("Added to %s's mailbox: %s\n", recipient, mailbox->mails->mail.topic);
  return 0;
}


void* mail_server(void* arg){
  int socket = *((int *)arg);
  server_listen(socket, get_mail);
  return 0;
}


int main(){
  int world_socket = create_server_socket(SERVER_IN_ADDR, SERVER_IN_PORT_WORLD);
  int mail_socket = create_server_socket(SERVER_IN_ADDR, SERVER_IN_PORT_MAIL);
  int inner_socket = create_server_socket(SERVER_IN_ADDR, SERVER_IN_PORT_INNER);

  pthread_t mail_thread_id;
  if(pthread_create(&mail_thread_id, NULL, mail_server, &mail_socket) != 0 )
     printf("Failed to create thread mail\n");

  pthread_detach(mail_thread_id);

  pthread_t inner_thread_id;
  if(pthread_create(&inner_thread_id, NULL, inner_server, &inner_socket) != 0 )
     printf("Failed to create thread inner\n");

  pthread_detach(inner_thread_id);

  server_listen(world_socket, give_mails);
  return 0;
}
