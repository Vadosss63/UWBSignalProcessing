TARGET = ShowtargetBlock
include(../../include.pri)

DESTDIR = $$SINK_PATH

INCLUDEPATH += $$PWD/../../../DSPMLib/Sink
DEPENDPATH += $$PWD/../../../DSPMLib/Sink

SOURCES += \
    showtargetblock.cpp \
    sectorgriditem.cpp \
    showtargetdialogbox.cpp \
    showtargetmodule.cpp \
    targetitem.cpp\
    ../../../DSPMLib/Tools/target.cpp

HEADERS += \
    showtargetblock.h \
    intrface.h \
    sectorgriditem.h \
    showtargetdialogbox.h \
    showtargetmodule.h \
    ../../../DSPMLib/Tools/target.h \
    targetitem.h

DISTFILES += showtargetblock.json

RESOURCES += \
    res.qrc
