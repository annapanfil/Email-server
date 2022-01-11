#ifndef MAILBOX_H
#define MAILBOX_H

#include "user.h"

typedef struct RcvdMail{
  Mail mail;
  struct RcvdMail* next; //beginning of the linked list
} RcvdMail;


typedef struct Mailbox{
  char username[USERNAME_LEN];
  RcvdMail* mails;
} Mailbox;

#endif //MAILBOX_H
