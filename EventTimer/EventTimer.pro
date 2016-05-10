
QT += core sql
QT -= gui

CONFIG += c++11

TEMPLATE = lib
TARGET = eventtimer

INCLUDEPATH += \
    inc/ \
    src/

HEADERS += \
    inc/eventtimer.hh \
    inc/event.hh \
    inc/eventhandler.hh \
    inc/logger.hh \
    inc/eventtimerbuilder.hh \
    src/eventtimerlogic.hh \
    src/databasehandler.hh

SOURCES += \
    src/event.cc \
    src/eventtimerbuilder.cc \
    src/eventtimerlogic.cc \
    src/databasehandler.cc

