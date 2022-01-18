/*Accepts mail pulling, stores mail data*/

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
#include <signal.h>

#include "mail.h"
#include "mailbox.h"
#include "config.h"
#include "server_base.c"
#include "user.h"
#include "bool.h"
#include "server_incoming_world.c"
#include "server_incoming_mail.c"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
bool running = true;

void exit_handler(int sig){
  printf("\nClosing server...\n");
  running = false;
}

int main(){
  signal(SIGINT, exit_handler);

  int user_request_socket = create_server_socket(SERVER_IN_ADDR, SERVER_IN_PORT_PULL_MAIL);
  int incoming_mail_socket = create_server_socket(SERVER_IN_ADDR, SERVER_IN_PORT_MAIL);

  pthread_t mail_thread_id;
  if(pthread_create(&mail_thread_id, NULL, mail_server, &incoming_mail_socket) != 0 )
     printf("Failed to create mail thread\n");
   pthread_detach(mail_thread_id);


   pthread_t user_thread_id;
   if(pthread_create(&user_thread_id, NULL, user_server, &user_request_socket) != 0 )
      printf("Failed to create user thread\n");
    pthread_detach(user_thread_id);

  while(running == true){
    ;
  }

  pthread_kill(user_thread_id, 17); //SIGCHLD
  pthread_kill(mail_thread_id, 18); //SIGCHLD
  printf("Signal sent\n");

  pthread_join(user_thread_id, NULL);
  pthread_join(mail_thread_id, NULL);
  printf("Threads joined\n");

  close(user_request_socket);
  close(incoming_mail_socket);
  printf("Sockets closed\n");

  printf("\n\e[0;36mⓘ Goodbye!\e[m\n");
  return 0;
}
