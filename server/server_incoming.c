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

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
Mailbox mailboxes[MAX_CLIENTS];
int mailboxes_num;


char* get_recipient(Mail* mail){
  char* recipient;
  recipient = strdup(mail->to);
  recipient = strsep(&recipient, "@");
  // printf("%s -> %s\n", mail->to, recipient);

  return recipient;
}

Mailbox* find_mailbox(char* username){
  //find existing mailbox...
  for (int i=0; i<=mailboxes_num; i++){
      if (strcmp(username, mailboxes[i].username) == 0){
        printf("Found mailbox\n");
        return mailboxes + i;
      }
  }

  //...or create a new one
  printf("Create a new one\n");
  Mailbox* mailbox = (Mailbox*) malloc(sizeof(Mailbox));
  strcpy(mailbox->username, username);
  mailbox->mails = NULL;
  mailboxes[mailboxes_num] = *mailbox;
  mailboxes_num += 1; //TODO: mutex?
  return mailboxes + mailboxes_num;
}


Mailbox* add_to_mailbox(Mail mail, char* username){
  printf("\n\e[0;36mⓘ Add to mailbox\e[m\n");
  Mailbox* mailbox = find_mailbox(username);
  RcvdMail* new_mail = (RcvdMail*) malloc(sizeof(RcvdMail));
  new_mail->mail = mail;
  new_mail->next = mailbox->mails;
  mailbox->mails = new_mail;

  return mailbox;
}


void* get_mail(void *arg)
{
  int new_socket = *((int *)arg);
  Mail mail;  //TODO: malloc?
  recv(new_socket, &mail, sizeof(mail), 0);

  printf("Got message: \"%s\"\n", mail.topic);
  char* recipient = get_recipient(&mail);
  Mailbox* mailbox = add_to_mailbox(mail, recipient);
  printf("Added to %s's mailbox: %s\n", recipient, mailbox->mails->mail.topic);
  return 0;
}


int main(){
  //TODO: add world_socket
  int inner_socket = create_server_socket(SERVER_IN_ADDR, SERVER_IN_PORT_INNER);
  server_listen(inner_socket, get_mail);
  return 0;
}
