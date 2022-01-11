/*Users data structures - quasi database*/
#ifndef USERTABLES_C
#define USERTABLES_C
#include "user.h"

#define MAX_USERS 100

typedef struct UserInList{
  char username[USERNAME_LEN];
  struct UserInList* next;
} UserInList;

User users[MAX_USERS];  //all registered users
int users_num = 0;
pthread_mutex_t lock_users;

UserInList* active_users = NULL; //logged in users
pthread_mutex_t lock_active_users;


void init_mutexes(){
  if (pthread_mutex_init(&lock_users, NULL) != 0)
    printf("Mutex (users) init failed\n");
  if (pthread_mutex_init(&lock_active_users, NULL) != 0)
    printf("Mutex (active users) init failed\n");
}

//------------------------------------------------
void add_user(User* user){
  pthread_mutex_lock(&lock_users);
  users[users_num] = *user;
  users_num += 1;
  pthread_mutex_unlock(&lock_users);
}


User* find_user(char* username){
  for(int i=0; i<=users_num; i++){
    if (strcmp(users[i].username, username) == 0){
      return users + i; //found
    }
  }
  return NULL; //not found
}

//-----------------------------------------------------------------------
void add_active_user(char* username){
  UserInList* new_active_user = (UserInList*) malloc(sizeof(UserInList));
  strcpy(new_active_user->username, username);

  pthread_mutex_lock(&lock_active_users);
  new_active_user->next = active_users;
  active_users = new_active_user;
  pthread_mutex_unlock(&lock_active_users);
}


UserInList* find_active_user(char* username){
  UserInList* current = active_users;
  while (current){
    if (strcmp(current->username, username) == 0){
      return current;
    }
    current = current->next;
  }
  return NULL;
}


void rm_active_user(char* username){
  UserInList* previous = active_users;
  UserInList* current = active_users;
  pthread_mutex_lock(&lock_active_users);
  while (current){
    if (strcmp(current->username, username) == 0){
      previous->next = current->next;
      pthread_mutex_unlock(&lock_active_users);
      free(current);
      return;
    }
    previous = current;
    current = current->next;
  }
  pthread_mutex_unlock(&lock_active_users);
}

#endif //USERTABLES_C
