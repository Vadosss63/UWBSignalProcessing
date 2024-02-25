TARGET = ReceiverUDPBlock

include(../../include.pri)

DESTDIR = $$SOURCE_PATH

INCLUDEPATH += $${PATH_TO_API}/UtilityUWB
DEPENDPATH += $${PATH_TO_API}/UtilityUWB

SOURCES += \
    $${PATH_TO_API}/UtilityUWB/networkuwb.cpp \
    $${PATH_TO_API}/UtilityUWB/errorcodeboost.cpp \
    receiverudpblock.cpp \
    receiverudpmodule.cpp

HEADERS += \
    $${PATH_TO_API}/UtilityUWB/networkuwb.h \
    $${PATH_TO_API}/UtilityUWB/errorcodeboost.h \
    receiverudpblock.h \
    receiverudpmodule.h

DISTFILES += receiverudpblock.json
