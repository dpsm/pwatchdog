TEMPLATE = app
TARGET = pwatchdog
QT += core \
    gui
HEADERS += MainWindow.h \
    AbstractProcessView.h \
    PWatchdogEvents.h \
    Model.h
SOURCES += MainWindow.cpp \
    PWatchdogEvents.cpp \
    ProcessWatchDog.cpp \
    main.cpp \
    Model.cpp
FORMS += pwatchdog.ui
RESOURCES += pwatchdog.qrc
CONFIG += thread

# CONFIG += static
win32 { 
    SOURCES += Win32PowerManager.cpp \
        Win32Utils.cpp
    HEADERS += Win32PowerManager.h
    RC_FILE = pwatchdog.rc
    LIBS += -lPsapi
}
unix:SOURCES += UnixUtils.cpp
