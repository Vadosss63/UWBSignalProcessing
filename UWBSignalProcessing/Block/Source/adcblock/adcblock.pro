TARGET = ADCBlock

include(../../include.pri)

QT += network

DESTDIR = $$SOURCE_PATH

INCLUDEPATH += $$PWD/../../../../UtilityUWB
DEPENDPATH += $$PWD/../../../../UtilityUWB

INCLUDEPATH += $$PWD/../../../DSPMLib/Tools
DEPENDPATH += $$PWD/../../../DSPMLib/Tools

INCLUDEPATH += $$PWD/../../../DataProtocol
DEPENDPATH += $$PWD/../../../DataProtocol

INCLUDEPATH += $$PWD/Reciver/UserInterfaceReciver
DEPENDPATH += $$PWD/Reciver/UserInterfaceReciver

INCLUDEPATH += $$PWD/Reciver/Commands
DEPENDPATH += $$PWD/Reciver/Commands

SOURCES += \
    adcblock.cpp \
    adcdialogbox.cpp \
    adcmodule.cpp \
    ../../../../UtilityUWB/networkuwb.cpp \
    ../../../../UtilityUWB/errorcodeboost.cpp \
    Reciver/UserInterfaceReciver/ptreecommands.cpp \
    Reciver/UserInterfaceReciver/ptreecommandswriter.cpp \
    Reciver/UserInterfaceReciver/rs485dialogbox.cpp \
    Reciver/Commands/controlword.cpp \
    ../../../Tools/crc16.cpp \
    ../../../../UtilityUWB/ptreewr.cpp

HEADERS += \
    adcblock.h \
    adcdialogbox.h \
    adcmodule.h \
    ../../../../UtilityUWB/networkuwb.h \
    ../../../../UtilityUWB/errorcodeboost.h \
    Reciver/UserInterfaceReciver/ptreecommands.h \
    Reciver/UserInterfaceReciver/ptreecommandswriter.h \
    Reciver/UserInterfaceReciver/rs485dialogbox.h \
    Reciver/Commands/controlword.h \
    ../../../Tools/crc16.h \
    ../../../../UtilityUWB/ptreewr.h

DISTFILES += adcblock.json

FORMS += \
    adcdialogbox.ui \
    Reciver/UserInterfaceReciver/rs485dialogbox.ui
