TARGET = FFTBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

unix:LIBS += -lfftw3

SOURCES += \
    fftblock.cpp \
    fftmodule.cpp

HEADERS += \
    fftblock.h \
    fftmodule.h

DISTFILES += fftblock.json
