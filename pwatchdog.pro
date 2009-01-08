TEMPLATE = app
TARGET = pwatchdog
QT += core \
    gui
HEADERS += ProcessHandler.h \
    AbstractProcessView.h \
    PWatchdogEvents.h \
    View.h \
    Model.h \
    ProcessWatchDog.h
SOURCES += PWatchdogEvents.cpp \
    ProcessWatchDog.cpp \
    main.cpp \
    Model.cpp \
    View.cpp
FORMS += pwatchdog.ui
RESOURCES += pwatchdog.qrc
CONFIG += thread
win32:SOURCES += Win32ProcessHandler.cpp
unix:SOURCES += UnixProcessHandler.cpp
