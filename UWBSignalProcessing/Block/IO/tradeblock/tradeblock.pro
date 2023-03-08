TARGET = TradeBlock

include(../../include.pri)

DESTDIR = $$IO_PATH

SOURCES += \
    tradeblock.cpp \
    tradedialogbox.cpp \
    trademodule.cpp

HEADERS += \
    tradeblock.h \
    tradedialogbox.h \
    trademodule.h

DISTFILES += tradeblock.json
