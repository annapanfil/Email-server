#ifndef USER_H
#define USER_H

#define USERNAME_LEN 256
#define PASSWORD_LEN 30

typedef struct User{
  int id;
  char username[USERNAME_LEN];
  char password[PASSWORD_LEN];
} User;


/* create user with given credentials */
Feedback new_user(char* username, char* password);

/* validate password and log user (or not) - add to active users list */
Feedback login_user(char* username, char* password);

/*remove user from users list*/
Feedback logout_user(char* username);

/* Check whether user is on active users list and send info to the other server */
void is_logged_in(char* username, int other_server_socket);

#endif //USER_H
