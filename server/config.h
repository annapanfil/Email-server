#ifndef CONFIG_H
#define CONFIG_H

//https://stackoverflow.com/questions/16508685/understanding-inaddr-any-for-socket-programming/16510000

const char* SERVER_OUT_ADDR = "127.0.0.1";
const int SERVER_OUT_PORT = 1100;

const char* SERVER_IN_ADDR = "127.0.0.1";
const int SERVER_IN_PORT = 5000;

#define MAX_CLIENTS 100
#define MAX_MAILS_PER_CLIENT 1000
//TODO: delete and do linked list
#endif //CONFIG_H
