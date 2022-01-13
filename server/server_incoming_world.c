#include "bool.h"
#include "server_messages.h"

Mailbox mailboxes[MAX_CLIENTS];
int mailboxes_num;


Mailbox* find_mailbox(char* username, bool can_create){
  //find existing mailbox...
  for (int i=0; i<=mailboxes_num; i++){
      if (strcmp(username, mailboxes[i].username) == 0){
        return mailboxes + i;
      }
  }

  if (can_create == true){
    //...or create a new one (if available)
    Mailbox* mailbox = (Mailbox*) malloc(sizeof(Mailbox));
    strcpy(mailbox->username, username);
    mailbox->mails = NULL;
    mailboxes[mailboxes_num] = *mailbox;
    mailboxes_num += 1; //TODO: mutex?
    return mailboxes + mailboxes_num;
  }
  return NULL;
}


void send_all_messages(char*username, int client_socket){
  printf("send_all_messages\n");
  Mailbox* mailbox = find_mailbox(username, false);

  if (mailbox){
    printf("found mailbox\n");
    RcvdMail* current = mailbox->mails;
    while (current){
      //send all mails
      if(send(client_socket, &current->mail, sizeof(current->mail), 0) < 0)
        printf("Sending mail failed\n");
      else{
        printf("mail sent\n");
      }
      current = current->next;
      }
    printf("No more mails\n");
  }
  //send stop mail to mark the end of transmission
  Mail stop = {.to="STOP"};
  printf("Sending STOP\n");
  if(send(client_socket, &stop, sizeof(stop), 0) < 0){
    printf("Sending stop mail failed\n");
  }
  else{
    printf("STOP sent\n");
  };
}


void* give_mails(void* arg){
  /*get user's address and send all mails*/
  printf("\n\e[0;36mⓘ Send all messages\e[m\n");

  int other_server_socket = *((int*)arg);
  Userdata userdata;

  // get client data
  int n = recv(other_server_socket, &userdata, sizeof(userdata), 0);
  printf("%d %s\n", n, userdata.username);

  //TODO: change to udp
  //create a socket for the client
  int client_socket = socket(PF_INET, SOCK_STREAM, 0);

  if (connect(client_socket, (struct sockaddr * )&userdata.user_addr, sizeof userdata.user_addr) == -1){
      printf("Cannot connect to client\n");
      return 0;
  }
  else{
    printf("Connected to client\n");
  }

  while (n>0){
    send_all_messages(userdata.username, client_socket);
    n = recv(other_server_socket, &userdata, sizeof(userdata), 0);
  }
  printf("n= %d quitting... \n", n);
  return 0;
}
