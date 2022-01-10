#include "user.h"

void new_user(char* username, char* password, User* user_place, int id){
  User* user = (User*) malloc(sizeof(User));
  // user->id = id;
  strcpy(user->username, username);
  strcpy(user->password, password);
  user->mailbox = NULL;

  *user_place = *user;

  printf("New user %s created with id %d\n", username, id);
}


int login_user(char* username, char* password, User* users, int users_num, User* active_user_place){

  for(int i=0; i<users_num; i++){
    if (strcmp(users[i].username, username) == 0){
      if(strcmp(users[i].password, password) == 0){
        //QUESTION: encrypted?
        printf("Logged %s\n", username);
        //TODO: send message to client
        *active_user_place = users[i];
        return 0;
      }
      else{
        printf("Wrong password for %s\n", username);
        //TODO: send message to client
        return 2;
      }
    }
  }
  printf("Wrong username \"%s\"\n", username);
  //TODO: send message to client
  return 1;
}
