TARGET = ReceiverAutoUDPBlock

include(../../include.pri)

DESTDIR = $$SOURCE_PATH

INCLUDEPATH += $${PATH_TO_API}/UtilityUWB
DEPENDPATH += $${PATH_TO_API}/UtilityUWB

INCLUDEPATH += $${PATH_TO_API}/DSPMLib/Tools
DEPENDPATH += $${PATH_TO_API}/DSPMLib/Tools

SOURCES += \
    $${PATH_TO_API}/UtilityUWB/networkuwb.cpp \
    $${PATH_TO_API}/UtilityUWB/errorcodeboost.cpp \
    receiverautoudpblock.cpp \
    receiverautoudpmodule.cpp \
    $${PATH_TO_API}/DSPMLib/Tools/target.cpp


HEADERS += \
    $${PATH_TO_API}/UtilityUWB/networkuwb.h \
    $${PATH_TO_API}/UtilityUWB/errorcodeboost.h \
    receiverautoudpblock.h \
    receiverautoudpmodule.h \
    $${PATH_TO_API}/DSPMLib/Tools/target.h\
    $${PATH_TO_API}/DSPMLib/Tools/autodatagram.h

DISTFILES += \
    receiverautoudpblock.json
