/*Send mail and receive it back*/

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

int main(){

  Mail mail = {.from = "me@me.me", .to = "other@oth.oth", .topic = "Mytopic", .text = "My text\nwith enter"};
  Mail mail_rcv;

  struct sockaddr_in serverAddr = {.sin_family = AF_INET, .sin_port = htons(SERVER_OUT_PORT_MAIL), .sin_addr.s_addr = inet_addr(SERVER_OUT_ADDR)}; //Settings of the server address
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

  int clientSocket = socket(PF_INET, SOCK_STREAM, 0);

  connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof serverAddr);

  if( send(clientSocket, &mail, sizeof(mail), 0) < 0)
  {
    printf("Send failed\n");
  }

  if(recv(clientSocket, &mail_rcv, sizeof(mail_rcv), 0) < 0)
  {
    printf("Receive failed\n");
  }

  printf("Topic: %s\n%s\n", mail_rcv.topic, mail_rcv.text);

  close(clientSocket);

  return 0;
}
