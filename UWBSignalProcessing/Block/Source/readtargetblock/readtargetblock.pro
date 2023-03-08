TARGET = ReadTargetBlock

include(../../include.pri)

DESTDIR = $$SOURCE_PATH

SOURCES += \
    readtargetblock.cpp \
    readtargetdialogbox.cpp \
    readtargetmodule.cpp \
    ../../../DSPMLib/Tools/target.cpp

HEADERS += \
    readtargetblock.h \
    readtargetdialogbox.h \
    readtargetmodule.h\
    ../../../DSPMLib/Tools/target.h\
    ../../../DSPMLib/Tools/autodatagram.h


DISTFILES += \
    readtargetblock.json
