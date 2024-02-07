TARGET = IQtoMagnitudeBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

unix:LIBS += -lfftw3

SOURCES += \
    iqtomagnitudeblock.cpp \
    iqtomagnitudemodule.cpp \

HEADERS += \
    iqtomagnitudeblock.h \
    iqtomagnitudemodule.h

DISTFILES += iqtomagnitudeblock.json
