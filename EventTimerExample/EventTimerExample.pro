#-------------------------------------------------
#
# Project created by QtCreator 2016-05-15T15:53:39
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = EventTimerExample
CONFIG   += console c++11
CONFIG   -= app_bundle

INCLUDEPATH += ../EventTimer/inc/

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../EventTimer/release/ -lEventTimer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../EventTimer/debug/ -lEventTimer
else:unix: LIBS += -L$$OUT_PWD/../EventTimer/ -lEventTimer

TEMPLATE = app


SOURCES += main.cc
