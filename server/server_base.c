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

void create_socket(const char* ip, const int port, struct sockaddr_in* address, int* new_socket){

  //Configure address
  address->sin_family = AF_INET;
  address->sin_port = htons(port);
  address->sin_addr.s_addr = inet_addr(ip);
  memset(address->sin_zero, '\0', sizeof address->sin_zero);

  //Create the socket
  *new_socket = socket(PF_INET, SOCK_STREAM, 0);
  if (*new_socket == -1){
    printf("Error creating socket");
    exit (EXIT_FAILURE);
  }
  printf("%s:%d\n", ip, port);
}


int create_server_socket(const char* ip_addr, const int port){
  /*create server socket*/

  struct sockaddr_in server_addr;
  int server_socket;
  create_socket(ip_addr, port, &server_addr, &server_socket);

  //Bind the address struct to the socket
  int res = bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
  if (res == -1){
    printf("Cannot bind [Errno: %d]\n", errno);
    exit(EXIT_FAILURE);
  }

  return server_socket;
}


int server_listen(int server_socket, void* (*client_f)(void*)){
  int new_socket;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  //Listen on the socket
  if(listen(server_socket, 50) == 0)
    printf("Listening on %d\n", server_socket);
  else
    printf("Error while listening attempt\n");

  pthread_t thread_id;

  while(1)
  {
      //Accept call creates a new socket for the incoming connection
      addr_size = sizeof serverStorage;
      new_socket = accept(server_socket, (struct sockaddr *) &serverStorage, &addr_size);

      if(pthread_create(&thread_id, NULL, client_f, &new_socket) != 0 )
         printf("Failed to create thread\n");

      pthread_detach(thread_id);
  }
}
