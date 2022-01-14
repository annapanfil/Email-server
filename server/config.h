#ifndef CONFIG_H
#define CONFIG_H

/*
hasło nieszyfrowane
*/

const char* SERVER_OUT_ADDR = "127.0.0.1";
const int SERVER_OUT_PORT_USER = 1100;
const int SERVER_OUT_PORT_MAIL = 1101;

const char* SERVER_IN_ADDR = "127.0.0.1";
const int SERVER_IN_PORT_MAIL = 5001;
const int SERVER_IN_PORT_PULL_MAIL = 5000;

#define MAX_CLIENTS 100
#endif //CONFIG_H
