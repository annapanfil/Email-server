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
  int id;
  char username[USERNAME_LEN];
  char password[PASSWORD_LEN];
  RcvdMail* mailbox;
} User;


/* create user with given credentials */
void new_user(char* username, char* password, User* user_place, int id); // id only for printf

/* validate password and log user (or not) - add to active users list */
int login_user(char* username, char* password, User* users, int users_num, User* active_user_place);

/*remove user from users list*/
void logout_user(char* username);

#endif //USER_H
