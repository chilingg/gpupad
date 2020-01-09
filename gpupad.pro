QT += core gui network qml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17
TEMPLATE = app
TARGET = GPUpad

DESTDIR = $$PWD/../bin

INCLUDEPATH += $$PWD/libs
INCLUDEPATH += $$PWD/src

DEFINES += QAPPLICATION_CLASS=QApplication

win32 {
LIBS += -ldwmapi \
}

unix: {
!macx:
LIBS += -L/usr/lib/ -ldrm
INCLUDEPATH += /usr/include/libdrm
}

CONFIG(debug, debug|release){
    DEFINES += _DEBUG
} else {
}

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    libs/SingleApplication/singleapplication.cpp \
    src/FileCache.cpp \
    src/FileDialog.cpp \
    src/MainWindow.cpp \
    src/MessageList.cpp \
    src/MessageWindow.cpp \
    src/OutputWindow.cpp \
    src/Settings.cpp \
    src/Singletons.cpp \
    src/SynchronizeLogic.cpp \
    src/editors/BinaryEditor.cpp \
    src/editors/DockWindow.cpp \
    src/editors/EditorManager.cpp \
    src/editors/FindReplaceBar.cpp \
    src/editors/GlslHighlighter.cpp \
    src/editors/ImageEditor.cpp \
    src/editors/JsHighlighter.cpp \
    src/editors/SourceEditor.cpp \
    src/main.cpp  \
    src/render/CompositorSync.cpp \
    src/render/GLBuffer.cpp \
    src/render/GLCall.cpp \
    src/render/GLProgram.cpp \
    src/render/GLShader.cpp \
    src/render/GLStream.cpp \
    src/render/GLTarget.cpp \
    src/render/GLTexture.cpp \
    src/render/ProcessSource.cpp \
    src/render/RenderSession.cpp \
    src/render/RenderTask.cpp \
    src/render/Renderer.cpp \
    src/scripting/CustomActions.cpp \
    src/scripting/GpupadScriptObject.cpp \
    src/scripting/ScriptEngine.cpp \
    src/session/AttachmentProperties.cpp \
    src/session/BindingProperties.cpp \
    src/session/CallProperties.cpp \
    src/session/ColorPicker.cpp \
    src/session/DataComboBox.cpp \
    src/session/ExpressionEditor.cpp \
    src/session/ExpressionLineEdit.cpp \
    src/session/ExpressionMatrix.cpp \
    src/session/ReferenceComboBox.cpp \
    src/session/SessionEditor.cpp \
    src/session/SessionModel.cpp \
    src/session/SessionModelCore.cpp \
    src/session/SessionProperties.cpp \
    src/session/TextureProperties.cpp

HEADERS +=  \
    libs/SingleApplication/singleapplication.h \
    libs/SingleApplication/singleapplication_p.h \
    src/AutoOrientationSplitter.h \
    src/EditActions.h \
    src/FileCache.h \
    src/FileDialog.h \
    src/MainWindow.h \
    src/MessageList.h \
    src/MessageWindow.h \
    src/OutputWindow.h \
    src/Settings.h \
    src/Singletons.h \
    src/SourceType.h \
    src/SynchronizeLogic.h \
    src/_version.h \
    src/editors/BinaryEditor.h \
    src/editors/BinaryEditor_DataModel.h \
    src/editors/BinaryEditor_EditableRegion.h \
    src/editors/BinaryEditor_HexModel.h \
    src/editors/BinaryEditor_SpinBoxDelegate.h \
    src/editors/DockWindow.h \
    src/editors/EditorManager.h \
    src/editors/FindReplaceBar.h \
    src/editors/GlslHighlighter.h \
    src/editors/IEditor.h \
    src/editors/ImageEditor.h \
    src/editors/JsHighlighter.h \
    src/editors/SourceEditor.h \
    src/render/CompositorSync.h \
    src/render/GLBuffer.h \
    src/render/GLCall.h \
    src/render/GLContext.h \
    src/render/GLItem.h \
    src/render/GLObject.h \
    src/render/GLProgram.h \
    src/render/GLShader.h \
    src/render/GLShareSynchronizer.h \
    src/render/GLStream.h \
    src/render/GLTarget.h \
    src/render/GLTexture.h \
    src/render/ProcessSource.h \
    src/render/RenderSession.h \
    src/render/RenderTask.h \
    src/render/Renderer.h \
    src/scripting/CustomActions.h \
    src/scripting/GpupadScriptObject.h \
    src/scripting/ScriptEngine.h \
    src/session/AttachmentProperties.h \
    src/session/BindingProperties.h \
    src/session/CallProperties.h \
    src/session/ColorPicker.h \
    src/session/DataComboBox.h \
    src/session/ExpressionEditor.h \
    src/session/ExpressionLineEdit.h \
    src/session/ExpressionMatrix.h \
    src/session/Item.h \
    src/session/ItemEnums.h \
    src/session/ItemFunctions.h \
    src/session/ReferenceComboBox.h \
    src/session/SessionEditor.h \
    src/session/SessionModel.h \
    src/session/SessionModelCore.h \
    src/session/SessionModelPriv.h \
    src/session/SessionProperties.h \
    src/session/TextureProperties.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src/resources.qrc

FORMS += \
    src/MainWindow.ui \
    src/editors/FindReplaceBar.ui \
    src/scripting/CustomActions.ui \
    src/session/AttachmentProperties.ui \
    src/session/AttributeProperties.ui \
    src/session/BindingProperties.ui \
    src/session/BufferProperties.ui \
    src/session/CallProperties.ui \
    src/session/ColumnProperties.ui \
    src/session/GroupProperties.ui \
    src/session/ImageProperties.ui \
    src/session/ProgramProperties.ui \
    src/session/ScriptProperties.ui \
    src/session/ShaderProperties.ui \
    src/session/StreamProperties.ui \
    src/session/TargetProperties.ui \
    src/session/TextureProperties.ui
