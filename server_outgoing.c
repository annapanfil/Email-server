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

char client_message[2000];
char buffer[1024];
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
  int server_socket, new_socket;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  //Configure server address
  struct sockaddr_in serverAddr = {.sin_family = AF_INET, .sin_port = htons(SERVER_OUT_PORT), .sin_addr.s_addr = inet_addr(SERVER_OUT_ADDR)};
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  //Create the socket
  server_socket = socket(PF_INET, SOCK_STREAM, 0);
  if (server_socket == -1){
    printf("Error creating socket");
    exit (EXIT_FAILURE);
  }

  //Bind the address struct to the socket
  int res = bind(server_socket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
  if (res == -1){
    printf("Cannot bind [Errno: %d]\n", errno);
    exit(EXIT_FAILURE);
  }

  //Listen on the socket
  if(listen(server_socket, 50) == 0)
    printf("Listening\n");
  else
    printf("Error\n");

  pthread_t thread_id;

  while(1)
  {
      //Accept call creates a new socket for the incoming connection
      addr_size = sizeof serverStorage;
      new_socket = accept(server_socket, (struct sockaddr *) &serverStorage, &addr_size);

      if( pthread_create(&thread_id, NULL, mail_service, &new_socket) != 0 )
         printf("Failed to create thread\n");

      pthread_detach(thread_id);
  }
  return 0;
}
