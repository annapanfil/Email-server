#ifndef USER_H
#define USER_H

#include "mail.h"
#define USERNAME_LEN 256
#define PASSWORD_LEN 30


typedef struct RcvdMail{
  Mail mail;
  struct RcvdMail* next; //beginning of the linked list
} RcvdMail;


typedef struct User{
  int id; //TODO: enum for types
  char username[USERNAME_LEN];
  char password[PASSWORD_LEN];
} User;


typedef struct Mailbox{
  char username[USERNAME_LEN];
  RcvdMail* mails;
} Mailbox;


/* create user with given credentials */
Feedback new_user(char* username, char* password);

/* validate password and log user (or not) - add to active users list */
Feedback login_user(char* username, char* password);

/*remove user from users list*/
Feedback logout_user(char* username);

#endif //USER_H
