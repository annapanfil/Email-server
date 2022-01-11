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
      printf("%d %s\n", i, mailboxes[i].username);
      if (strcmp(username, mailboxes[i].username) == 0){
        printf("Found mailbox\n");
        return mailboxes + i;
      }
  }

  if (can_create == true){
    //...or create a new one
    printf("Create a new one\n");
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
  printf("\n\e[0;36mⓘ Send all messages\e[m\n");
  Mailbox* mailbox = find_mailbox(username, false);
  if (mailbox){
    printf("Checking mailbox\n");
    RcvdMail* current = mailbox->mails;
    while (current){
      printf("topic: %s\n", current->mail.topic);
      if(send(client_socket, &current->mail, sizeof(current->mail), 0) < 0)
        printf("Sending mail failed\n");
      current = current->next;
      }
  }
  Mail stop = {.to="STOP"};
  if(send(client_socket, &stop, sizeof(stop), 0) < 0)
    printf("Sending stop mail failed\n");
}


void* give_mails(void* arg){
  printf("new thread\n");
  int client_socket = *((int*)arg);
  char username[256];
  Feedback feedback;
  int n = recv(client_socket, &username, sizeof(username), 0);
  printf("%s\n", username);
  while (n>0){
    // if (is_log_in(username)) //TODO

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
