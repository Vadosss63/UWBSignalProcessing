TARGET = VRLIUDPBlock

include(../../include.pri)

DESTDIR = $$SINK_PATH

INCLUDEPATH += $${PATH_TO_API}/UtilityUWB
DEPENDPATH += $${PATH_TO_API}/UtilityUWB

SOURCES += \
    vrliudpblock.cpp \
    vrliudpmodule.cpp \
    $${PATH_TO_API}/UtilityUWB/errorcodeboost.cpp \
    $${PATH_TO_API}/UtilityUWB/networkuwb.cpp

HEADERS += \
    vrliudpblock.h \
    vrliudpmodule.h \
    $${PATH_TO_API}/UtilityUWB/errorcodeboost.h \
    $${PATH_TO_API}/UtilityUWB/networkuwb.h

DISTFILES += vrliudpblock.json
