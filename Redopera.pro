TEMPLATE = app
CONFIG += c++11
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt

R_DEPENDENT_PATH = $$PWD/../bin

win32{
LIBS += -L$$R_DEPENDENT_PATH/libs/ -lglfw3 \

INCLUDEPATH += $$R_DEPENDENT_PATH/include
DEPENDPATH += $$R_DEPENDENT_PATH/include
}

unix{
unix:!macx:
LIBS += -lglfw \
    -ldl \
}

SOURCES += \
        glad.c \
        main.cpp \
        rdebug.cpp \
        rmaincontrol.cpp \
        rresource.cpp \
        rshader.cpp \
        rshaderprogram.cpp

HEADERS += \
    rdebug.h \
    rmaincontrol.h \
    rresource.h \
    rshader.h \
    rshaderprogram.h
