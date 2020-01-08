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
    DESTDIR = $$PWD/bin_release
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
        Redlib/Extern/Lua/lapi.c \
        Redlib/Extern/Lua/lauxlib.c \
        Redlib/Extern/Lua/lbaselib.c \
        Redlib/Extern/Lua/lcode.c \
        Redlib/Extern/Lua/lcorolib.c \
        Redlib/Extern/Lua/lctype.c \
        Redlib/Extern/Lua/ldblib.c \
        Redlib/Extern/Lua/ldebug.c \
        Redlib/Extern/Lua/ldo.c \
        Redlib/Extern/Lua/ldump.c \
        Redlib/Extern/Lua/lfunc.c \
        Redlib/Extern/Lua/lgc.c \
        Redlib/Extern/Lua/linit.c \
        Redlib/Extern/Lua/liolib.c \
        Redlib/Extern/Lua/llex.c \
        Redlib/Extern/Lua/lmathlib.c \
        Redlib/Extern/Lua/lmem.c \
        Redlib/Extern/Lua/loadlib.c \
        Redlib/Extern/Lua/lobject.c \
        Redlib/Extern/Lua/lopcodes.c \
        Redlib/Extern/Lua/loslib.c \
        Redlib/Extern/Lua/lparser.c \
        Redlib/Extern/Lua/lstate.c \
        Redlib/Extern/Lua/lstring.c \
        Redlib/Extern/Lua/lstrlib.c \
        Redlib/Extern/Lua/ltable.c \
        Redlib/Extern/Lua/ltablib.c \
        Redlib/Extern/Lua/ltests.c \
        Redlib/Extern/Lua/ltm.c \
        Redlib/Extern/Lua/lua.c \
        Redlib/Extern/Lua/lundump.c \
        Redlib/Extern/Lua/lutf8lib.c \
        Redlib/Extern/Lua/lvm.c \
        Redlib/Extern/Lua/lzio.c \
        Redlib/Extern/Lua/onelua.c \
        Redlib/Extern/RtAudio.cpp \
        Redlib/Extern/stb_truetype.cpp \
        Redlib/RAudioStream.cpp \
        Redlib/RInputModule.cpp \
        Redlib/RPackFile.cpp \
        Redlib/RResource/RCursor.cpp \
        Redlib/RResource/RLuaScript.cpp \
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
        WeiqiGoban.cpp \
        main.cpp

HEADERS += \
    Learn.h \
    Redlib/Extern/Lua/lapi.h \
    Redlib/Extern/Lua/lauxlib.h \
    Redlib/Extern/Lua/lcode.h \
    Redlib/Extern/Lua/lctype.h \
    Redlib/Extern/Lua/ldebug.h \
    Redlib/Extern/Lua/ldo.h \
    Redlib/Extern/Lua/lfunc.h \
    Redlib/Extern/Lua/lgc.h \
    Redlib/Extern/Lua/ljumptab.h \
    Redlib/Extern/Lua/llex.h \
    Redlib/Extern/Lua/llimits.h \
    Redlib/Extern/Lua/lmem.h \
    Redlib/Extern/Lua/lobject.h \
    Redlib/Extern/Lua/lopcodes.h \
    Redlib/Extern/Lua/lopnames.h \
    Redlib/Extern/Lua/lparser.h \
    Redlib/Extern/Lua/lprefix.h \
    Redlib/Extern/Lua/lstate.h \
    Redlib/Extern/Lua/lstring.h \
    Redlib/Extern/Lua/ltable.h \
    Redlib/Extern/Lua/ltests.h \
    Redlib/Extern/Lua/ltm.h \
    Redlib/Extern/Lua/lua.h \
    Redlib/Extern/Lua/luaconf.h \
    Redlib/Extern/Lua/lualib.h \
    Redlib/Extern/Lua/lundump.h \
    Redlib/Extern/Lua/lvm.h \
    Redlib/Extern/Lua/lzio.h \
    Redlib/Extern/RtAudio.h \
    Redlib/Extern/json.hpp \
    Redlib/Extern/minimp3.h \
    Redlib/Extern/minimp3_ex.h \
    Redlib/Extern/stb_truetype.h \
    Redlib/RAudioStream.h \
    Redlib/RInputModule.h \
    Redlib/RJson.h \
    Redlib/RPackFile.h \
    Redlib/RRect.h \
    Redlib/RResource/RCursor.h \
    Redlib/RResource/RLuaScript.h \
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
    WeiqiGoban.h

DISTFILES += \
    Resource/shader/Fragment.frag \
    Resource/shader/LineBox.frag \
    Resource/shader/LineBox.geom \
    Resource/shader/LineBox.vert \
    Resource/shader/Vertex.vert \
    Test.lua \
    config.json
