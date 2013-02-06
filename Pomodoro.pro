#-------------------------------------------------
#
# Project created by QtCreator 2013-01-31T21:12:36
#
#-------------------------------------------------

QT       += core gui sql\
            webkit

TARGET = Pomorodo
TEMPLATE = app


SOURCES += main.cpp\
            pomodoro.cpp\
            Abstract\bdtools.cpp\
    Object/config.cpp \
    Component/topmenu.cpp \
    Component/yesno.cpp \
    Interface/home.cpp \
    Interface/configuration.cpp \
    Interface/about.cpp

HEADERS  += pomodoro.h\
            Abstract\bdtools.h\
    Object/config.h \
    Component/topmenu.h \
    Component/yesno.h \
    Interface/home.h \
    Interface/configuration.h \
    Interface/about.h

FORMS    += pomodoro.ui

RESOURCES += \
    Files.qrc

RC_FILE += \
        pomodoro-ico.rc
