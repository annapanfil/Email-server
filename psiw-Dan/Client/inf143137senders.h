#ifndef SENDERS_H
#define SENDERS_H
#include "inf143137buffsize.h"

typedef struct SenderInfo
{
        int ID;
        char name[BUFF_SIZE];

} SenderInfo;

extern const SenderInfo EmptySendersInfo;

#endif // SENDERS_H
