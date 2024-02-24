TARGET = ScopeBufferBlock

include(../../include.pri)

DESTDIR = $$SINK_PATH

INCLUDEPATH += $${PATH_TO_API}/Tools
DEPENDPATH += $${PATH_TO_API}/Tools

SOURCES += \
    scopebufferblock.cpp \
    scopebufferdialogbox.cpp \
    scopebuffermodule.cpp \
    $${PATH_TO_API}/Tools/plotter.cpp

HEADERS += \
    scopebufferblock.h \
    scopebufferdialogbox.h \
    scopebuffermodule.h \
    $${PATH_TO_API}/Tools/plotter.h\
    $${PATH_TO_API}/Tools/plotter_interface.h

DISTFILES += scopebufferblock.json
