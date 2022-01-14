void* get_interaction(void* arg){
  /*user creation, login and logout*/

  int new_socket = *((int*)arg);
  User user;
  Feedback feedback;

  int n = recv(new_socket, &user, sizeof(user), 0);

  while (n>0){
    switch (user.id){
      case 1: feedback = new_user(user.username, user.password); break;
      case 2: feedback = login_user(user.username, user.password); break;
      case 3: feedback = logout_user(user.username); break;
      case 5: is_logged_in(user.username, new_socket); // from the other server
    }

    if (user.id < 5){
      //send feedback
      if(send(new_socket, &feedback, sizeof(feedback), 0) < 0)
      printf("Send feedback failed\n");
      else
      printf("\e[0;35mFeedback: %d %s\e[m\n", feedback.feedback, feedback.message);
    }

    memset(&(feedback.message), 0, sizeof (feedback.message));
    memset(&(user.username), 0, sizeof (user.username));
    memset(&(user.password), 0, sizeof (user.password));

    n = recv(new_socket, &user, sizeof(user), 0);
  }
  return 0;
}


void* server_users(void* arg){
  //wait for requests
  int server_socket = *((int*)arg);
  server_listen(server_socket, get_interaction);
  return 0;
}
