TARGET = KamazUDPBlock

include(../../include.pri)

DESTDIR = $$SINK_PATH

INCLUDEPATH += $${PATH_TO_API}/UtilityUWB
DEPENDPATH += $${PATH_TO_API}/UtilityUWB

SOURCES += \
    kamazudpblock.cpp \
    kamazudpmodule.cpp \
    $${PATH_TO_API}/UtilityUWB/errorcodeboost.cpp \
    $${PATH_TO_API}/UtilityUWB/networkuwb.cpp

HEADERS += \
    kamazudpblock.h \
    kamazudpmodule.h \
    $${PATH_TO_API}/UtilityUWB/errorcodeboost.h \
    $${PATH_TO_API}/UtilityUWB/networkuwb.h \
    kamazdatagramprli.h

DISTFILES += kamazudpblock.json
