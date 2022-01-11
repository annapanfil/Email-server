#include "bool.h"

Mailbox mailboxes[MAX_CLIENTS];
int mailboxes_num;


/*bool is_log_in(char* username){
  // ask server if logged in
  int other_server_socket;
  struct sockaddr_in server_addr;

  create_socket(SERVER_OUT_ADDR, SERVER_OUT_PORT_USER, &server_addr, &other_server_socket); //TODO: przenieść do maina?
  connect(other_server_socket, (struct sockaddr *) &server_addr, sizeof server_addr);

  User user = {.id=4, .username
    =username};
  send(other_server_socket, &mail, sizeof(mail), 0);

  //no i jak to odebrać, żeby się nie pomieszały????
}*/


Mailbox* find_mailbox(char* username, bool can_create){
  //find existing mailbox...
  for (int i=0; i<=mailboxes_num; i++){
      if (strcmp(username, mailboxes[i].username) == 0){
        return mailboxes + i;
      }
  }

  if (can_create == true){
    //...or create a new one
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
  if(send(client_socket, &stop, sizeof(stop), 0) < 0)
    printf("Sending stop mail failed\n");
}


void* give_mails(void* arg){
  /*check if user can download mails and send them*/
  printf("\n\e[0;36mⓘ Send all messages\e[m\n");
  int client_socket = *((int*)arg);
  char username[256];
  Feedback feedback;

  // get client data
  int n = recv(client_socket, &username, sizeof(username), 0);

  while (n>0){
    //TODO: check whether is logged in

    //for client to prepare for transmission
    feedback.feedback = 0;
    strcpy(feedback.message, "downloading mails");
    if(send(client_socket, &feedback, sizeof(feedback), 0) < 0)
      printf("Send feedback failed\n");
    else
      printf("\e[0;35mFeedback: %d %s\e[m\n", feedback.feedback, feedback.message);

    send_all_messages(username, client_socket);

    memset(&(feedback.message), 0, sizeof (feedback.message));

    n =recv(client_socket, &username, sizeof(username), 0);
  }
  return 0;
}
