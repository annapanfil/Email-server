#include "user.h"

void new_user(char* username, char* password, User* user_place){
  printf("creating new user %s\n", username);
  User* user = (User*) malloc(sizeof(User));
  strcpy(user->username, username);
  strcpy(user->password, password);
  user->mailbox = NULL;

  *user_place = *user;
}


void login_user(char* username, char* password){

}
