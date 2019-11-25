TEMPLATE = app
CONFIG += c++11
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$PWD/../Redopera/Redlib

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/bin_debug
    DEFINES += R_DEBUG
} else {
    DESTDIR = bin_release
}

unix{
unix:!macx:
LIBS += -lglfw \
    -ldl \
}

SOURCES += \
        Redlib/Extern/glad.c \
        Redlib/Extern/stb_image_implementatio.cpp \
        Redlib/RController.cpp \
        Redlib/RDebug.cpp \
        Redlib/RInputEvent.cpp \
        Redlib/RPlant.cpp \
        Redlib/RResource/RImage.cpp \
        Redlib/RResource/RResource.cpp \
        Redlib/RResource/RShader.cpp \
        Redlib/RResource/RShaderProgram.cpp \
        Redlib/RResource/RTexture.cpp \
        Redlib/RWindowCtrl.cpp \
        Testctr.cpp \
        main.cpp

HEADERS += \
    Redlib/Extern/sigslot.h \
    Redlib/Extern/stb_image.h \
    Redlib/RColor.h \
    Redlib/RController.h \
    Redlib/RDebug.h \
    Redlib/RInputEvent.h \
    Redlib/RMaths.h \
    Redlib/RNotifyEvent.h \
    Redlib/ROpenGL.h \
    Redlib/RPlant.h \
    Redlib/RPoint.h \
    Redlib/RResource/RImage.h \
    Redlib/RResource/RResource.h \
    Redlib/RResource/RShader.h \
    Redlib/RResource/RShaderProgram.h \
    Redlib/RResource/RTexture.h \
    Redlib/RSize.h \
    Redlib/RWindowCtrl.h \
    Redlib/RSigslot.h \
    Testctr.h

DISTFILES += \
    Resource/StackList.txt \
    Resource/shader/Fragment.frag \
    Resource/shader/LineBox.frag \
    Resource/shader/LineBox.geom \
    Resource/shader/LineBox.vert \
    Resource/shader/Vertex.vert
