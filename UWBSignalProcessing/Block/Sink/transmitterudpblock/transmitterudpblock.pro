TARGET = TransmitterUDPBlock

include(../../include.pri)

DESTDIR = $$SINK_PATH

INCLUDEPATH += $$PWD/../../../DSPMLib/Sink
DEPENDPATH += $$PWD/../../../DSPMLib/Sink
INCLUDEPATH += $$PWD/../../../../UtilityUWB
DEPENDPATH += $$PWD/../../../../UtilityUWB

SOURCES += \
    transmitterudpblock.cpp \
    transmitterudpmodule.cpp \
    ../../../../UtilityUWB/errorcodeboost.cpp \
    ../../../../UtilityUWB/networkuwb.cpp \

HEADERS += \
    transmitterudpblock.h \
    transmitterudpmodule.h \
    ../../../../UtilityUWB/errorcodeboost.h \
    ../../../../UtilityUWB/networkuwb.h


DISTFILES += transmitterudpblock.json
