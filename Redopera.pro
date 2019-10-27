TEMPLATE = app
CONFIG += c++11
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$PWD/../Redopera/Redlib

CONFIG(debug, debug|release){
    DEFINES += R_DEBUG
} else {
}

CONFIG(debug, debug|release)：DEFINES += R_DEBUG

SOURCES += \
        Redlib/RController.cpp \
        Redlib/RDebug.cpp \
        Redlib/RInputEvent.cpp \
        Redlib/RNotifyEvent.cpp \
        main.cpp

HEADERS += \
    Redlib/Extern/sigslot.h \
    Redlib/RController.h \
    Redlib/RDebug.h \
    Redlib/RInputEvent.h \
    Redlib/RNotifyEvent.h

DISTFILES += \
    StackList.txt
