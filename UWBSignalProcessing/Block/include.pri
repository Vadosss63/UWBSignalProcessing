QT += core gui widgets
CONFIG += plugin c++17
DEFINES += QT_DEPRECATED_WARNINGS
TEMPLATE = lib

INCLUDEPATH += $$PWD/../DSPMLib
DEPENDPATH += $$PWD/../DSPMLib
INCLUDEPATH += $$PWD/../DSPMLib/Tools
DEPENDPATH += $$PWD/../DSPMLib/Tools
INCLUDEPATH += $$PWD/../MainWindow
DEPENDPATH += $$PWD/../MainWindow
INCLUDEPATH += $$PWD/../DialogBox
DEPENDPATH += $$PWD/../DialogBox
INCLUDEPATH += $$PWD/../Intefaces
DEPENDPATH += $$PWD/../Intefaces

INCLUDEPATH += $$PWD/../Tools
DEPENDPATH += $$PWD/../Tools

NAME_FILE = $${TARGET}.json

unix:BILD_DIR = $${files(/home/*)}/UWB/BlocksLib
#Winddows
win32:BILD_DIR = $$PWD/../../UWB/BlocksLib

IO_PATH = $${BILD_DIR}/IOBlocks
SINK_PATH = $${BILD_DIR}/SinkBlocks
SOURCE_PATH = $${BILD_DIR}/SourceBlocks

SOURCES += \
    $$PWD/../DSPMLib/abstractmodule.cpp \
    $$PWD/../DSPMLib/csignal.cpp \
    $$PWD/../DialogBox/customdialwidget.cpp \
    $$PWD/../MainWindow/abstractblock.cpp \
    $$PWD/../MainWindow/ioblock.cpp \
    $$PWD/../MainWindow/sourceblock.cpp \
    $$PWD/../MainWindow/sinkblock.cpp \
    $$PWD/../MainWindow/abstractdialogcommand.cpp \
    $$PWD/../Tools/timerevent.cpp \
    $$PWD/../MainWindow/lineconnect.cpp


HEADERS += \
    $$PWD/../DSPMLib/abstractmodule.h \
    $$PWD/../DSPMLib/iomodule.h \
    $$PWD/../DSPMLib/sinkmodule.h \
    $$PWD/../DSPMLib/sourcemodule.h \
    $$PWD/../DSPMLib/csignal.h \
    $$PWD/../Intefaces/bilderblocks.h \
    $$PWD/../Intefaces/abstractoperationmanager.h \
    $$PWD/../Intefaces/changeevent.h \
    $$PWD/../Intefaces/dialogwindow.h \
    $$PWD/../Intefaces/moduleconnectports.h \
    $$PWD/../DialogBox/customdialwidget.h \
    $$PWD/../MainWindow/abstractblock.h \
    $$PWD/../MainWindow/ioblock.h \
    $$PWD/../MainWindow/sinkblock.h \
    $$PWD/../MainWindow/sourceblock.h \
    $$PWD/../MainWindow/abstractdialogcommand.h \
    $$PWD/../Tools/timerevent.h \
    $$PWD/../MainWindow/lineconnect.h \
    $$PWD/../DSPMLib/portsconnect.h \
    $$PWD/../DSPMLib/datavrli.h \
    $$PWD/../DSPMLib/typesignal.h

#windows
win32:INCLUDEPATH += C:/LIBS/fftw-3.3.5-dll64
win32:DEPENDPATH += C:/LIBS/fftw-3.3.5-dll64
win32:LIBS += -LC:/LIBS/fftw-3.3.5-dll64 libfftw3-3

win32:LIBS += -lsetupapi
win32:LIBS += -lwsock32
win32:LIBS += -lws2_32
win32:DEFINES += _WIN32_WINNT=0x0501

win32:INCLUDEPATH += C:/LIBS/tbb/include
win32:DEPENDPATH += C:/LIBS/tbb/include

win32:LIBS += -LC:/LIBS/tbb/build/windows_intel64_gcc_mingw7.3.0_release -ltbb

win32:INCLUDEPATH += C:/LIBS/boost_1_70_0
win32:DEPENDPATH += C:/LIBS/boost_1_70_0
win32:LIBS += -LC:/LIBS/boost_1_70_0/stage/lib -lboost_system-mgw73-mt-x64-1_70
win32:LIBS += -LC:/LIBS/boost_1_70_0/stage/lib -lboost_exception-mgw73-mt-x64-1_70
win32:LIBS += -LC:/LIBS/boost_1_70_0/stage/lib -lboost_thread-mgw73-mt-x64-1_70
win32:LIBS += -LC:/LIBS/boost_1_70_0/stage/lib -lboost_log-mgw73-mt-x64-1_70
