/*My pretty database*/
#ifndef USERTABLES_C
#define USERTABLES_C

#define MAX_USERS 100

typedef struct UserInList{
  User* user;
  User* next;
} UserInList;

User users[MAX_USERS];
int users_num = 0;
pthread_mutex_t lock_users;

User* active_users[MAX_USERS]; //hashtable would be better I think
int active_users_num = 0;
pthread_mutex_t lock_active_users;


void init_mutexes(){
  if (pthread_mutex_init(&lock_users, NULL) != 0)
    printf("Mutex (users) init failed\n");
  if (pthread_mutex_init(&lock_active_users, NULL) != 0)
    printf("Mutex (active users) init failed\n");
}

void add_user(User* user){
  pthread_mutex_lock(&lock_users);
  users[users_num] = *user;
  users_num += 1;
  pthread_mutex_unlock(&lock_users);
}


void add_active_user(User* user){
  // int id;
  // pthread_mutex_lock(&lock_active_users);
  // id = active_users_num;
  // active_users[active_users_num] = user;
  // active_users_num += 1;
  // pthread_mutex_unlock(&lock_active_users);

  // return id;

}

void rm_active_user(int id){
//   pthread_mutex_lock(&lock_active_users);
//   active_users[active_users_num] = NULL;
//   active_users_num -= 1;
//   pthread_mutex_unlock(&lock_active_users);
}

#endif //USERTABLES_C
