TARGET = FFTBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    fftblock.cpp \
    fftmodule.cpp

HEADERS += \
    fftblock.h \
    fftmodule.h

DISTFILES += fftblock.json
