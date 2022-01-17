#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <pthread.h>
#define MAX 80

#include "../server/config.h"
#include "../server/mail.h"
#include "../server/server_base.c"
#include "../server/user.h"

//    Mail mail = {.from = "", .to = "", .topic = "", .text = ""};

void create_user(int sockfd)
{

    User user = {.id = 1, .username="", .password=""};
    Feedback feedback;
    char buff_username[MAX];
    char buff_password[MAX];
    int n;
    int z;
        bzero(buff_username, sizeof(buff_username));
        bzero(buff_password, sizeof(buff_password));
        printf("Enter the username : ");
        n = 0;
        while ((buff_username[n++] = getchar()) != '\n');
        strcpy(user.username, buff_username);
        printf("Enter the password : ");
        n = 0;
        while ((buff_password[n++] = getchar()) != '\n');
        strcpy(user.password, buff_password);  
        send(sockfd, &user, sizeof(user), 0);
  	recv(sockfd, &feedback, sizeof(feedback), 0);
  	printf("Feedback: %d %s\n", feedback.feedback, feedback.message);
}

void login_for_user(int sockfd)
{

    User user = {.id = 2, .username="", .password=""};
    Feedback feedback;
    char buff_username[MAX];
    char buff_password[MAX];
    int n;
    int z;
        bzero(buff_username, sizeof(buff_username));
        bzero(buff_password, sizeof(buff_password));
        printf("Enter the username : ");
        n = 0;
        while ((buff_username[n++] = getchar()) != '\n');
        strcpy(user.username, buff_username);
        printf("Enter the password : ");
        z = 0;
        while ((buff_password[z++] = getchar()) != '\n');
        strcpy(user.password, buff_password);  
        send(sockfd, &user, sizeof(user), 0);
  	recv(sockfd, &feedback, sizeof(feedback), 0);
  	printf("Feedback: %d %s\n", feedback.feedback, feedback.message);
  	if (feedback.feedback==0){
  	return buff_username;
  	}
  	else{
  	return 0;
  	}
}

void send_message_user(int sockfd,char username)
{

    Mail mail = {.from = "", .to = "", .topic = "", .text = ""};
    Feedback feedback;
    char buff_to[MAX];
    char buff_topic[MAX];
    char buff_text[MAX];
    int n;
    int z;
    int y;
    strcpy(mail.from,username);
        bzero(buff_username, sizeof(buff_username));
        bzero(buff_password, sizeof(buff_password));
        printf("Enter the username : ");
        n = 0;
        while ((buff_username[n++] = getchar()) != '\n');
        strcpy(user.username, buff_username);
        printf("Enter the password : ");
        z = 0;
        while ((buff_password[z++] = getchar()) != '\n');
        strcpy(user.password, buff_password);  
        send(sockfd, &user, sizeof(user), 0);
  	recv(sockfd, &feedback, sizeof(feedback), 0);
  	printf("Feedback: %d %s\n", feedback.feedback, feedback.message);
  	if (feedback.feedback==0){
  	return buff_username;
  	}
  	else{
  	return 0;
  	}
}



int main(){



  struct sockaddr_in server_addr;
  int server_out_mail_socket, server_out_user_socket, server_in_socket;

  create_socket(SERVER_OUT_ADDR, SERVER_OUT_PORT_MAIL, &server_addr, &server_out_mail_socket);

  connect(server_out_mail_socket, (struct sockaddr *) &server_addr, sizeof server_addr);

  create_socket(SERVER_OUT_ADDR, SERVER_OUT_PORT_USER, &server_addr, &server_out_user_socket);

  connect(server_out_user_socket, (struct sockaddr *) &server_addr, sizeof server_addr);

  create_socket(SERVER_IN_ADDR, SERVER_IN_PORT_PULL_MAIL, &server_addr, &server_in_socket);

  connect(server_in_socket, (struct sockaddr *) &server_addr, sizeof server_addr);

  // create user
//  send(server_out_user_socket, &user, sizeof(user), 0);
  //recv(server_out_user_socket, &feedback, sizeof(feedback), 0);
  //printf("Feedback: %d %s\n", feedback.feedback, feedback.message);

  // create user
  //user.id = 1;
  //strcpy(user.username, "dan");
  //send(server_out_user_socket, &user, sizeof(user), 0);
  //recv(server_out_user_socket, &feedback, sizeof(feedback), 0);
  //printf("Feedback: %d %s\n", feedback.feedback, feedback.message);


  // login successful
  //user.id = 2;
  //strcpy(user.username, "anna");
  //strcpy(user.password, "qwerty");
  //send(server_out_user_socket, &user, sizeof(user), 0);
  //recv(server_out_user_socket, &feedback, sizeof(feedback), 0);
  //printf("Feedback: %d %s\n", feedback.feedback, feedback.message);

  // login successful
  //user.id = 2;
  //strcpy(user.username, "dan");
  //send(server_out_user_socket, &user, sizeof(user), 0);
  //recv(server_out_user_socket, &feedback, sizeof(feedback), 0);
  //printf("Feedback: %d %s\n", feedback.feedback, feedback.message);

  //send mail - successful
  //send(server_out_mail_socket, &mail, sizeof(mail), 0);
  //recv(server_out_mail_socket, &feedback, sizeof(feedback), 0);
  //printf("Feedback: %d %s\n", feedback.feedback, feedback.message);


  //char message[] = "dan";

  //if(send(server_in_socket, &message, sizeof(message), 0)>0)
    //printf("sent\n");

  //recv(server_in_socket, &feedback, sizeof(feedback), 0);
  //printf("Feedback %d %s\n", feedback.feedback, feedback.message);

  //if (feedback.feedback == 0){
    //Mail mail_rcv = {.to="a"};
    //printf("Reading mails...\n");

    //while(strcmp(mail_rcv.to, "STOP") != 0){
      //if (recv(server_in_socket, &mail_rcv, sizeof(mail_rcv), 0) > 0){
        //printf("%s\n", mail_rcv.topic);
     // }
      //else{
        //printf("Received nothing\n");
        //break;
      //}
    //}
  //}


  //logout
  //user.id = 3;
  //strcpy(user.username, "anna");
  //send(server_out_user_socket, &user, sizeof(user), 0);
  //recv(server_out_user_socket, &feedback, sizeof(feedback), 0);
  //printf("Feedback: %d %s\n", feedback.feedback, feedback.message);

  //logout
  //user.id = 3;
  //strcpy(user.username, "dan");
  //send(server_out_user_socket, &user, sizeof(user), 0);
  //recv(server_out_user_socket, &feedback, sizeof(feedback), 0);
 // printf("Feedback: %d %s\n", feedback.feedback, feedback.message);
  char username[MAX];
  for(;;){
  	char mode[MAX];
  	int n;
  	printf("Select an option \n");
  	n = 0;
        while ((mode[n++] = getchar()) != '\n');
        if ((strncmp(mode, "register", 8)) == 0) {
            printf("Registration started \n");
            create_user(server_out_user_socket);  
        }
        if ((strncmp(mode, "login", 5)) == 0) {
            printf("Give data for login \n");
            login_for_user(server_out_user_socket);
            username=login_for_user(server_out_user_socket);
            printf(username);  
        }
  	
  	
  
  }
  
  close(server_out_mail_socket);
  close(server_out_user_socket);
  close(server_in_socket);
  return 0;
}
