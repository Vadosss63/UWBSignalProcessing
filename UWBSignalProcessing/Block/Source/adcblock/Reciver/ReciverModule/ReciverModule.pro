TEMPLATE = app
CONFIG += console

CONFIG -= app_bundle
CONFIG -= qt

lessThan(QT_MAJOR_VERSION, 5): QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += $$PWD/../../UtilityUWB
INCLUDEPATH += $$PWD/../Commands

DESTDIR = ../../../UWB

SOURCES += main.cpp \
    recievercontrolmodule.cpp \
    rs485handler.cpp \
    commandcontrol.cpp \
    controllercommands.cpp \
    interfaces.cpp \
    observer.cpp \
    ../Commands/controlword.cpp \
    ../../UtilityUWB/ptreewr.cpp \
    ../../UtilityUWB/networkuwb.cpp \
    ../../UtilityUWB/serialport.cpp \
    ../../UtilityUWB/errorcodeboost.cpp

HEADERS += \
    recievercontrolmodule.h \
    rs485handler.h \
    commandcontrol.h \
    controllercommands.h \
    interfaces.h \
    observer.h \
    ../Commands/controlword.h \
    ../../UtilityUWB/ptreewr.h \
    ../../UtilityUWB/networkuwb.h \
    ../../UtilityUWB/serialport.h \
    ../../UtilityUWB/errorcodeboost.h

LIBS += -lboost_system
LIBS += -pthread


