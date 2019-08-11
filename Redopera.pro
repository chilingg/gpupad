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
        RDebug.cpp \
        RImage.cpp \
        RMainControl.cpp \
        RResource.cpp \
        RShader.cpp \
        RShaderProgram.cpp \
        RTexture.cpp \
        glad.c \
        main.cpp \
        stb_image.cpp

HEADERS += \
    RDebug.h \
    RImage.h \
    RJoystickEvent.h \
    RKeyEvent.h \
    RMainControl.h \
    RMouseEvent.h \
    RResource.h \
    RShader.h \
    RShaderProgram.h \
    RTexture.h \
    RWheelEvent.h \
    stb_image.h
