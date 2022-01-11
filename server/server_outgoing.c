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
#include "usertables.c" //init_mutexes

// ---------------------------------------------------------------------------

void* get_interaction(void* arg){
  int new_socket = *((int*)arg);
  User user;
  Feedback feedback;
  int n = recv(new_socket, &user, sizeof(user), 0);
  while (n>0){
    switch (user.id){
      case 1: feedback = new_user(user.username, user.password); break;
      case 2: feedback = login_user(user.username, user.password); break;
      case 3: feedback = logout_user(user.username); break;
    }
    if(send(new_socket, &feedback, sizeof(feedback), 0) < 0)
      printf("Send feedback failed\n");
    else
      printf("\e[0;35mFeedback: %d %s\e[m\n", feedback.feedback, feedback.message);

    memset(&(feedback.message), 0, sizeof (feedback.message));
    memset(&(user.username), 0, sizeof (user.username));
    memset(&(user.password), 0, sizeof (user.password));

    n = recv(new_socket, &user, sizeof(user), 0);
  }
  return 0;
}


void* mail_service(void *arg)
{
  printf("\n\e[0;36mⓘ Mail service\e[m\n");
  int new_socket = *((int *)arg);
  Mail mail;
  Feedback feedback;

  int n = recv(new_socket, &mail, sizeof(mail), 0);
  while (n>0){
    char* sender = mail.from;
    sender = strsep(&sender, "@");
    if (!find_active_user(sender)){
      feedback.feedback = 2;
      strcpy(feedback.message, "user not logged in");
    }
    else{
      if (!find_user(get_recipient(&mail))){
        feedback.feedback = 1;
        strcpy(feedback.message, "wrong address");
      }
      else{
        // Here normal server would check receiver's address domain and choose proper incoming server, but since we've got only one, we pass it there.

        int other_server_socket;
        struct sockaddr_in server_addr;

        create_socket(SERVER_IN_ADDR, SERVER_IN_PORT_INNER, &server_addr, &other_server_socket); //TODO: przenieść do maina?
        connect(other_server_socket, (struct sockaddr *) &server_addr, sizeof server_addr);

        feedback.feedback = 0;
        strcpy(feedback.message, "mail sent");
        send(other_server_socket, &mail, sizeof(mail), 0);
    }
  }

  printf("\e[0;35mFeedback: %d %s\e[m\n", feedback.feedback, feedback.message);
  send(new_socket, &feedback, sizeof(feedback), 0);
  memset(&(feedback.message), 0, sizeof (feedback.message));
  memset(&(mail.from), 0, sizeof (mail.from));
  memset(&(mail.to), 0, sizeof (mail.to));
  memset(&(mail.topic), 0, sizeof (mail.topic));
  memset(&(mail.text), 0, sizeof (mail.text));

  n = recv(new_socket, &mail, sizeof(mail), 0);


  }
  pthread_exit(NULL);
}


void* server_users(void* arg){

  int server_socket = *((int*)arg);

  server_listen(server_socket, get_interaction);
  return 0;
}

int main(){
  int user_interaction_socket = create_server_socket(SERVER_OUT_ADDR, SERVER_OUT_PORT_USER);
  int mail_socket = create_server_socket(SERVER_OUT_ADDR, SERVER_OUT_PORT_MAIL);
  init_mutexes();

  /* create thread to controll user interaction and to receive mails*/
  pthread_t thread_id;
  if(pthread_create(&thread_id, NULL, server_users, &user_interaction_socket) != 0 )
     printf("Failed to create thread server_users\n");

  pthread_detach(thread_id);

  server_listen(mail_socket, mail_service);

  //TODO: exit nicely (close sockets, remove users from memory, destroy mutexes)
  return 0;
}
