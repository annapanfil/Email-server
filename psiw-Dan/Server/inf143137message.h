#ifndef MESSAGE_H
#define MESSAGE_H
#include <stdlib.h>
#include "inf143137msgtype.h"
#include "inf143137buffsize.h"

typedef struct Message
{
    char text[BUFF_SIZE];
    size_t priority;
    MsgType msgType;

} Message;

extern const Message EmptyMessage;

#endif // MESSAGE_H
