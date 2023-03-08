TARGET = WriteTargetBlock

include(../../include.pri)

DESTDIR = $$SINK_PATH

SOURCES += \
    writetargetblock.cpp \
    writetargetdialogbox.cpp \
    writetargetmodule.cpp \
    ../../../DSPMLib/Tools/target.cpp

HEADERS += \
    writetargetblock.h \
    writetargetdialogbox.h \
    writetargetmodule.h \
    ../../../DSPMLib/Tools/target.h \
    ../../../DSPMLib/Tools/autodatagram.h

DISTFILES += \
    writetargetblock.json
