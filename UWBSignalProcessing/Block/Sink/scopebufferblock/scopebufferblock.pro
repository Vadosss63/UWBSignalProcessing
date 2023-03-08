TARGET = ScopeBufferBlock

include(../../include.pri)

DESTDIR = $$SINK_PATH

INCLUDEPATH += $$PWD/../../../Tools
DEPENDPATH += $$PWD/../../../Tools

SOURCES += \
    scopebufferblock.cpp \
    scopebufferdialogbox.cpp \
    scopebuffermodule.cpp \
    ../../../Tools/plotter.cpp

HEADERS += \
    scopebufferblock.h \
    scopebufferdialogbox.h \
    scopebuffermodule.h \
    ../../../Tools/plotter.h\
    ../../../Tools/plotter_interface.h

DISTFILES += scopebufferblock.json
