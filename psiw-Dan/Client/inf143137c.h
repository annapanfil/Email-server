#ifndef CLIENT_H
#define CLIENT_H
#include "inf143137bool.h"
#include "inf143137message.h"
#include "inf143137error.h"

typedef struct ClientInfo
{
    int clientFileDesc;
    int serverFileDesc;
    Error *lastError;

} ClientInfo;

#endif // CLIENT_H
