
TARGET = IQToComplexblock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    iqtocomplexblock.cpp \
    iqtocomplexmodule.cpp

HEADERS += \
    iqtocomplexblock.h \
    iqtocomplexmodule.h

DISTFILES += iqtocomplexblock.json
