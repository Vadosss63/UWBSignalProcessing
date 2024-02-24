win32:INCLUDEPATH += C:/LIBS/boost_1_70_0
win32:DEPENDPATH += C:/LIBS/boost_1_70_0

win32:INCLUDEPATH += C:/LIBS/tbb/include
win32:DEPENDPATH += C:/LIBS/tbb/include

win32:LIBS += -LC:/LIBS/tbb/build/windows_intel64_gcc_mingw7.3.0_release -ltbb

win32:LIBS += -LC:/LIBS/boost_1_70_0/stage/lib -llibboost_system-mgw73-mt-x64-1_70

win32:LIBS += -lsetupapi
win32:LIBS += -lwsock32
win32:LIBS += -lws2_32
win32:DEFINES += _WIN32_WINNT=0x0501
