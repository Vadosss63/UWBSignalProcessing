TARGET = 3DBlock
QT += opengl

win32: LIBS += -lglu32 -lopengl32
include(../../include.pri)

DESTDIR = $$SINK_PATH

INCLUDEPATH += $$PWD/../../../DSPMLib/Sink
DEPENDPATH += $$PWD/../../../DSPMLib/Sink

SOURCES += \
    3dblock.cpp \
    3ddialogbox.cpp \
    3dmodule.cpp \
    arrayvertex.cpp
    #scopewidgetopgl.cpp

HEADERS += \
    3dblock.h \
    3ddialogbox.h \
    3dmodule.h \
    arrayvertex.h \
    interface.h
    #scopewidgetopgl.h

DISTFILES += 3dblock.json
