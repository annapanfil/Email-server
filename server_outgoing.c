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
#include "mail.h"

char client_message[2000];
char buffer[1024];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void * mail_service(void *arg)
{
  printf("new thread \n");
  int newSocket = *((int *)arg);
  int n;
  for(;;){
    n=recv(newSocket , client_message , 2000 , 0);
    printf("%s\n",client_message);
        if(n<1){
            break;
        }

    char *message = malloc(sizeof(client_message));
    strcpy(message,client_message);

    send(newSocket,message,sizeof(message),0);
    memset(&client_message, 0, sizeof (client_message));

    }
    printf("Exit socketThread \n");

    pthread_exit(NULL);
}

int main(){
  int serverSocket, newSocket;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  //Configure server address
  struct sockaddr_in serverAddr = {.sin_family = AF_INET, .sin_port = htons(1100), .sin_addr.s_addr = htonl(INADDR_ANY)}; // localhost, port 1100
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  //Create the socket
  serverSocket = socket(PF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1){
    printf("Error creating socket");
    exit (EXIT_FAILURE);
  }

  //Bind the address struct to the socket
  int res = bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
  if (res == -1){
    printf("Cannot bind\n");
    exit(EXIT_FAILURE);
  }

  //Listen on the socket
  if(listen(serverSocket, 50) == 0)
    printf("Listening\n");
  else
    printf("Error\n");

  pthread_t thread_id;

  while(1)
  {
      //Accept call creates a new socket for the incoming connection
      addr_size = sizeof serverStorage;
      newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);

      if( pthread_create(&thread_id, NULL, mail_service, &newSocket) != 0 )
         printf("Failed to create thread\n");

      pthread_detach(thread_id);
  }
  return 0;
}
