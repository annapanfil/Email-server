#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "user.c"

int main(){
  User users[100];
  int users_num = 0;
  User active_users[100];
  int active_users_num = 0;

  new_user("Anna", "qwerty", users + users_num, users_num);
  users_num += 1; //TODO: inside function? c++? return?

  printf("created %s\n", users[users_num-1].username);

  int res = login_user("Anna", "uiop", users, users_num, active_users + active_users_num);
  if (res == 0) active_users_num += 1;  //TODO: inside function? c++? return?

  res = login_user("Anna", "qwerty", users, users_num, active_users + active_users_num);
  if (res == 0) active_users_num += 1;

  res = login_user("Dan", "qwerty", users, users_num, active_users + active_users_num);
  if (res == 0) active_users_num += 1;
}
