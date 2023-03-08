TARGET = ReceiverUDPBlock

include(../../include.pri)

DESTDIR = $$SOURCE_PATH

INCLUDEPATH += $$PWD/../../../../UtilityUWB
DEPENDPATH += $$PWD/../../../../UtilityUWB

SOURCES += \
    ../../../../UtilityUWB/networkuwb.cpp \
    ../../../../UtilityUWB/errorcodeboost.cpp \
    receiverudpblock.cpp \
    receiverudpmodule.cpp


HEADERS += \
    ../../../../UtilityUWB/networkuwb.h \
    ../../../../UtilityUWB/errorcodeboost.h \
    receiverudpblock.h \
    receiverudpmodule.h



DISTFILES += \
    receiverudpblock.json
