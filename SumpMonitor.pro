QT += core network sql
QT -= gui

CONFIG += c++11

TARGET = SumpMonitor
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += src/main.cpp \
    src/notifier.cpp \
    src/monitor.cpp \
    src/ultrasonicSensor.cpp \
    src/floatSensor.cpp \
    src/etapeSensor.cpp \
    src/postsqldata.cpp \
    src/sumpdata.cpp

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    src/tokens.h \
    src/notifier.h \
    src/monitor.h \
    src/ultrasonicSensor.h \
    src/floatSensor.h \
    src/etapeSensor.h \
    src/postsqldata.h \
    src/sumpdata.h

LIBS += -lwiringPi

INCLUDEPATH += src

GIT_VERSION = $$system(git --git-dir $$PWD/.git --work-tree $$PWD describe --always --tags)
DEFINES += GIT_VERSION=\\\"$$GIT_VERSION\\\"
