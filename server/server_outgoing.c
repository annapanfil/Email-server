/*Obsługa maili wychodzących i bazy danych użytkowników*/

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
#include "config.h"
#include "server_base.c"
#include "user.c"
#include "usertables.c" //init_mutexes
#include "server_outgoing_user.c"
#include "server_outgoing_mail.c"

// ---------------------------------------------------------------------------

bool running = true;

void exit_handler(int sig){
  running = false;
}

int main(){
  signal(SIGINT, exit_handler);

  int user_interaction_socket = create_server_socket(SERVER_OUT_ADDR, SERVER_OUT_PORT_USER);
  int mail_socket = create_server_socket(SERVER_OUT_ADDR, SERVER_OUT_PORT_MAIL);
  init_mutexes();

  /* create thread to control user interaction*/
  pthread_t thread_id;
  if(pthread_create(&thread_id, NULL, server_users, &user_interaction_socket) != 0 )
     printf("Failed to create thread server_users\n");

  pthread_detach(thread_id);

  /* create thread to receive mails*/
  pthread_t mail_thread_id;
  if(pthread_create(&mail_thread_id, NULL, server_mail, &mail_socket) != 0 )
     printf("Failed to create thread server_mail\n");

  pthread_detach(mail_thread_id);

  while (running == true){
    ;
  }

  close(user_interaction_socket);
  close(mail_socket);
  printf("\n\e[0;36mⓘ Goodbye!\e[m\n");
  return 0;
}
