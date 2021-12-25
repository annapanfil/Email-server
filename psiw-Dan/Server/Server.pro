TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        inf143137main.c \
    inf143137receivers.c \
    inf143137senders.c \
    inf143137message.c \
    inf143137server.c \
    inf143137connect.c \
    inf143137msgtype.c \
    inf143137requests.c

HEADERS += \
    inf143137connect.h \
    inf143137message.h \
    inf143137receivers.h \
    inf143137senders.h \
    inf143137requests.h \
    inf143137server.h \
    inf143137bool.h \
    inf143137sendersmsg.h \
    inf143137receiversmsg.h \
    inf143137msgtype.h \
    inf143137buffsize.h \
    inf143137error.h \
    inf143137msgbuf.h \
    inf143137port.h
