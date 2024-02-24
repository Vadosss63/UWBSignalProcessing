TARGET = AutoUDPBlock

include(../../include.pri)

DESTDIR = $$SINK_PATH

INCLUDEPATH += $${PATH_TO_API}/DSPMLib/Sink
DEPENDPATH += $${PATH_TO_API}/DSPMLib/Sink
INCLUDEPATH += $${PATH_TO_API}/../UtilityUWB
DEPENDPATH += $${PATH_TO_API}/../UtilityUWB
INCLUDEPATH += $${PATH_TO_API}/DSPMLib/Tools
DEPENDPATH += $${PATH_TO_API}/DSPMLib/Tools

SOURCES += \
    autoudpblock.cpp \
    autoudpmodule.cpp \
    $${PATH_TO_API}/../UtilityUWB/errorcodeboost.cpp \
    $${PATH_TO_API}/../UtilityUWB/networkuwb.cpp \
    $${PATH_TO_API}/DSPMLib/Tools/target.cpp

HEADERS += \
    autoudpblock.h \
    autoudpmodule.h \
    $${PATH_TO_API}/../UtilityUWB/errorcodeboost.h \
    $${PATH_TO_API}/../UtilityUWB/networkuwb.h \
    $${PATH_TO_API}/DSPMLib/Tools/target.h \
    $${PATH_TO_API}/DSPMLib/Tools/autodatagram.h

DISTFILES += autoudpblock.json
