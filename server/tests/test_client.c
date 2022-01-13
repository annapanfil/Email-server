#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <pthread.h>

#include "../config.h"
#include "../mail.h"
#include "../server_base.c"
#include "../user.h"


int main(){

  Mail mail = {.from = "anna@me.me", .to = "dan@oth.oth", .topic = "Mytopic", .text = "My text\nwith enter"};
  User user = {.id = 1, .username="anna", .password="qwerty"};
  Feedback feedback;

  struct sockaddr_in server_addr;
  int server_out_mail_socket, server_out_user_socket,
  server_in_socket;

  create_socket(SERVER_OUT_ADDR, SERVER_OUT_PORT_MAIL, &server_addr, &server_out_mail_socket);

  connect(server_out_mail_socket, (struct sockaddr *) &server_addr, sizeof server_addr);

  create_socket(SERVER_OUT_ADDR, SERVER_OUT_PORT_USER, &server_addr, &server_out_user_socket);

  connect(server_out_user_socket, (struct sockaddr *) &server_addr, sizeof server_addr);

  create_socket(SERVER_IN_ADDR, SERVER_IN_PORT_PULL_MAIL, &server_addr, &server_in_socket);

  connect(server_in_socket, (struct sockaddr *) &server_addr, sizeof server_addr);
  // //send mail - not logged in
  // send(server_out_mail_socket, &mail, sizeof(mail), 0);
  // recv(server_out_mail_socket, &feedback, sizeof(feedback), 0);
  // printf("Feedback: %d %s\n", feedback.feedback, feedback.message);
  //
  // create user
  send(server_out_user_socket, &user, sizeof(user), 0);
  recv(server_out_user_socket, &feedback, sizeof(feedback), 0);
  printf("Feedback: %d %s\n", feedback.feedback, feedback.message);

  // login successful
  user.id = 2;
  strcpy(user.username, "anna");
  strcpy(user.password, "qwerty");
  send(server_out_user_socket, &user, sizeof(user), 0);
  recv(server_out_user_socket, &feedback, sizeof(feedback), 0);
  printf("Feedback: %d %s\n", feedback.feedback, feedback.message);

  // //login wrong password
  // user.id = 2;
  // strcpy(user.username, "anna");
  // strcpy(user.password, "uiop");
  // send(server_out_user_socket, &user, sizeof(user), 0);
  // recv(server_out_user_socket, &feedback, sizeof(feedback), 0);
  // printf("Feedback: %d %s\n", feedback.feedback, feedback.message);
  //
  // // login wrong user
  // user.id = 2;
  // strcpy(user.username, "dan");
  // send(server_out_user_socket, &user, sizeof(user), 0);
  // recv(server_out_user_socket, &feedback, sizeof(feedback), 0);
  // printf("Feedback: %d %s\n", feedback.feedback, feedback.message);
  //
  // //send mail - wrong address
  // send(server_out_mail_socket, &mail, sizeof(mail), 0);
  // recv(server_out_mail_socket, &feedback, sizeof(feedback), 0);
  // printf("Feedback: %d %s\n", feedback.feedback, feedback.message);

  // create user
  user.id = 1;
  strcpy(user.username, "dan");
  send(server_out_user_socket, &user, sizeof(user), 0);
  recv(server_out_user_socket, &feedback, sizeof(feedback), 0);
  printf("Feedback: %d %s\n", feedback.feedback, feedback.message);

  // login successful
  user.id = 2;
  strcpy(user.username, "dan");
  send(server_out_user_socket, &user, sizeof(user), 0);
  recv(server_out_user_socket, &feedback, sizeof(feedback), 0);
  printf("Feedback: %d %s\n", feedback.feedback, feedback.message);


  //send mail - successful
  send(server_out_mail_socket, &mail, sizeof(mail), 0);
  recv(server_out_mail_socket, &feedback, sizeof(feedback), 0);
  printf("Feedback: %d %s\n", feedback.feedback, feedback.message);


  //pull mails
  printf("\n\e[0;36mⓘ Pull mails\e[m\n");
  user.id = 4;
  strcpy(user.username, "dan");

  if(send(server_out_user_socket, &user, sizeof(user), 0)>0)
    printf("sent\n");

  recv(server_out_user_socket, &feedback, sizeof(feedback), 0);
  printf("Feedback %d %s\n", feedback.feedback, feedback.message);
  if (feedback.feedback == 0){
    Mail mail_rcv = {.to="a"};
    printf("Reading mails...\n");

    //TODO: UDP
    while(strcmp(mail_rcv.to, "STOP") != 0){
      if (recv(socket, &mail_rcv, sizeof(mail_rcv), 0) > 0){
        printf("%s\n", mail_rcv.topic);
      }
      else{
        printf("Received nothing\n");
        break;
      }
    }
  }

  //logout
  user.id = 3;
  strcpy(user.username, "anna");
  send(server_out_user_socket, &user, sizeof(user), 0);
  recv(server_out_user_socket, &feedback, sizeof(feedback), 0);
  printf("Feedback: %d %s\n", feedback.feedback, feedback.message);

  //logout
  user.id = 3;
  strcpy(user.username, "dan");
  send(server_out_user_socket, &user, sizeof(user), 0);
  recv(server_out_user_socket, &feedback, sizeof(feedback), 0);
  printf("Feedback: %d %s\n", feedback.feedback, feedback.message);


  close(server_out_mail_socket);
  close(server_out_user_socket);
  close(server_in_socket);
  return 0;
}
