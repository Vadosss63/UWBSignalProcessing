TARGET = ReadTargetBlock

include(../../include.pri)

DESTDIR = $$SOURCE_PATH

INCLUDEPATH += $${PATH_TO_API}/DSPMLib/Tools
DEPENDPATH += $${PATH_TO_API}/DSPMLib/Tools

SOURCES += \
    readtargetblock.cpp \
    readtargetdialogbox.cpp \
    readtargetmodule.cpp \
    $${PATH_TO_API}/DSPMLib/Tools/target.cpp

HEADERS += \
    readtargetblock.h \
    readtargetdialogbox.h \
    readtargetmodule.h\
    $${PATH_TO_API}/DSPMLib/Tools/target.h\
    $${PATH_TO_API}/DSPMLib/Tools/autodatagram.h


DISTFILES += \
    readtargetblock.json
