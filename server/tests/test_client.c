#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<pthread.h>

#include "../config.h"
#include "../mail.h"
#include "../server_base.c"
#include "../user.h"


int main(){

  Mail mail = {.from = "me@me.me", .to = "other2@oth.oth", .topic = "Mytopic", .text = "My text\nwith enter"};
  User user = {.id = 1, .username="anna", .password="qwerty"};
  Feedback feedback;

  // Mail mail_rcv;
  struct sockaddr_in server_addr;
  int server_out_mail_socket, server_out_user_socket,
  server_in_socket;

  // create_socket(SERVER_OUT_ADDR, SERVER_OUT_PORT_MAIL, &server_addr, &server_out_mail_socket);
  //
  // connect(server_out_mail_socket, (struct sockaddr *) &server_addr, sizeof server_addr);

  create_socket(SERVER_OUT_ADDR, SERVER_OUT_PORT_USER, &server_addr, &server_out_user_socket);

  connect(server_out_user_socket, (struct sockaddr *) &server_addr, sizeof server_addr);

  // //send mail
  // send(server_out_mail_socket, &mail, sizeof(mail), 0);
  // recv(server_out_mail_socket, &feedback, sizeof(feedback), 0);
  // printf("Feedback: %d %s\n", feedback.feedback, feedback.message);

  // create user
  // send(server_out_user_socket, &user, sizeof(user), 0);
  // recv(server_out_user_socket, &feedback, sizeof(feedback), 0);
  // printf("Feedback: %d %s\n", feedback.feedback, feedback.message);

  //login successful
  // user.id = 2;
  // strcpy(user.username, "anna");
  // strcpy(user.password, "qwerty");
  // if(send(server_out_user_socket, &user, sizeof(user), 0) > 0)
  //   printf("sent");
  // recv(server_out_user_socket, &feedback, sizeof(feedback), 0);
  // printf("Feedback: %d %s\n", feedback.feedback, feedback.message);

  // //login wrong password
  // user.id = 2;
  // strcpy(user.username, "anna");
  // strcpy(user.password, "uiop");
  // send(server_out_user_socket, &user, sizeof(user), 0);
  // recv(server_out_user_socket, &feedback, sizeof(feedback), 0);
  // printf("Feedback: %d %s\n", feedback.feedback, feedback.message);

  //login wrong user
  user.id = 2;
  strcpy(user.username, "Dan");
  send(server_out_user_socket, &user, sizeof(user), 0);
  recv(server_out_user_socket, &feedback, sizeof(feedback), 0);
  printf("Feedback: %d %s\n", feedback.feedback, feedback.message);

  close(server_out_mail_socket);
  close(server_out_user_socket);
  // close(server_in_socket);
  return 0;
}
