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
  /*send all mails from mailbox to the user*/
  Mailbox* mailbox = find_mailbox(username, false);

  if (mailbox){
    RcvdMail* current = mailbox->mails;
    while (current){
      //send all mails
      if(send(client_socket, &current->mail, sizeof(current->mail), 0) < 0)
        printf("Sending mail failed\n");
      current = current->next;
      }
  }
  //send stop mail to mark the end of transmission
  Mail stop = {.to="STOP"};
  if(send(client_socket, &stop, sizeof(stop), 0) < 0){
    printf("Sending stop mail failed\n");
  }
}


bool is_logged(char* username){
  /*ask the other server whether the user is logged in*/
  //create a socket for the other server
  struct sockaddr_in address;
  int other_server_socket;

  create_socket(SERVER_OUT_ADDR, SERVER_OUT_PORT_USER, &address, &other_server_socket);

  if (connect(other_server_socket, (struct sockaddr* )&address, sizeof address) == -1){
      printf("Cannot connect to the other server\n");
      return 0;
  }

  User user_to_check;
  user_to_check.id = 5;
  strcpy(user_to_check.username, username);

  if(send(other_server_socket, &user_to_check, sizeof(user_to_check), 0) < 0)
    printf("Sending failed\n");

  bool logged_in;
  recv(other_server_socket, &logged_in, sizeof(logged_in), 0);
  return logged_in;
}


void* give_mails(void* arg){
  /*check if user logged in send all mails*/
  printf("\n\e[0;36mⓘ Send all messages\e[m\n");

  int client_socket = *((int*)arg);

  char username[USERNAME_LEN];

  // get client data
  int n = recv(client_socket, &username, sizeof(username), 0);

  if (is_logged(username) == true){
    // send feedback for client to prepare for transmission
    Feedback feedback = {.feedback = 0, .message="downloading mails"};
    send(client_socket, &feedback, sizeof(feedback), 0);

    while (n>0){
      send_all_messages(username, client_socket);
      n = recv(client_socket, &username, sizeof(username), 0);
    }
  }
  else{
    Feedback feedback = {.feedback = 1, .message="user not logged in"};
    send(client_socket, &feedback, sizeof(feedback), 0);
  }

  return 0;
}
