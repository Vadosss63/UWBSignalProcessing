TARGET = ADCBlock

include(../../include.pri)

QT += network

DESTDIR = $$SOURCE_PATH

INCLUDEPATH += $${PATH_TO_API}/UtilityUWB
DEPENDPATH += $${PATH_TO_API}/UtilityUWB

INCLUDEPATH += $${PATH_TO_API}/DSPMLib/Tools
DEPENDPATH += $${PATH_TO_API}/DSPMLib/Tools

INCLUDEPATH += $$PWD/Reciver/UserInterfaceReciver
DEPENDPATH += $$PWD/Reciver/UserInterfaceReciver

INCLUDEPATH += $$PWD/Reciver/Commands
DEPENDPATH += $$PWD/Reciver/Commands

SOURCES += \
    adcblock.cpp \
    adcdialogbox.cpp \
    adcmodule.cpp \
    $${PATH_TO_API}/UtilityUWB/networkuwb.cpp \
    $${PATH_TO_API}/UtilityUWB/errorcodeboost.cpp \
    Reciver/UserInterfaceReciver/ptreecommands.cpp \
    Reciver/UserInterfaceReciver/ptreecommandswriter.cpp \
    Reciver/UserInterfaceReciver/rs485dialogbox.cpp \
    Reciver/Commands/controlword.cpp \
    $${PATH_TO_API}/Tools/crc16.cpp \
    $${PATH_TO_API}/UtilityUWB/ptreewr.cpp

HEADERS += \
    adcblock.h \
    adcdialogbox.h \
    adcmodule.h \
    $${PATH_TO_API}/UtilityUWB/networkuwb.h \
    $${PATH_TO_API}/UtilityUWB/errorcodeboost.h \
    Reciver/UserInterfaceReciver/ptreecommands.h \
    Reciver/UserInterfaceReciver/ptreecommandswriter.h \
    Reciver/UserInterfaceReciver/rs485dialogbox.h \
    Reciver/Commands/controlword.h \
    $${PATH_TO_API}/Tools/crc16.h \
    $${PATH_TO_API}/UtilityUWB/ptreewr.h

DISTFILES += adcblock.json

FORMS += \
    adcdialogbox.ui \
    Reciver/UserInterfaceReciver/rs485dialogbox.ui
