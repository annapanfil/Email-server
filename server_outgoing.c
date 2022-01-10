/*Accepts connections from multiple clients, adds sender to the address book, read receiver's address from it and passes it to the other server. */

//TODO: create other_server_socket

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

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* mail_service(void *arg)
{
  printf("new thread \n");
  int new_socket = *((int *)arg);
  int n;
  Mail mail;
  for(;;){
    n=recv(new_socket, &mail, 2000, 0);
    printf("topic: %s\n%s\n", mail.topic, mail.text);
        if(n<1){
            break;
        }

    // Here normal server would check receiver's address domain and choose proper incoming server, but since we've got only one, we pass it there.

    // send(other_server_socket, mail, sizeof(mail), 0);
    send(new_socket, &mail, sizeof(mail), 0); //return to client
    }
    printf("Exit socketThread \n");

    pthread_exit(NULL);
}


int main(){
  base(SERVER_OUT_ADDR, SERVER_OUT_PORT, mail_service)
  return 0;
}
