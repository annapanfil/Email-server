#ifndef RECEIVERSMSG_H
#define RECEIVERSMSG_H
#include "inf143137message.h"
#include "inf143137receivers.h"
#include "inf143137requests.h"

typedef union RcvMsg_Data
{
    ReceiverInfo user;
    Message msg;
    MsgType msgTypeArray[BUFF_SIZE];

} RcvMsg_Data;

typedef struct RcvMsg
{
    ClientRequest request;
    RcvMsg_Data data;

} RcvMsg;

#endif // RECEIVERSMSG_H
