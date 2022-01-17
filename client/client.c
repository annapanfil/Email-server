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
        z = 0;
        while ((buff_password[z++] = getchar()) != '\n');
        strcpy(user.password, buff_password);  
        send(sockfd, &user, sizeof(user), 0);
  	recv(sockfd, &feedback, sizeof(feedback), 0);
  	printf("Feedback: %d %s\n", feedback.feedback, feedback.message);
}
void send_message_user(int sockfd,User user)
{

    Mail mail = {.from = "", .to = "", .topic = "", .text = ""};
    Feedback feedback;
    char buff_to[MAX];
    char buff_topic[MAX];
    char buff_text[MAX];
    int n;
    int z;
    int y;
    strcpy(mail.from,user.username);
        bzero(buff_to, sizeof(buff_to));
        bzero(buff_topic, sizeof(buff_topic));
        bzero(buff_text, sizeof(buff_text));
        
        printf("Enter the receiver of mail : ");
        n = 0;
        while ((buff_to[n++] = getchar()) != '\n');
        strcpy(mail.to, buff_to);
        printf("Enter the topic : ");
        z = 0;
        while ((buff_topic[z++] = getchar()) != '\n');
        strcpy(mail.topic, buff_topic);
        printf("Enter the text : ");
        y = 0;
        while ((buff_text[y++] = getchar()) != '\n');
        strcpy(mail.text, buff_text);    
        send(sockfd, &mail, sizeof(mail), 0);
        recv(sockfd, &feedback, sizeof(feedback), 0);
        printf("Feedback: %d %s\n", feedback.feedback, feedback.message);
        
}

void get_message_user(int sockfd,User user)
{
	Feedback feedback;
	char message=user.username;
	if(send(sockfd, &message, sizeof(message), 0)>0)
    	printf("sent\n");
	recv(sockfd, &feedback, sizeof(feedback), 0);
  	printf("Feedback %d %s\n", feedback.feedback, feedback.message);
  	if (feedback.feedback == 0){
    		Mail mail_rcv = {.to="a"};
    		printf("Reading mails...\n");

    		while(strcmp(mail_rcv.to, "STOP") != 0){
      			if (recv(sockfd, &mail_rcv, sizeof(mail_rcv), 0) > 0){
        			printf("%s\n", mail_rcv.topic);
      }
      else{
        printf("Received nothing\n");
      }
    }
    }
    else{
    	printf("Nothing\n");
    }
        
}


void logout_for_user(int sockfd,User user)
{
    Feedback feedback;
    user.id=3;
    send(sockfd, &user, sizeof(user), 0);
    recv(sockfd, &feedback, sizeof(feedback), 0);
    printf("Feedback: %d %s\n", feedback.feedback, feedback.message);
}


void login_for_user(int sockfd, int sckfd,int scfd)
{

    User user = {.id = 2, .username="", .password=""};
    Feedback feedback;
    char buff_username[MAX];
    char buff_password[MAX];
    char mode[MAX];
    int n;
    int z;
    int y;
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
  	printf("Thank you for login\n");
  	for(;;){
  	printf("Choose what you want to get\n Send email-mail\n Check your inbox-read\n Logout-logout\n ");
  	y = 0;
        while ((mode[y++] = getchar()) != '\n');
  	if ((strncmp(mode, "mail", 4)) == 0) {
            printf("Give data for mail \n");
            send_message_user(scfd,user);
        }
        else if ((strncmp(mode, "read", 4)) == 0) {
            printf("Getting data \n");
            get_message_user(sckfd,user);
        }
        else if ((strncmp(mode, "logout", 6)) == 0) {
            printf("Loging out \n");
            logout_for_user(sockfd,user);
            break;
        }
  	
  	}
  	}
  	else{
  	printf("Try again or choose another option");
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

  for(;;){
  	char mode[MAX];
  	int n;
  	printf("Select an option \n Register - register\n Login - login\n");
  	n = 0;
        while ((mode[n++] = getchar()) != '\n');
        if ((strncmp(mode, "register", 8)) == 0) {
            printf("Registration started \n");
            create_user(server_out_user_socket);  
        }
        if ((strncmp(mode, "login", 5)) == 0) {
            printf("Give data for login \n");
            login_for_user(server_out_user_socket,server_in_socket,server_out_mail_socket);
        }
  	
  	
  
  }
  
  close(server_out_mail_socket);
  close(server_out_user_socket);
  close(server_in_socket);
  return 0;
}
