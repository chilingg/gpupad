#include "EditorManager.h"
#include "SourceEditor.h"
#include "GlslHighlighter.h"
#include "JsHighlighter.h"
#include "BinaryEditor.h"
#include "ImageEditor.h"
#include "Singletons.h"
#include "FileDialog.h"
#include <functional>
#include <QDockWidget>
#include <QTimer>
#include <QMessageBox>
#include <QAction>
#include <QApplication>

namespace {
    void raiseEditor(QWidget *editor)
    {
        if (!editor)
            return;
        QTimer::singleShot(0,
            [editor]() {
                auto dock = qobject_cast<QDockWidget*>(editor->parentWidget());
                if (dock)
                    dock->raise();
                editor->setFocus();
            });
    }
} // namespace

EditorManager::EditorManager(QWidget *parent)
    : DockWindow(parent)
{
    setWindowFlags(Qt::Widget);
    setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::North);
    setDockOptions(AnimatedDocks | AllowNestedDocks |
        AllowTabbedDocks | GroupedDragging);
    setDocumentMode(true);
    setContentsMargins(0, 1, 0, 0);
}

EditorManager::~EditorManager() = default;

int EditorManager::openNotSavedDialog(const QString& fileName)
{
    QMessageBox dialog(this);
    dialog.setIcon(QMessageBox::Question);
    dialog.setWindowTitle(tr("Question"));
    dialog.setText(tr("<h3>The file '%1' is not saved.</h3>"
           "Do you want to save it before closing?<br>")
           .arg(FileDialog::getFileTitle(fileName)));
    dialog.addButton(QMessageBox::Save);
    dialog.addButton(tr("&Don't Save"), QMessageBox::RejectRole);
    dialog.addButton(QMessageBox::Cancel);
    dialog.setDefaultButton(QMessageBox::Save);
    return dialog.exec();
}

bool EditorManager::updateCurrentEditor()
{
    mCurrentDock = nullptr;

    auto focus = qApp->focusWidget();
    foreach (QDockWidget* dock, mDocks.keys()) {
        if (dock->isAncestorOf(focus)) {
            mCurrentDock = dock;
            break;
        }
    }

    if (mCurrentDock != mPrevCurrentDock) {
        mPrevCurrentDock = mCurrentDock;
        return true;
    }
    return false;
}

QString EditorManager::currentEditorFileName()
{
    if (auto editor = currentEditor())
        return editor->fileName();
    return "";
}

IEditor *EditorManager::currentEditor()
{
    if (mCurrentDock)
        return mDocks[mCurrentDock];
    return nullptr;
}

QList<QMetaObject::Connection> EditorManager::connectEditActions(
    const EditActions &actions)
{
    if (auto editor = currentEditor())
        return editor->connectEditActions(actions);
    return { };
}

QString EditorManager::openNewSourceEditor()
{
    auto fileName = FileDialog::generateNextUntitledFileName();
    auto editor = new SourceEditor(fileName);
    addSourceEditor(editor);
    raiseEditor(editor);
    return fileName;
}

QString EditorManager::openNewBinaryEditor()
{
    auto fileName = FileDialog::generateNextUntitledFileName();
    auto editor = new BinaryEditor(fileName);
    addBinaryEditor(editor);
    raiseEditor(editor);
    return fileName;
}

QString EditorManager::openNewImageEditor()
{
    auto fileName = FileDialog::generateNextUntitledFileName();
    auto editor = new ImageEditor(fileName);
    addImageEditor(editor);
    raiseEditor(editor);
    return fileName;
}

bool EditorManager::openEditor(const QString &fileName, bool raise)
{
    if (FileDialog::isBinaryFileName(fileName))
        if (openBinaryEditor(fileName, raise))
            return true;

    if (openSourceEditor(fileName, raise))
        return true;
    if (openImageEditor(fileName, raise))
        return true;
    if (openBinaryEditor(fileName, raise))
        return true;

    return false;
}

SourceEditor *EditorManager::openSourceEditor(const QString &fileName, bool raise,
    int line, int column)
{
    auto editor = getSourceEditor(fileName);
    if (!editor) {
        editor = new SourceEditor(fileName);
        if (!editor->load()) {
            delete editor;
            return nullptr;
        }
        addSourceEditor(editor);
    }
    if (raise)
        raiseEditor(editor);
    if (editor)
        editor->setCursorPosition(line, column);
    return editor;
}

BinaryEditor *EditorManager::openBinaryEditor(const QString &fileName, bool raise)
{
    auto editor = getBinaryEditor(fileName);
    if (!editor) {
        editor = new BinaryEditor(fileName);
        if (!editor->load()) {
            delete editor;
            return nullptr;
        }
        addBinaryEditor(editor);
    }
    if (raise)
        raiseEditor(editor);
    return editor;
}

ImageEditor *EditorManager::openImageEditor(const QString &fileName, bool raise)
{
    auto editor = getImageEditor(fileName);
    if (!editor) {
        editor = new ImageEditor(fileName);
        if (!editor->load()) {
            delete editor;
            return nullptr;
        }
        addImageEditor(editor);
    }
    if (raise)
        raiseEditor(editor);
    return editor;
}

SourceEditor* EditorManager::getSourceEditor(const QString &fileName)
{
    foreach (SourceEditor *editor, mSourceEditors)
        if (editor->fileName() == fileName)
            return editor;
    return nullptr;
}

BinaryEditor* EditorManager::getBinaryEditor(const QString &fileName)
{
    foreach (BinaryEditor *editor, mBinaryEditors)
        if (editor->fileName() == fileName)
            return editor;
    return nullptr;
}

ImageEditor* EditorManager::getImageEditor(const QString &fileName)
{
    foreach (ImageEditor *editor, mImageEditors)
        if (editor->fileName() == fileName)
            return editor;
    return nullptr;
}

QStringList EditorManager::getSourceFileNames() const
{
    auto result = QStringList();
    foreach (SourceEditor *editor, mSourceEditors)
        result.append(editor->fileName());
    return result;
}

QStringList EditorManager::getBinaryFileNames() const
{
    auto result = QStringList();
    foreach (BinaryEditor *editor, mBinaryEditors)
        result.append(editor->fileName());
    return result;
}

QStringList EditorManager::getImageFileNames() const
{
    auto result = QStringList();
    foreach (ImageEditor *editor, mImageEditors)
        result.append(editor->fileName());
    return result;
}

bool EditorManager::saveEditor()
{
    if (auto editor = currentEditor()) {
        if (FileDialog::isUntitled(editor->fileName()))
            return saveEditorAs();

        return editor->save();
    }
    return false;
}

bool EditorManager::saveEditorAs()
{
    if (!mCurrentDock)
        return false;

    if (auto editor = currentEditor()) {
        auto options = FileDialog::Options{ FileDialog::Saving };
        if (qobject_cast<SourceEditor*>(mCurrentDock->widget()))
            options |= FileDialog::ShaderExtensions;
        else if (qobject_cast<BinaryEditor*>(mCurrentDock->widget()))
            options |= FileDialog::BinaryExtensions;
        else if (qobject_cast<ImageEditor*>(mCurrentDock->widget()))
            options |= FileDialog::ImageExtensions;

        if (Singletons::fileDialog().exec(options, editor->fileName())) {
            auto prevFileName = editor->fileName();
            editor->setFileName(Singletons::fileDialog().fileName());
            emit editorRenamed(prevFileName, editor->fileName());
            return saveEditor();
        }
    }
    return false;
}

bool EditorManager::saveAllEditors()
{
    foreach (QDockWidget *dock, mDocks.keys())
        if (dock->isWindowModified())
            if (!saveDock(dock))
                return false;
    return true;
}

bool EditorManager::reloadEditor()
{
    if (auto editor = currentEditor()) {
        if (FileDialog::isUntitled(editor->fileName()))
            return false;

        return editor->load();
    }
    return false;
}

bool EditorManager::closeEditor()
{
    if (mCurrentDock)
        return closeDock(mCurrentDock);
    return false;
}

bool EditorManager::closeAllEditors()
{
    foreach (QDockWidget* dock, mDocks.keys())
        if (!closeDock(dock))
            return false;
    return true;
}

void EditorManager::addSourceEditor(SourceEditor *editor)
{
    // TODO
    //auto highlighter = new JsHighlighter(editor);
    auto highlighter = new GlslHighlighter(editor);
    editor->setHighlighter(highlighter);
    editor->setCompleter(highlighter->completer());
    mSourceEditors.append(editor);

    auto dock = createDock(editor, editor);
    connect(editor, &SourceEditor::modificationChanged,
        dock, &QDockWidget::setWindowModified);
    connect(editor, &SourceEditor::fileNameChanged,
        [dock](const QString &fileName) {
            dock->setWindowTitle(FileDialog::getWindowTitle(fileName));
        });
    connect(editor, &SourceEditor::textChanged,
        [this, editor]() {
            emit sourceEditorChanged(editor->fileName());
        });
}

void EditorManager::addImageEditor(ImageEditor *editor)
{
    mImageEditors.append(editor);

    auto dock = createDock(editor, editor);
    connect(editor, &ImageEditor::modificationChanged,
        dock, &QDockWidget::setWindowModified);
    connect(editor, &ImageEditor::fileNameChanged,
        [dock](const QString &fileName) {
            dock->setWindowTitle(FileDialog::getWindowTitle(fileName));
        });
    connect(editor, &ImageEditor::dataChanged,
        [this, editor]() {
            emit imageEditorChanged(editor->fileName());
        });
}

void EditorManager::addBinaryEditor(BinaryEditor *editor)
{
    mBinaryEditors.append(editor);

    auto dock = createDock(editor, editor);
    connect(editor, &BinaryEditor::modificationChanged,
        dock, &QDockWidget::setWindowModified);
    connect(editor, &BinaryEditor::fileNameChanged,
        [dock](const QString &fileName) {
            dock->setWindowTitle(FileDialog::getWindowTitle(fileName));
        });
    connect(editor, &BinaryEditor::dataChanged,
        [this, editor]() {
            emit binaryEditorChanged(editor->fileName());
        });
}

QDockWidget *EditorManager::createDock(QWidget *widget, IEditor *editor)
{
    auto fileName = editor->fileName();
    auto dock = new QDockWidget(FileDialog::getWindowTitle(fileName), this);
    dock->setWidget(widget);

    dock->setFeatures(QDockWidget::DockWidgetMovable |
        QDockWidget::DockWidgetClosable | 
        QDockWidget::DockWidgetFloatable);

    dock->toggleViewAction()->setVisible(false);
    dock->installEventFilter(this);

    if (!mDocks.isEmpty())
        tabifyDockWidget(mDocks.firstKey(), dock);
    else
        addDockWidget(Qt::LeftDockWidgetArea, dock);

    mDocks.insert(dock, editor);
    return dock;
}

bool EditorManager::saveDock(QDockWidget *dock)
{
    auto currentDock = mCurrentDock;
    mCurrentDock = dock;
    auto result = saveEditor();
    mCurrentDock = currentDock;
    return result;
}

bool EditorManager::closeDock(QDockWidget *dock)
{
    auto editor = mDocks[dock];

    if (dock->isWindowModified()) {
        auto ret = openNotSavedDialog(editor->fileName());
        if (ret == QMessageBox::Cancel)
            return false;

        if (ret == QMessageBox::Save &&
            !saveDock(dock))
            return false;
    }

    emit editorRenamed(editor->fileName(), "");

    mSourceEditors.removeAll(static_cast<SourceEditor*>(editor));
    mBinaryEditors.removeAll(static_cast<BinaryEditor*>(editor));
    mImageEditors.removeAll(static_cast<ImageEditor*>(editor));

    mDocks.remove(dock);

    if (mCurrentDock == dock) {
        mCurrentDock = nullptr;
        if (!mDocks.isEmpty())
            raiseEditor(mDocks.lastKey()->widget());
    }

    DockWindow::closeDock(dock);

    if (mDocks.isEmpty()) {
        Singletons::fileDialog().resetNextUntitledFileIndex();
        openNewSourceEditor();
    }
    return true;
}