TARGET = ReceiverUDPBlock
include(../../include.pri)

DESTDIR = $$SOURCE_PATH



INCLUDEPATH += $$PWD/../../../DSPMLib/Tools
DEPENDPATH += $$PWD/../../../DSPMLib/Tools

INCLUDEPATH += $$PWD/../../../DataProtocol
DEPENDPATH += $$PWD/../../../DataProtocol

INCLUDEPATH += $$PWD/../../../Tools
DEPENDPATH += $$PWD/../../../Tools

SOURCES += \
    receiverudpblock.cpp \
    receiverudpdialogbox.cpp \
    receiverudpmodule.cpp \
    ../../../DSPMLib/Tools/network.cpp \
    ../../../DataProtocol/datagramprli.cpp \
    ../../../DataProtocol/DataParser.cpp \
    ../../../DataProtocol/datatargetimitation.cpp \
    ../../../Tools/crc16.cpp

HEADERS += \
    receiverudpblock.h \
    receiverudpdialogbox.h \
    receiverudpmodule.h \
    ../../../DataProtocol/datagramheder.h \
    ../../../DataProtocol/datagramkud.h \
    ../../../DataProtocol/datagramprli.h \
    ../../../DataProtocol/datagramvrli.h \
    ../../../DataProtocol/dataparser.h \
    ../../../DataProtocol/datatargetimitation.h \
    ../../../Tools/crc16.h


DISTFILES += receiverudpblock.json
