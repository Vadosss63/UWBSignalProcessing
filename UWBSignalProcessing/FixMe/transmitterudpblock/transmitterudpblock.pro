TARGET = TransmitterUDPBlock

include(../../include.pri)

DESTDIR = $$SINK_PATH

INCLUDEPATH += $$PWD/../../../DSPMLib/Sink
DEPENDPATH += $$PWD/../../../DSPMLib/Sink
INCLUDEPATH += $$PWD/../../../DataProtocol
DEPENDPATH += $$PWD/../../../DataProtocol
INCLUDEPATH += $$PWD/../../../Tools
DEPENDPATH += $$PWD/../../../Tools

SOURCES += \
    transmitterudpblock.cpp \
    transmitterudpmodule.cpp \
    ../../../DSPMLib/Sink/endpointmodule.cpp \
    ../../../Tools/crc16.cpp \
    ../../../DataProtocol/datagramprli.cpp \
    ../../../DataProtocol/DataParser.cpp \
    ../../../DataProtocol/datatargetimitation.cpp

HEADERS += \
    transmitterudpblock.h \
    transmitterudpmodule.h \
    ../../../DSPMLib/Sink/endpointmodule.h \
    ../../../Tools/crc16.h \
    ../../../DataProtocol/datagramheder.h \
    ../../../DataProtocol/datagramkud.h \
    ../../../DataProtocol/datagramprli.h \
    ../../../DataProtocol/datagramvrli.h \
    ../../../DataProtocol/dataparser.h \
    ../../../DataProtocol/datatargetimitation.h


DISTFILES += transmitterudpblock.json
