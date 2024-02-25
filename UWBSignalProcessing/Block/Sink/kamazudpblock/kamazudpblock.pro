TARGET = KamazUDPBlock

include(../../include.pri)

DESTDIR = $$SINK_PATH

INCLUDEPATH += $${PATH_TO_API}/../UtilityUWB
DEPENDPATH += $${PATH_TO_API}/../UtilityUWB

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
