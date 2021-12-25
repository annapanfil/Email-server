TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += link_pkgconfig
PKGCONFIG += gtk+-3.0


SOURCES += \
    inf143137connect.c \
    inf143137main.c \
    inf143137message.c \
    inf143137msgtype.c \
    inf143137receivers.c \
    inf143137senders.c \
    inf143137loginwindow.c \
    inf143137registerwindow.c \
    inf143137msgbox.c \
    inf143137removewidgets.c \
    inf143137listening.c

HEADERS += \
    inf143137bool.h \
    inf143137buffsize.h \
    inf143137client.h \
    inf143137connect.h \
    inf143137error.h \
    inf143137loginwindow.h \
    inf143137message.h \
    inf143137msgbuf.h \
    inf143137msgtype.h \
    inf143137receivers.h \
    inf143137receiversmsg.h \
    inf143137registerwindow.h \
    inf143137requests.h \
    inf143137senders.h \
    inf143137sendersmsg.h \
    inf143137msgbox.h \
    inf143137removewidgets.h \
    inf143137listening.h \
    inf143137port.h
