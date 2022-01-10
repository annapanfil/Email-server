/*Accepts connections from multiple clients, adds sender to the address book, read receiver's address from it and passes it to the other server. */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <pthread.h>
#include <errno.h>

#include "mail.h"
#include "config.h"
#include "server_base.c"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* function(void *arg)
{

}


int main(){
  base(SERVER_IN_ADDR, SERVER_IN_PORT, function);
  return 0;
}
