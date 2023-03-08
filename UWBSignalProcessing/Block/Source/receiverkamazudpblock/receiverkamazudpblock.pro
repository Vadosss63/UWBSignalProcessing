TARGET = ReceiverKamazUDPBlock

include(../../include.pri)

DESTDIR = $$SOURCE_PATH

INCLUDEPATH += $$PWD/../../../../UtilityUWB
DEPENDPATH += $$PWD/../../../../UtilityUWB

SOURCES += \
    receiverkamazudpblock.cpp \
    receiverkamazudpmodule.cpp \
    ../../../../UtilityUWB/networkuwb.cpp \
    ../../../../UtilityUWB/errorcodeboost.cpp

HEADERS += \
    receiverkamazudpblock.h \
    receiverkamazudpmodule.h \
    kamazdatagramprli.h \
    ../../../../UtilityUWB/networkuwb.h \
    ../../../../UtilityUWB/errorcodeboost.h


DISTFILES += receiverkamazudpblock.json
