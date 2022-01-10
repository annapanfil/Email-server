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
#include "server_base.c"
#include "user.c"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* mail_service(void *arg)
{
  int new_socket = *((int *)arg);
  int n;
  Mail mail;
  for(;;){  //QUESTION: czy to powinno być w pętli?
    n=recv(new_socket, &mail, 2000, 0);
    printf("Got message \"%s\". Passing to incoming server.\n", mail.topic);
    if(n<1){
        break;
    }

    // Here normal server would check receiver's address domain and choose proper incoming server, but since we've got only one, we pass it there.

    int other_server_socket;
    struct sockaddr_in server_addr;

    create_socket(SERVER_IN_ADDR, SERVER_IN_PORT, &server_addr, &other_server_socket); //TODO: przenieść do maina?
    connect(other_server_socket, (struct sockaddr *) &server_addr, sizeof server_addr);

    send(other_server_socket, &mail, sizeof(mail), 0);
    // send(new_socket, &mail, sizeof(mail), 0); //return to client TODO: delete
    }

    pthread_exit(NULL);
}

int main(){
  User users[100];
  int users_num = 0;
  User active_users[100];
  int active_users_num = 0;

  base(SERVER_OUT_ADDR, SERVER_OUT_PORT, mail_service);
  return 0;
}
