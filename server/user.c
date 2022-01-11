#include "user.h"
#include "mail.h"
#include "usertables.c"

Feedback new_user(char* username, char* password){
  printf("\n\e[0;36mⓘ New user %s\e[m\n", username);
  User* user = (User*) malloc(sizeof(User));
  strcpy(user->username, username);
  strcpy(user->password, password);

  if(find_user(username)){
    Feedback feedback = {.feedback=1, .message="user already exists"};
    return feedback;
  }
  add_user(user);

  printf("New user %s created\n", username);

  Feedback feedback = {.feedback=0, .message="user created"};

  return feedback;
}


Feedback login_user(char* username, char* password){
  printf("\n\e[0;36mⓘ Login %s\e[m\n", username);
  Feedback feedback;

  //look for the user
  User* user = find_user(username);
  if (user){
      //check password
      if(strcmp(user->password, password) == 0){
        //QUESTION: encrypted?
        add_active_user(username);
        feedback.feedback = 0;
        strcpy(feedback.message, "logged in");
        return feedback;
      }
      else{
        feedback.feedback = 2;
        strcpy(feedback.message, "wrong password");
        return feedback;
      }
  }
  feedback.feedback = 1;
  strcpy(feedback.message, "wrong username");

  return feedback;
}


Feedback logout_user(char* username){
  printf("\n\e[0;36mⓘ Logout %s\e[m\n", username);
  rm_active_user(username);
  Feedback feedback = {.feedback=0, .message="logged out"};
  return feedback;
}
