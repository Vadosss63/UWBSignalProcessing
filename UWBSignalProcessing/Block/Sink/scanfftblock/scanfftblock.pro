TARGET = ScanfftBlock

include(../../include.pri)

DESTDIR = $$SINK_PATH

SOURCES += \
    scanfftblock.cpp \
    scanfftdialogbox.cpp \
    scanfftmodule.cpp \
    sectoritem.cpp \
    sectorgriditem.cpp \
    fft_plotter.cpp

HEADERS += \
    scanfftblock.h \
    scanfftdialogbox.h \
    scanfftmodule.h \
    sectoritem.h \
    intrface.h \
    sectorgriditem.h \
    fft_plotter.h

DISTFILES += scanfftblock.json
