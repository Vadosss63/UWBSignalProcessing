TARGET = ScopeComplexBlock

include(../../include.pri)

DESTDIR = $$SINK_PATH

INCLUDEPATH += $${PATH_TO_API}/Tools
DEPENDPATH += $${PATH_TO_API}/Tools

SOURCES += \
    scopecomplexblock.cpp \
    scopecomplexdialogbox.cpp \
    scopecomplexmodule.cpp \
    $${PATH_TO_API}/Tools/plotter.cpp

HEADERS += \
    scopecomplexblock.h \
    scopecomplexdialogbox.h \
    scopecomplexmodule.h \
    $${PATH_TO_API}/Tools/plotter.h\
    $${PATH_TO_API}/Tools/plotter_interface.h

DISTFILES += scopecomplexblock.json
