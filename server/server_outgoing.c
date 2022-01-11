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

#define MAX_USERS 100

User users[MAX_USERS];
int users_num = 0;
User active_users[MAX_USERS];
int active_users_num = 0;
// TODO: mutexy dla nich

// ---------------------------------------------------------------------------

void* get_interaction(void* arg){

  int new_socket = *((int*)arg);
  User user;
  recv(new_socket, &user, sizeof(user), 0);
  printf("Type: %d\n", user.id);

  // switch (user.id){
  //   case 1: new_user(user.username, user.password, users, users_num); break;
  //   case 2: login_user(user.username, user.password, users, users_num, active_users, active_users_num); break;
  //   case 3: logout_user(user.username, active_users, active_users_num); break;
  // }
  return 0;
}


void* mail_service(void *arg)
{
  int new_socket = *((int *)arg);
  Mail mail;
  recv(new_socket, &mail, sizeof(mail), 0);
  printf("Got message \"%s\". Passing to incoming server.\n", mail.topic);

  // Here normal server would check receiver's address domain and choose proper incoming server, but since we've got only one, we pass it there.

  int other_server_socket;
  struct sockaddr_in server_addr;

  create_socket(SERVER_IN_ADDR, SERVER_IN_PORT_INNER, &server_addr, &other_server_socket); //TODO: przenieść do maina?
  connect(other_server_socket, (struct sockaddr *) &server_addr, sizeof server_addr);

  send(other_server_socket, &mail, sizeof(mail), 0);
  // send(new_socket, &mail, sizeof(mail), 0); //return to client TODO: delete

  pthread_exit(NULL);
}

//
// void server_listen(int server_socket){
//   int new_socket;
//   struct sockaddr_storage serverStorage;
//   socklen_t addr_size;
//
//   //Listen on the socket
//   if(listen(server_socket, 50) == 0)
//     printf("Listening mail thread\n");
//   else
//     printf("Error in mail thread\n");
//
//   pthread_t thread_id;
//
//   while(1)
//   {
//       //Accept call creates a new socket for the incoming connection
//       addr_size = sizeof serverStorage;
//       new_socket = accept(server_socket, (struct sockaddr *) &serverStorage, &addr_size);
//
//       if(pthread_create(&thread_id, NULL, mail_service, &new_socket) != 0 )
//          printf("Failed to create mail thread\n");
//
//       pthread_detach(thread_id);
//   }
// }


void* server_users(void* arg){

  int server_socket = *((int*)arg);

  server_listen(server_socket, get_interaction);
  // int new_socket;
  // struct sockaddr_storage serverStorage;
  // socklen_t addr_size;
  //
  // //Listen on the socket
  // if(listen(server_socket, 50) == 0)
  //   printf("Listening on users thread\n");
  // else
  //   printf("Error in users thread\n");
  //
  // pthread_t thread_id;
  //
  //
  // while(1)
  // {
  //     //Accept call creates a new socket for the incoming connection
  //     addr_size = sizeof serverStorage;
  //     new_socket = accept(server_socket, (struct sockaddr *) &serverStorage, &addr_size);
  //
  //     if(pthread_create(&thread_id, NULL, get_interaction, &new_socket) != 0 )
  //        printf("Failed to create thread\n");
  //
  //     pthread_detach(thread_id);
  // }
  return 0;
}

int main(){
  int user_interaction_socket = create_server_socket(SERVER_OUT_ADDR, SERVER_OUT_PORT_USER);
  int mail_socket = create_server_socket(SERVER_OUT_ADDR, SERVER_OUT_PORT_MAIL);

  /* create thread to controll user interaction and to receive mails*/
  pthread_t thread_id;
  if(pthread_create(&thread_id, NULL, server_users, &user_interaction_socket) != 0 )
     printf("Failed to create thread server_users\n");

  pthread_detach(thread_id);

  server_listen(mail_socket, mail_service);
  return 0;
}
