TARGET = KamazUDPBlock

include(../../include.pri)

DESTDIR = $$SINK_PATH

INCLUDEPATH += $$PWD/../../../DSPMLib/Sink
DEPENDPATH += $$PWD/../../../DSPMLib/Sink
INCLUDEPATH += $$PWD/../../../../UtilityUWB
DEPENDPATH += $$PWD/../../../../UtilityUWB

SOURCES += \
    kamazudpblock.cpp \
    kamazudpmodule.cpp \
    ../../../../UtilityUWB/errorcodeboost.cpp \
    ../../../../UtilityUWB/networkuwb.cpp

HEADERS += \
    kamazudpblock.h \
    kamazudpmodule.h \
    ../../../../UtilityUWB/errorcodeboost.h \
    ../../../../UtilityUWB/networkuwb.h \
    kamazdatagramprli.h

DISTFILES += kamazudpblock.json
