TARGET = ShowtargetBlock
include(../../include.pri)

DESTDIR = $$SINK_PATH

INCLUDEPATH += $${PATH_TO_API}/DSPMLib/Tools
DEPENDPATH += $${PATH_TO_API}/DSPMLib/Tools

SOURCES += \
    showtargetblock.cpp \
    sectorgriditem.cpp \
    showtargetdialogbox.cpp \
    showtargetmodule.cpp \
    targetitem.cpp\
    $${PATH_TO_API}/DSPMLib/Tools/target.cpp

HEADERS += \
    showtargetblock.h \
    intrface.h \
    sectorgriditem.h \
    showtargetdialogbox.h \
    showtargetmodule.h \
    $${PATH_TO_API}/DSPMLib/Tools/target.h \
    targetitem.h

DISTFILES += showtargetblock.json

RESOURCES += \
    res.qrc
