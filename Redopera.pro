TEMPLATE = app
CONFIG += c++11
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt

DEPENDENT_PATH = $$PWD/../bin
R_DEPENDENT_PATH = $$PWD/../Redopera/Redlib

win32{
LIBS += -L$$DEPENDENT_PATH/libs/ -lglfw3 \

INCLUDEPATH += $$DEPENDENT_PATH/include
DEPENDPATH += $$DEPENDENT_PATH/include
}

INCLUDEPATH += $$R_DEPENDENT_PATH
DEPENDPATH += $$R_DEPENDENT_PATH

unix{
unix:!macx:
LIBS += -lglfw \
    -ldl \
}

SOURCES += \
        Redlib/RController.cpp \
        Redlib/RDebug.cpp \
        Redlib/RImage.cpp \
        Redlib/RResource.cpp \
        Redlib/RShader.cpp \
        Redlib/RShaderProgram.cpp \
        Redlib/RTexture.cpp \
        Redlib/RWindow.cpp \
        Redlib/glad.c \
        Redlib/stb_image.cpp \
        main.cpp \
    testclass.cpp \
    testctrl.cpp

HEADERS += \
    Redlib/RController.h \
    Redlib/RDebug.h \
    Redlib/RImage.h \
    Redlib/RJoystickEvent.h \
    Redlib/RKeyEvent.h \
    Redlib/RMouseEvent.h \
    Redlib/RResizeEvent.h \
    Redlib/RResource.h \
    Redlib/RShader.h \
    Redlib/RShaderProgram.h \
    Redlib/RTexture.h \
    Redlib/RWheelEvent.h \
    Redlib/RWindow.h \
    Redlib/stb_image.h \
    constant.h \
    testclass.h \
    testctrl.h

DISTFILES += \
    shaders/fragment.frag \
    shaders/vertex.vert
