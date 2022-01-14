/* Get mail from client, validate and send to incoming server */

void send_to_other_server(Mail mail){
    /*send the mail to incoming server*/
    int other_server_socket;
    struct sockaddr_in server_addr;

    create_socket(SERVER_IN_ADDR, SERVER_IN_PORT_MAIL, &server_addr, &other_server_socket);
    connect(other_server_socket, (struct sockaddr *) &server_addr, sizeof server_addr);

    send(other_server_socket, &mail, sizeof(mail), 0);
}


void reset_memory(Mail mail, Feedback feedback){
  memset(&(feedback.message), 0, sizeof (feedback.message));
  memset(&(mail.from), 0, sizeof (mail.from));
  memset(&(mail.to), 0, sizeof (mail.to));
  memset(&(mail.topic), 0, sizeof (mail.topic));
  memset(&(mail.text), 0, sizeof (mail.text));
}


UserInList* check_sender(char* sender){
  /*check whether the sender is logged in*/
  sender = strsep(&sender, "@");
  return find_active_user(sender);
}


void* mail_service(void *arg)
{
  /*get mail from client and send it to the other server*/
  
  printf("\n\e[0;36mⓘ Mail service\e[m\n");
  int new_socket = *((int *)arg);
  Mail mail;
  Feedback feedback;

  //get mail from client
  int n = recv(new_socket, &mail, sizeof(mail), 0);
  while (n>0){
    //check if sender logged in and recipient exists
    if (!check_sender(mail.from)){
      feedback.feedback = 2;
      strcpy(feedback.message, "user not logged in");
    }
    else{
      if (!find_user(get_recipient(&mail))){
        feedback.feedback = 1;
        strcpy(feedback.message, "wrong address");
      }
      else{
        // Here normal server would check receiver's address domain and choose proper incoming server, but since we've got only one, we pass it there.
        send_to_other_server(mail);
        feedback.feedback = 0;
        strcpy(feedback.message, "mail sent");
    }
  }

  //send feedback to client
  printf("\e[0;35mFeedback: %d %s\e[m\n", feedback.feedback, feedback.message);

  //prepare for the next iteration
  send(new_socket, &feedback, sizeof(feedback), 0);
  reset_memory(mail, feedback);

  n = recv(new_socket, &mail, sizeof(mail), 0);
  }
  pthread_exit(NULL);
}
