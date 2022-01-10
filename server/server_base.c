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

void create_socket(const char* ip, const int port, struct sockaddr_in* server_addr, int* server_socket){
  // int server_socket = create_socket(ip_addr, port);
  //Configure server address
  // *server_addr = {.sin_family = AF_INET, .sin_port = htons(port), .sin_addr.s_addr = inet_addr(ip)};
  server_addr->sin_family = AF_INET;
  server_addr->sin_port = htons(port);
  server_addr->sin_addr.s_addr = inet_addr(ip);
  memset(server_addr->sin_zero, '\0', sizeof server_addr->sin_zero);

  //Create the socket
  *server_socket = socket(PF_INET, SOCK_STREAM, 0);
  if (*server_socket == -1){
    printf("Error creating socket");
    exit (EXIT_FAILURE);
  }
  printf("%s:%d\n", ip, port);
}


void base(const char* ip_addr, const int port, void* (*client_f)(void*)){
  /*create server socket, accept clients and run client_f on each*/

  int new_socket;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;
  struct sockaddr_in server_addr;
  int server_socket;
  create_socket(ip_addr, port, &server_addr, &server_socket);

  //Bind the address struct to the socket
  int res = bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
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

      if( pthread_create(&thread_id, NULL, client_f, &new_socket) != 0 )
         printf("Failed to create thread\n");

      pthread_detach(thread_id);
  }
}

void login(){

}
