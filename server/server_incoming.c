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

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
Mail* mails[MAX_CLIENTS];

void add_to_mailbox(Mail mail){
  //TODO
}


void* get_mail(void *arg)
{
  int new_socket = *((int *)arg);
  int n;
  Mail mail;
  for(;;){
    n=recv(new_socket, &mail, 2000, 0);
    printf("Got message: \"%s\"\n", mail.topic);
    if(n<1){
        break;
    }
  add_to_mailbox(mail);
  }
  return 0;
}


int main(){
  //TODO: add world_socket
  int inner_socket = create_server_socket(SERVER_IN_ADDR, SERVER_IN_PORT_INNER);
  server_listen(inner_socket, get_mail);
  return 0;
}
