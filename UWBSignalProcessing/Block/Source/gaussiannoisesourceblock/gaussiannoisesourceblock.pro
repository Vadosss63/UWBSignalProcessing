TARGET = GaussianNoisesourceBlock

include(../../include.pri)

DESTDIR = $$SOURCE_PATH

SOURCES += \
    gaussiannoisesourceblock.cpp \
    gaussiannoisesourcedialogbox.cpp \
    gaussiannoisesourcemodule.cpp

HEADERS += \
    gaussiannoisesourceblock.h \
    gaussiannoisesourcedialogbox.h \
    gaussiannoisesourcemodule.h


DISTFILES += gaussiannoisesourceblock.json
