QT += core gui widgets
CONFIG += plugin c++17
DEFINES += QT_DEPRECATED_WARNINGS
TEMPLATE = lib

PATH_TO_API = $$PWD/..

INCLUDEPATH += $${PATH_TO_API}/DSPMLib
DEPENDPATH += $${PATH_TO_API}/DSPMLib
INCLUDEPATH += $${PATH_TO_API}/DSPMLib/Tools
DEPENDPATH += $${PATH_TO_API}/DSPMLib/Tools
INCLUDEPATH += $${PATH_TO_API}/MainWindow
DEPENDPATH += $${PATH_TO_API}/MainWindow
INCLUDEPATH += $${PATH_TO_API}/DialogBox
DEPENDPATH += $${PATH_TO_API}/DialogBox
INCLUDEPATH += $${PATH_TO_API}/Intefaces
DEPENDPATH += $${PATH_TO_API}/Intefaces

INCLUDEPATH += $${PATH_TO_API}/Tools
DEPENDPATH += $${PATH_TO_API}/Tools

NAME_FILE = $${TARGET}.json

unix:BILD_DIR = $${files(/home/*)}/UWB/BlocksLib

IO_PATH = $${BILD_DIR}/IOBlocks
SINK_PATH = $${BILD_DIR}/SinkBlocks
SOURCE_PATH = $${BILD_DIR}/SourceBlocks

SOURCES += \
    $${PATH_TO_API}/DSPMLib/abstractmodule.cpp \
    $${PATH_TO_API}/DSPMLib/csignal.cpp \
    $${PATH_TO_API}/DialogBox/customdialwidget.cpp \
    $${PATH_TO_API}/MainWindow/abstractblock.cpp \
    $${PATH_TO_API}/MainWindow/ioblock.cpp \
    $${PATH_TO_API}/MainWindow/sourceblock.cpp \
    $${PATH_TO_API}/MainWindow/sinkblock.cpp \
    $${PATH_TO_API}/MainWindow/abstractdialogcommand.cpp \
    $${PATH_TO_API}/Tools/timerevent.cpp \
    $${PATH_TO_API}/MainWindow/lineconnect.cpp


HEADERS += \
    $${PATH_TO_API}/DSPMLib/abstractmodule.h \
    $${PATH_TO_API}/DSPMLib/iomodule.h \
    $${PATH_TO_API}/DSPMLib/sinkmodule.h \
    $${PATH_TO_API}/DSPMLib/sourcemodule.h \
    $${PATH_TO_API}/DSPMLib/csignal.h \
    $${PATH_TO_API}/Intefaces/bilderblocks.h \
    $${PATH_TO_API}/Intefaces/abstractoperationmanager.h \
    $${PATH_TO_API}/Intefaces/changeevent.h \
    $${PATH_TO_API}/Intefaces/dialogwindow.h \
    $${PATH_TO_API}/Intefaces/moduleconnectports.h \
    $${PATH_TO_API}/DialogBox/customdialwidget.h \
    $${PATH_TO_API}/MainWindow/abstractblock.h \
    $${PATH_TO_API}/MainWindow/ioblock.h \
    $${PATH_TO_API}/MainWindow/sinkblock.h \
    $${PATH_TO_API}/MainWindow/sourceblock.h \
    $${PATH_TO_API}/MainWindow/abstractdialogcommand.h \
    $${PATH_TO_API}/Tools/timerevent.h \
    $${PATH_TO_API}/MainWindow/lineconnect.h \
    $${PATH_TO_API}/DSPMLib/portsconnect.h \
    $${PATH_TO_API}/DSPMLib/datavrli.h \
    $${PATH_TO_API}/DSPMLib/typesignal.h
