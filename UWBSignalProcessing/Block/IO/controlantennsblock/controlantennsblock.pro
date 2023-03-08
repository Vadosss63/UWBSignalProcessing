TARGET = ControlAntennsBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    controlantennsblock.cpp \
    controlantennsmodule.cpp

HEADERS += \
    controlantennsblock.h \
    controlantennsmodule.h

DISTFILES += controlantennsblock.json
