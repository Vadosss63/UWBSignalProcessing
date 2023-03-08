TARGET = ScopeComplexBlock

include(../../include.pri)

DESTDIR = $$SINK_PATH

INCLUDEPATH += $$PWD/../../../Tools
DEPENDPATH += $$PWD/../../../Tools

SOURCES += \
    scopecomplexblock.cpp \
    scopecomplexdialogbox.cpp \
    scopecomplexmodule.cpp \
    ../../../Tools/plotter.cpp

HEADERS += \
    scopecomplexblock.h \
    scopecomplexdialogbox.h \
    scopecomplexmodule.h \
    ../../../Tools/plotter.h\
    ../../../Tools/plotter_interface.h

DISTFILES += scopecomplexblock.json
