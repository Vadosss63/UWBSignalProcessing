TARGET = SinGeneratorBlock

include(../../include.pri)

DESTDIR = $$SOURCE_PATH

SOURCES += \
    singeneratorblock.cpp \
    singeneratordialogbox.cpp \
    singeneratormodule.cpp

HEADERS += \
    singeneratorblock.h \
    singeneratordialogbox.h \
    singeneratormodule.h

DISTFILES += singeneratorblock.json
