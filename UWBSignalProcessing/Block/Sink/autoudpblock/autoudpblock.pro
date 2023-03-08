TARGET = AutoUDPBlock

include(../../include.pri)

DESTDIR = $$SINK_PATH

INCLUDEPATH += $$PWD/../../../DSPMLib/Sink
DEPENDPATH += $$PWD/../../../DSPMLib/Sink
INCLUDEPATH += $$PWD/../../../../UtilityUWB
DEPENDPATH += $$PWD/../../../../UtilityUWB

SOURCES += \
    autoudpblock.cpp \
    autoudpmodule.cpp \
    ../../../../UtilityUWB/errorcodeboost.cpp \
    ../../../../UtilityUWB/networkuwb.cpp \
    ../../../DSPMLib/Tools/target.cpp

HEADERS += \
    autoudpblock.h \
    autoudpmodule.h \
    ../../../../UtilityUWB/errorcodeboost.h \
    ../../../../UtilityUWB/networkuwb.h \
    ../../../DSPMLib/Tools/target.h \
    ../../../DSPMLib/Tools/autodatagram.h

DISTFILES += autoudpblock.json
