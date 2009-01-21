TEMPLATE = app
TARGET = pwatchdog
QT += core \
    gui
HEADERS += Utils.h \
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
win32 { 
    SOURCES += Win32PowerManager.cpp \
        Win32Utils.cpp
    HEADERS += Win32PowerManager.h
    RC_FILE = pwatchdog.rc
    LIBS += -lPsapi
}
unix:SOURCES += UnixUtils.cpp
