TARGET = 3DTargetBlock

QT += opengl
win32: LIBS += -lglu32 -lopengl32

include(../../include.pri)

DESTDIR = $$SINK_PATH

INCLUDEPATH += $$PWD/../../../DSPMLib/Sink
DEPENDPATH += $$PWD/../../../DSPMLib/Sink

SOURCES += \
    arrayvertex.cpp \
    3dtargetblock.cpp \
    3dtargetdialogbox.cpp \
    3dtargetmodule.cpp\
    ../../../DSPMLib/Tools/target.cpp


HEADERS += \
    arrayvertex.h \
    interface.h \ 
    3dtargetblock.h \
    3dtargetdialogbox.h \
    3dtargetmodule.h\
    ../../../DSPMLib/Tools/target.h

DISTFILES += \
    3dtargetblock.json
