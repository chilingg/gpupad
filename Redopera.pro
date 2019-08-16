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
        RController.cpp \
        RDebug.cpp \
        RImage.cpp \
        RResource.cpp \
        RShader.cpp \
        RShaderProgram.cpp \
        RTexture.cpp \
        RWindow.cpp \
        glad.c \
        main.cpp \
        stb_image.cpp

HEADERS += \
    RController.h \
    RDebug.h \
    RImage.h \
    RJoystickEvent.h \
    RKeyEvent.h \
    RMouseEvent.h \
    RResource.h \
    RShader.h \
    RShaderProgram.h \
    RTexture.h \
    RWheelEvent.h \
    RWindow.h \
    stb_image.h
