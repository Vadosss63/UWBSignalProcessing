TARGET = ReceiverKamazUDPBlock

include(../../include.pri)

DESTDIR = $$SOURCE_PATH

INCLUDEPATH += $${PATH_TO_API}/UtilityUWB
DEPENDPATH += $${PATH_TO_API}/UtilityUWB

SOURCES += \
    receiverkamazudpblock.cpp \
    receiverkamazudpmodule.cpp \
    $${PATH_TO_API}/UtilityUWB/networkuwb.cpp \
    $${PATH_TO_API}/UtilityUWB/errorcodeboost.cpp

HEADERS += \
    receiverkamazudpblock.h \
    receiverkamazudpmodule.h \
    kamazdatagramprli.h \
    $${PATH_TO_API}/UtilityUWB/networkuwb.h \
    $${PATH_TO_API}/UtilityUWB/errorcodeboost.h

DISTFILES += receiverkamazudpblock.json
