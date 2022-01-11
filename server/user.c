#include "user.h"
#include "mail.h"
#include "usertables.c"

Feedback new_user(char* username, char* password){
  printf("\n\e[0;36mⓘ New user\e[m\n");
  User* user = (User*) malloc(sizeof(User));
  strcpy(user->username, username);
  strcpy(user->password, password);

  //TODO: check if user exists
  add_user(user);

  printf("New user %s created\n", username);

  Feedback feedback = {.feedback=0, .message="user created"};

  return feedback;
}


Feedback login_user(char* username, char* password){
  printf("\n\e[0;36mⓘ Login\e[m\n");
  Feedback feedback;

  //look for the user
  User* user = find_user(username);
  // printf("%s\n", user->username);
  if (user){
      //check password
      if(strcmp(user->password, password) == 0){
        //QUESTION: encrypted?
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
