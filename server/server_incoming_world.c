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
  printf("send_all_messages to %s\n", username);
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


bool is_logged(char* username){
  //create a socket for the other server
  struct sockaddr_in address;
  int other_server_socket;

  create_socket(SERVER_OUT_ADDR, SERVER_OUT_PORT_USER, &address, &other_server_socket);

  if (connect(other_server_socket, (struct sockaddr* )&address, sizeof address) == -1){
      printf("Cannot connect to the other server\n");
      return 0;
  }
  else{
    printf("Connected to the other server\n");
  }

  User user_to_check;
  user_to_check.id = 5;
  strcpy(user_to_check.username, username);

  if(send(other_server_socket, &user_to_check, sizeof(user_to_check), 0) < 0)
    printf("Sending failed\n");

  bool logged_in;
  recv(other_server_socket, &logged_in, sizeof(logged_in), 0);
  printf("%s %d\n", username, logged_in);
  return logged_in;
}


void* give_mails(void* arg){
  /*check if user logged in send all mails*/
  printf("\n\e[0;36mⓘ Send all messages\e[m\n");

  int client_socket = *((int*)arg);

  char username[USERNAME_LEN];

  // get client data
  int n = recv(client_socket, &username, sizeof(username), 0);
  printf("got client data: %s\n", username);

  if (is_logged(username) == true){
    while (n>0){
      send_all_messages(username, client_socket);
      n = recv(client_socket, &username, sizeof(username), 0);
    }
  }

  return 0;
}
