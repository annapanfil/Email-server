Mailbox* add_to_mailbox(Mail mail, char* username){
  /*Add to user mailbox*/
  printf("\n\e[0;36mⓘ Add to %s's mailbox %s\e[m\n", username, mail.topic);

  Mailbox* mailbox = find_mailbox(username, true);

  //copy the mail
  RcvdMail* new_mail = (RcvdMail*) malloc(sizeof(RcvdMail));
  new_mail->mail = *(Mail*) malloc(sizeof(mail));
  new_mail->mail = mail;
  new_mail->next = mailbox->mails;

  mailbox->mails = new_mail;

  return mailbox;
}


void* get_mail(void *arg)
{
  /*get mail from outgoing server*/
  int new_socket = *((int *)arg);
  Mail mail;
  recv(new_socket, &mail, sizeof(mail), 0);

  char* recipient = get_recipient(&mail);

  add_to_mailbox(mail, recipient);

  return 0;
}


void* mail_server(void* arg){
  /*wait for mails from outgoing server*/
  int socket = *((int *)arg);
  server_listen(socket, get_mail);
  return 0;
}
