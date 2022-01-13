#ifndef SERVER_MSGS_H
#define SERVER_MSGS_H

#include "user.h"

typedef struct Userdata{
  char username[USERNAME_LEN];
  struct sockaddr user_addr;
} Userdata;

#endif //SERVER_MSGS_H
