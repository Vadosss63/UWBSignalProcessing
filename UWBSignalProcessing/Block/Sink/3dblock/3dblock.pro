TARGET = 3DBlock

include(../../include.pri)

QT += opengl
win32: LIBS += -lglu32 -lopengl32

DESTDIR = $$SINK_PATH

SOURCES += \
    3dblock.cpp \
    3ddialogbox.cpp \
    3dmodule.cpp \
    arrayvertex.cpp

HEADERS += \
    3dblock.h \
    3ddialogbox.h \
    3dmodule.h \
    arrayvertex.h \
    interface.h

DISTFILES += 3dblock.json
