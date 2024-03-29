TARGET = 3DTargetBlock

include(../../include.pri)

QT += opengl
win32: LIBS += -lglu32 -lopengl32

DESTDIR = $$SINK_PATH

INCLUDEPATH += $${PATH_TO_API}/DSPMLib/Tools
DEPENDPATH += $${PATH_TO_API}/DSPMLib/Tools

SOURCES += \
    arrayvertex.cpp \
    3dtargetblock.cpp \
    3dtargetdialogbox.cpp \
    3dtargetmodule.cpp\
    $${PATH_TO_API}/DSPMLib/Tools/target.cpp


HEADERS += \
    arrayvertex.h \
    interface.h \ 
    3dtargetblock.h \
    3dtargetdialogbox.h \
    3dtargetmodule.h\
    $${PATH_TO_API}/DSPMLib/Tools/target.h

DISTFILES += 3dtargetblock.json
