TEMPLATE = app
TARGET = pwatchdog
QT += core \
    gui
HEADERS += View.h \
    Model.h \
    AbstractProcessView.h \
    ProcessWatchDog.h
SOURCES += main.cpp \
    Model.cpp \
    ProcessWatchDog.cpp \
    View.cpp
FORMS += pwatchdog.ui
RESOURCES += pwatchdog.qrc
