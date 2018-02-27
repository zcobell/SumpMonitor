#------------------------------GPLv3------------------------------------#
#
#  This file is part of SumpMonitor.
#
#  SumpMonitor is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  SumpMonitor is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with SumpMonitor. If not, see <http://www.gnu.org/licenses/>.
#
#------------------------------GPLv3------------------------------------#

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
    src/sumpdata.cpp \
    src/network.cpp \
    src/netcdfdata.cpp \
    src/calibration.cpp

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    src/tokens.h \
    src/notifier.h \
    src/monitor.h \
    src/ultrasonicSensor.h \
    src/floatSensor.h \
    src/etapeSensor.h \
    src/postsqldata.h \
    src/sumpdata.h \
    src/pins.h \
    src/network.h \
    src/netcdfdata.h \
    src/calibration.h

LIBS += -L../thirdparty/wiringPi/wiringPi -lwiringPi -lnetcdf

INCLUDEPATH += src thirdparty/wiringPi/wiringPi

GIT_VERSION = $$system(git --git-dir $$PWD/.git --work-tree $$PWD describe --always --tags)
DEFINES += GIT_VERSION=\\\"$$GIT_VERSION\\\"
