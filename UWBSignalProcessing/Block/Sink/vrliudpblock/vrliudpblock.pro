
TARGET = VRLIUDPBlock
include(../../include.pri)

DESTDIR = $$SINK_PATH

INCLUDEPATH += $$PWD/../../../../UtilityUWB
DEPENDPATH += $$PWD/../../../../UtilityUWB

SOURCES += \
    vrliudpblock.cpp \
    vrliudpmodule.cpp \
    ../../../../UtilityUWB/errorcodeboost.cpp \
    ../../../../UtilityUWB/networkuwb.cpp

HEADERS += \
    vrliudpblock.h \
    vrliudpmodule.h \
    ../../../../UtilityUWB/errorcodeboost.h \
    ../../../../UtilityUWB/networkuwb.h


DISTFILES += vrliudpblock.json
