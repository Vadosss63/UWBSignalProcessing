TARGET = TransmitterUDPBlock

include(../../include.pri)

DESTDIR = $$SINK_PATH

INCLUDEPATH += $${PATH_TO_API}/UtilityUWB
DEPENDPATH += $${PATH_TO_API}/UtilityUWB

SOURCES += \
    transmitterudpblock.cpp \
    transmitterudpmodule.cpp \
    $${PATH_TO_API}/UtilityUWB/errorcodeboost.cpp \
    $${PATH_TO_API}/UtilityUWB/networkuwb.cpp \

HEADERS += \
    transmitterudpblock.h \
    transmitterudpmodule.h \
    $${PATH_TO_API}/UtilityUWB/errorcodeboost.h \
    $${PATH_TO_API}/UtilityUWB/networkuwb.h

DISTFILES += transmitterudpblock.json
