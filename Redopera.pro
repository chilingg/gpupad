TEMPLATE = app
CONFIG += c++17
CONFIG -= console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$PWD/../Redopera/Redlib
INCLUDEPATH += $$PWD/../Redopera/Redlib/Extern

CONFIG(debug, debug|release){
    DESTDIR = $$PWD/bin_debug

    DEFINES += R_DEBUG
    DEFINES += __RTAUDIO_DEBUG__
} else {
    DESTDIR = bin_release
    DEFINES += NDEBUG
}

unix:!macx:{
LIBS += -lglfw \
    -ldl \
    -lfreetype \
    -lpthread

# Specify the API or multiple API to use under Linux:
DEFINES += __LINUX_PULSE__
# DEFINES += __LINUX_ALSA__
# DEFINES += __LINUX_OSS__
# DEFINES += __UNIX_JACK__
contains(DEFINES, __LINUX_PULSE__) {
    LIBS += -lpulse-simple -lpulse
}
contains(DEFINES, __LINUX_ALSA__) {
    LIBS += -lasound
}
contains(DEFINES, __UNIX_JACK__) {
    LIBS += $$system(pkg-config --cflags --libs jack)
}

INCLUDEPATH += /usr/include/freetype2
}

win32:{
# Specify the API or multiple API to use under Windows:
DEFINES += __WINDOWS_ASIO__
# DEFINES += __WINDOWS_DS__
# DEFINES += __WINDOWS_WASAPI__
}

SOURCES += \
        Learn.cpp \
        Redlib/Extern/RtAudio.cpp \
        Redlib/RAudioStream.cpp \
        Redlib/RInputModule.cpp \
        Redlib/RResource/RCursor.cpp \
        Redlib/RResource/RMp3.cpp \
        Redlib/Extern/minimp3_implementation.cpp \
        Redlib/RResourceWindow.cpp \
        Redlib/RSprite.cpp \
        Redlib/Extern/SourceCodePro.cpp \
        Redlib/Extern/glad.c \
        Redlib/Extern/stb_image_implementatio.cpp \
        Redlib/RController.cpp \
        Redlib/RDebug.cpp \
        Redlib/RPlane.cpp \
        Redlib/RResource/RFont.cpp \
        Redlib/RResource/RImage.cpp \
        Redlib/RResource/RResource.cpp \
        Redlib/RResource/RShader.cpp \
        Redlib/RResource/RShaderProgram.cpp \
        Redlib/RResource/RTexture.cpp \
        Redlib/RTextPlane.cpp \
        Redlib/RThreadPool.cpp \
        Redlib/RWindowCtrl.cpp \
        Testctr.cpp \
        Weiqi.cpp \
        WeiqiBoard.cpp \
        main.cpp

HEADERS += \
    Learn.h \
    Redlib/Extern/RtAudio.h \
    Redlib/Extern/minimp3.h \
    Redlib/Extern/minimp3_ex.h \
    Redlib/RAudioStream.h \
    Redlib/RInputModule.h \
    Redlib/RRect.h \
    Redlib/RResource/RCursor.h \
    Redlib/RResource/RMp3.h \
    Redlib/RResourceWindow.h \
    Redlib/RSprite.h \
    Redlib/Extern/sigslot.h \
    Redlib/Extern/stb_image.h \
    Redlib/RColor.h \
    Redlib/RController.h \
    Redlib/RDebug.h \
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
    Redlib/RThread.h \
    Redlib/RThreadPool.h \
    Redlib/RThreadQueue.h \
    Redlib/RThreadStack.h \
    Redlib/RTime.h \
    Redlib/RTimer.h \
    Redlib/RWindowCtrl.h \
    Redlib/RSigslot.h \
    Testctr.h \
    Weiqi.h \
    WeiqiBoard.h

DISTFILES += \
    Resource/StackList.txt \
    Resource/shader/Fragment.frag \
    Resource/shader/LineBox.frag \
    Resource/shader/LineBox.geom \
    Resource/shader/LineBox.vert \
    Resource/shader/Vertex.vert
