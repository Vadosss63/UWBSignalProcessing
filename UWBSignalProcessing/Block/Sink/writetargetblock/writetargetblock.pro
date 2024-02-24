TARGET = WriteTargetBlock

include(../../include.pri)

DESTDIR = $$SINK_PATH

INCLUDEPATH += $${PATH_TO_API}/DSPMLib/Tools
DEPENDPATH += $${PATH_TO_API}/DSPMLib/Tools

SOURCES += \
    writetargetblock.cpp \
    writetargetdialogbox.cpp \
    writetargetmodule.cpp \
    $${PATH_TO_API}/DSPMLib/Tools/target.cpp

HEADERS += \
    writetargetblock.h \
    writetargetdialogbox.h \
    writetargetmodule.h \
    $${PATH_TO_API}/DSPMLib/Tools/target.h \
    $${PATH_TO_API}/DSPMLib/Tools/autodatagram.h

DISTFILES += \
    writetargetblock.json
