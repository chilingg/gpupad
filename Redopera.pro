TEMPLATE = app
CONFIG += c++11
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$PWD/../Redopera/Redlib

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/bin_debug
} else {
    DEFINES += R_NO_DEBUG
    DESTDIR = bin_release
}

unix{
unix:!macx:
LIBS += -lglfw \
    -ldl \
}

SOURCES += \
        Redlib/Extern/glad.c \
        Redlib/RController.cpp \
        Redlib/RDebug.cpp \
        Redlib/RInputEvent.cpp \
        Redlib/RPoint.cpp \
        Redlib/RResource/RResource.cpp \
        Redlib/RWindowCtrl.cpp \
        Redlib/Redopera.cpp \
        Testctr.cpp \
        main.cpp

HEADERS += \
    Redlib/Extern/sigslot.h \
    Redlib/RController.h \
    Redlib/RDebug.h \
    Redlib/RInputEvent.h \
    Redlib/RNotifyEvent.h \
    Redlib/ROpenGL.h \
    Redlib/RPoint.h \
    Redlib/RResource/RResource.h \
    Redlib/RWindowCtrl.h \
    Redlib/Redopera.h \
    Testctr.h

DISTFILES += \ \
    Resource/StackList.txt
