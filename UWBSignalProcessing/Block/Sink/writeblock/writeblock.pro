TARGET = WriteBlock

include(../../include.pri)

DESTDIR = $$SINK_PATH

SOURCES += \
    writeblock.cpp \
    writedialogbox.cpp \
    writemodule.cpp

HEADERS += \
    writeblock.h \
    writedialogbox.h \
    writemodule.h

DISTFILES += writeblock.json
