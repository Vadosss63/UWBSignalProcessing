TARGET = MagnitudetoIQBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

unix:LIBS += -lfftw3

SOURCES += \
    magnitudetoiqblock.cpp \
    magnitudetoiqmodule.cpp

HEADERS += \
    magnitudetoiqblock.h \
    magnitudetoiqmodule.h


DISTFILES += magnitudetoiqblock.json
