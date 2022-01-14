#include "user.h"
#include "mail.h"
#include "usertables.c"
#include "server_messages.h"

Feedback new_user(char* username, char* password){
  /*Create new user (if not exists)*/
  printf("\n\e[0;36mⓘ New user %s\e[m\n", username);
  User* user = (User*) malloc(sizeof(User));
  strcpy(user->username, username);
  strcpy(user->password, password);

  if(find_user(username)){
    Feedback feedback = {.feedback=1, .message="user already exists"};
    return feedback;
  }
  add_user(user);

  Feedback feedback = {.feedback=0, .message="user created"};
  return feedback;
}


Feedback login_user(char* username, char* password){
  /*Add user to active users*/
  printf("\n\e[0;36mⓘ Login %s\e[m\n", username);
  Feedback feedback;

  //look for the user
  User* user = find_user(username);
  if (user){
      //check password
      if(strcmp(user->password, password) == 0){
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
  /*Remove user from active users*/
  printf("\n\e[0;36mⓘ Logout %s\e[m\n", username);
  rm_active_user(username);
  Feedback feedback = {.feedback=0, .message="logged out"};
  return feedback;
}


void is_logged(char* username, int other_server_socket){
  bool logged;
  if (find_active_user(username))
    logged = true;
  else
    logged = false;

  if(send(other_server_socket, &logged, sizeof(logged), 0) < 0){
    printf("Send failed\n");
  }
}


// Feedback is_logged_email_pull (char* username, struct sockaddr_storage user_address){
//   /*Check if user is logged in and send their data to the other server */
//   Feedback feedback;
//
//   if (find_active_user(username)){
//     //send user address data to the other server
//     int other_server_socket;
//     struct sockaddr_in server_addr;
//
//     Userdata userdata;
//     strcpy(userdata.username, username);
//     userdata.user_addr = user_address;
//
//     create_socket(SERVER_IN_ADDR, SERVER_IN_PORT_PULL_MAIL, &server_addr, &other_server_socket);
//     connect(other_server_socket, (struct sockaddr *) &server_addr, sizeof server_addr);
//
//     send(other_server_socket, &userdata, sizeof(userdata), 0);
//
//     feedback.feedback=0;
//     strcpy(feedback.message, "downloading mails");
//   }
//   else{
//     feedback.feedback=1;
//     strcpy(feedback.message, "user not logged in");
//   }
//
//   return feedback;
// }
