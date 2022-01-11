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

#include "mail.h"
#include "mailbox.h"
#include "config.h"
#include "server_base.c"
#include "user.h"
#include "server_incoming_inner.c"
#include "server_incoming_world.c"
#include "server_incoming_mail.c"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


int main(){
  int world_socket = create_server_socket(SERVER_IN_ADDR, SERVER_IN_PORT_WORLD);
  int mail_socket = create_server_socket(SERVER_IN_ADDR, SERVER_IN_PORT_MAIL);
  int inner_socket = create_server_socket(SERVER_IN_ADDR, SERVER_IN_PORT_INNER);

  pthread_t mail_thread_id;
  if(pthread_create(&mail_thread_id, NULL, mail_server, &mail_socket) != 0 )
     printf("Failed to create thread mail\n");

  pthread_detach(mail_thread_id);

  pthread_t inner_thread_id;
  if(pthread_create(&inner_thread_id, NULL, inner_server, &inner_socket) != 0 )
     printf("Failed to create thread inner\n");

  pthread_detach(inner_thread_id);

  server_listen(world_socket, give_mails);
  return 0;
}
