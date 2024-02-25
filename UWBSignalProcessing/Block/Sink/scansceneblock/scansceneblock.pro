TARGET = ScanSceneBlock

include(../../include.pri)

DESTDIR = $$SINK_PATH

QT += opengl

SOURCES += \
    scansceneblock.cpp \
    scanscenedialogbox.cpp \
    scanscenemodule.cpp \
    sectorgriditem.cpp \
    sectoritem.cpp \
    panelscan.cpp \
    vrliitem.cpp \
    vrliviewmodule.cpp

HEADERS += \
    scansceneblock.h \
    scanscenedialogbox.h \
    scanscenemodule.h \
    sectorgriditem.h \
    sectoritem.h \
    panelscan.h \
    vrliitem.h \
    vrliviewmodule.h

DISTFILES += scansceneblock.json

FORMS += panelscan.ui
