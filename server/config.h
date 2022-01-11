#ifndef CONFIG_H
#define CONFIG_H
/*TODO:
- server incoming - dodać socket world i feedback
- pull mails

sprawdzenie poprawności hasła po stronie klienta
hasło nieszyfrowane
*/


//https://stackoverflow.com/questions/16508685/understanding-inaddr-any-for-socket-programming/16510000

const char* SERVER_OUT_ADDR = "127.0.0.1";
const int SERVER_OUT_PORT_USER = 1100;
const int SERVER_OUT_PORT_MAIL = 1101;

const char* SERVER_IN_ADDR = "127.0.0.1";
const int SERVER_IN_PORT_WORLD = 5000;
const int SERVER_IN_PORT_MAIL = 5001;
const int SERVER_IN_PORT_INNER = 5002;


#define MAX_CLIENTS 100
#endif //CONFIG_H
