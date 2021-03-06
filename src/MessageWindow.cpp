#include "MessageWindow.h"
#include "Singletons.h"
#include "MessageList.h"
#include "session/SessionModel.h"
#include "FileDialog.h"
#include <QTimer>
#include <QHeaderView>

MessageWindow::MessageWindow(QWidget *parent) : QTableWidget(parent)
{
    connect(this, &MessageWindow::itemActivated,
        this, &MessageWindow::handleItemActivated);

    mUpdateItemsTimer = new QTimer(this);
    mUpdateItemsTimer->setInterval(250);
    connect(mUpdateItemsTimer, &QTimer::timeout,
        this, &MessageWindow::updateMessages);
    mUpdateItemsTimer->start();

    setColumnCount(2);
    verticalHeader()->setVisible(false);
    horizontalHeader()->setVisible(false);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setDefaultSectionSize(24);
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    setEditTriggers(NoEditTriggers);
    setSelectionMode(SingleSelection);
    setSelectionBehavior(SelectRows);
    setVerticalScrollMode(ScrollPerPixel);
    setShowGrid(false);
    setAlternatingRowColors(true);
    setWordWrap(true);

    mInfoIcon.addFile(QStringLiteral(":/images/16x16/dialog-information.png"));
    mWarningIcon.addFile(QStringLiteral(":/images/16x16/dialog-warning.png"));
    mErrorIcon.addFile(QStringLiteral(":/images/16x16/dialog-error.png"));
}

void MessageWindow::updateMessages()
{
    auto added = false;
    auto messages = MessageList::messages();
    auto messageIds = QSet<MessageId>();
    for (auto it = messages.begin(); it != messages.end(); ) {
        const auto& message = **it;
        if (message.type == MessageType::CallDuration)
            if (auto messageId = tryReplaceMessage(message)) {
                messageIds += messageId;
                it = messages.erase(it);
                continue;
            }
        added |= addMessageOnce(message);
        messageIds += message.id;
        ++it;
    }
    removeMessagesExcept(messageIds);

    if (added)
        emit messagesAdded();
}

QIcon MessageWindow::getMessageIcon(const Message &message) const
{
    switch (message.type) {
        case OpenGLVersionNotAvailable:
        case LoadingFileFailed:
        case UnsupportedShaderType:
        case CreatingFramebufferFailed:
        case UploadingImageFailed:
        case DownloadingImageFailed:
        case BlockNotSet:
        case AttributeNotSet:
        case ShaderError:
        case ScriptError:
        case ProgramNotAssigned:
        case TextureNotAssigned:
        case BufferNotAssigned:
        case InvalidSubroutine:
        case ImageFormatNotBindable:
        case UniformComponentMismatch:
            return mErrorIcon;

        case UnformNotSet:
        case ShaderWarning:
            return mWarningIcon;

        case ShaderInfo:
        case ScriptMessage:
        case CallDuration:
            return mInfoIcon;
    }
    return mWarningIcon;
}

QString MessageWindow::getMessageText(const Message &message) const
{
    switch (message.type) {
        case ShaderInfo:
        case ShaderWarning:
        case ShaderError:
        case ScriptError:
        case ScriptMessage:
            return message.text;

        case OpenGLVersionNotAvailable:
            return tr("the required OpenGL version %1 is not available").arg(
                message.text);
        case LoadingFileFailed:
            if (message.text.isEmpty())
                return tr("no file set");
            return tr("loading file '%1' failed").arg(
                FileDialog::getFileTitle(message.text));
        case UnsupportedShaderType:
            return tr("unsupported shader type");
        case CreatingFramebufferFailed:
            return tr("creating framebuffer failed %1").arg(message.text);
        case UploadingImageFailed:
            return tr("uploading image failed");
        case DownloadingImageFailed:
            return tr("downloading image failed");
        case UnformNotSet:
            return tr("uniform '%1' not set").arg(message.text);
        case BlockNotSet:
            return tr("block '%1' not set").arg(message.text);
        case AttributeNotSet:
            return tr("attribute '%1' not set").arg(message.text);
        case CallDuration:
            return tr("call took %1").arg(message.text);
        case ProgramNotAssigned:
            return tr("no program set");
        case TextureNotAssigned:
            return tr("no texture set");
        case BufferNotAssigned:
            return tr("no buffer set");
        case InvalidSubroutine:
            return tr("invalid subroutine '%1'").arg(message.text);
        case ImageFormatNotBindable:
            return tr("image format not bindable");
        case UniformComponentMismatch:
            return tr("uniform component mismatch %1").arg(message.text);
    }
    return message.text;
}

QString MessageWindow::getLocationText(const Message &message) const
{
    auto locationText = QString();
    if (message.itemId) {
        if (auto item = Singletons::sessionModel().findItem(message.itemId)) {
            if (item->parent && !item->parent->name.isEmpty())
                locationText += item->parent->name + " ";
            locationText += item->name;
        }
    }
    else if (!message.fileName.isEmpty()) {
        locationText = FileDialog::getFileTitle(message.fileName);
        if (message.line > 0)
            locationText += ":" + QString::number(message.line);
    }
    return locationText;
}

void MessageWindow::removeMessagesExcept(const QSet<MessageId> &messageIds)
{
    auto removeIds = mMessageIds;
    removeIds.subtract(messageIds);
    if (removeIds.empty())
        return;

    for (auto i = 0; i < rowCount(); ) {
        const auto messageId = item(i, 0)->data(Qt::UserRole + 5).toULongLong();
        if (removeIds.contains(messageId)) {
            removeRow(i);
            mMessageIds.remove(messageId);
        }
        else {
            ++i;
        }
    }
}

auto MessageWindow::tryReplaceMessage(const Message &message) -> MessageId
{
    for (auto i = 0; i < rowCount(); i++) {
        auto &item = *this->item(i, 0);
        if (item.data(Qt::UserRole + 1) == message.itemId &&
            item.data(Qt::UserRole + 4) == message.type) {

            const auto messageId = item.data(Qt::UserRole + 5).toULongLong();
            item.setText(getMessageText(message));
            return messageId;
        }
    }
    return 0;
}

bool MessageWindow::addMessageOnce(const Message &message)
{
    if (mMessageIds.contains(message.id))
        return false;
    mMessageIds.insert(message.id);

    auto messageIcon = getMessageIcon(message);
    auto messageText = getMessageText(message);
    auto messageItem = new QTableWidgetItem(messageIcon, messageText);
    messageItem->setData(Qt::UserRole + 1, message.itemId);
    messageItem->setData(Qt::UserRole + 2, message.fileName);
    messageItem->setData(Qt::UserRole + 3, message.line);
    messageItem->setData(Qt::UserRole + 4, message.type);
    messageItem->setData(Qt::UserRole + 5, message.id);

    auto locationText = getLocationText(message);
    auto locationItem = new QTableWidgetItem(locationText);
    locationItem->setTextAlignment(Qt::AlignLeft | Qt::AlignTop);

    auto row = rowCount();
    insertRow(row);
    setItem(row, 0, messageItem);
    setItem(row, 1, locationItem);
    return true;
}

void MessageWindow::handleItemActivated(QTableWidgetItem *messageItem)
{
    auto itemId = messageItem->data(Qt::UserRole + 1).toInt();
    auto fileName = messageItem->data(Qt::UserRole + 2).toString();
    auto line = messageItem->data(Qt::UserRole + 3).toInt();
    emit messageActivated(itemId, fileName, line, -1);
}
