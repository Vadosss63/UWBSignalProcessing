TARGET = ReceiverAutoUDPBlock

include(../../include.pri)

DESTDIR = $$SOURCE_PATH

INCLUDEPATH += $$PWD/../../../../UtilityUWB
DEPENDPATH += $$PWD/../../../../UtilityUWB

SOURCES += \
    ../../../../UtilityUWB/networkuwb.cpp \
    ../../../../UtilityUWB/errorcodeboost.cpp \
    receiverautoudpblock.cpp \
    receiverautoudpmodule.cpp \
    ../../../DSPMLib/Tools/target.cpp


HEADERS += \
    ../../../../UtilityUWB/networkuwb.h \
    ../../../../UtilityUWB/errorcodeboost.h \
    receiverautoudpblock.h \
    receiverautoudpmodule.h \
    ../../../DSPMLib/Tools/target.h\
    ../../../DSPMLib/Tools/autodatagram.h



DISTFILES += \
    receiverautoudpblock.json
