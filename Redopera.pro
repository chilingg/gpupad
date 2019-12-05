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

unix:!macx:{
LIBS += -lglfw \
    -ldl \
    -L/usr/lib/ -lfreetype

INCLUDEPATH += /usr/include/freetype2
}

SOURCES += \
        Learn.cpp \
        Redlib/RLayout.cpp \
        Redlib/RSprite.cpp \
        Redlib/Extern/SourceCodePro.cpp \
        Redlib/Extern/glad.c \
        Redlib/Extern/stb_image_implementatio.cpp \
        Redlib/RController.cpp \
        Redlib/RDebug.cpp \
        Redlib/RInputEvent.cpp \
        Redlib/RPlane.cpp \
        Redlib/RResource/RFont.cpp \
        Redlib/RResource/RImage.cpp \
        Redlib/RResource/RResource.cpp \
        Redlib/RResource/RShader.cpp \
        Redlib/RResource/RShaderProgram.cpp \
        Redlib/RResource/RTexture.cpp \
        Redlib/RTextPlane.cpp \
        Redlib/RWindowCtrl.cpp \
        Testctr.cpp \
        main.cpp

HEADERS += \
    Learn.h \
    Redlib/RLayout.h \
    Redlib/RSprite.h \
    Redlib/Extern/sigslot.h \
    Redlib/Extern/stb_image.h \
    Redlib/RColor.h \
    Redlib/RController.h \
    Redlib/RDebug.h \
    Redlib/RInputEvent.h \
    Redlib/RMaths.h \
    Redlib/RNotifyEvent.h \
    Redlib/ROpenGL.h \
    Redlib/RPlane.h \
    Redlib/RPoint.h \
    Redlib/RResource/RFont.h \
    Redlib/RResource/RImage.h \
    Redlib/RResource/RResource.h \
    Redlib/RResource/RShader.h \
    Redlib/RResource/RShaderProgram.h \
    Redlib/RResource/RTexture.h \
    Redlib/RSize.h \
    Redlib/RTextPlane.h \
    Redlib/RTimer.h \
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
