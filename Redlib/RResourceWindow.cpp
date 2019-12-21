#include "RResourceWindow.h"

#include "RDebug.h"

constexpr int RCLIST_WIDTH = 180;
constexpr int RCLIST_TITLE_HEIGHT = 26;
constexpr int RCLIST_LEFT_ALIGNMENT = 10;
constexpr int RCSCROLL_BAR_WIDTH = 10;

constexpr int NODE_LABEL_LENGTH = 130;
constexpr int CHILD_NODE_OFFSET = 30;

const RColor BACKGROUND_COLOR(50, 50, 50);
const RColor KEY_COLOR(70, 70, 70);
const RColor BACKGROUND_TOW_COLOR(40, 40, 40);
const RColor CONTENT_COLOR(30, 30, 30);
const RColor FONT_COLOR(220, 220, 220);

RResourceWindow::RResourceWindow(const std::string &name, RController *parent):
    RWindowCtrl(name, parent)
{
    setViewportPattern(RWindowCtrl::FullWindow);
    setWindowMinimumSize(960, 540);
    setBackgroundColor(CONTENT_COLOR);
}

RResourceWindow::~RResourceWindow()
{

}

void RResourceWindow::control()
{
    if(isFocus())
    {
        //更新手柄输入
        RInputModule::instance().updateGamepad();
        //更新键鼠输入
        RInputModule::instance().updateKeyboardInput(getWindowHandle());
        RInputModule::instance().updateMouseInput(getWindowHandle());
        //发布输入事件
        RInputEvent e(this);
        dispatchEvent(&e);
    }

    //清屏 清除颜色缓冲和深度缓冲
    glStencilMask(0xFF); // 启用模板缓冲写入
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    if(timer_.elapsed() > 1.0)
    {
        updateReSourceList();
        timer_.start();
    }

    rcListBackground_.render();
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilMask(0x00); // 禁止模板缓冲的写入
    rcListPlane_.render();
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // 所有的片段都通过

    rcTitle_.render();
    rcScrollBarBack_.render();
    rcScrollBar_.render();
    rcCountPlane_.render();

    ctrlTreeTitle_.render();
    RPoint2 p = nodeLabel_.pos();
    updateControllerTree(getFreeTree(), p);
    nodeLabel_.setPosition(p.x(), p.y());

    //rcListPlane_.renderLineBox(0, width(), 0, height());
    //rcListBackground_.renderLineBox(0, width(), 0, height());

    //调动子结点控制
    allChildrenActive();

    glfwSwapBuffers(getWindowHandle());

    if(glfwWindowShouldClose(getWindowHandle()) || glfwWindowShouldClose(shareContext))
        breakLoop();
}

void RResourceWindow::updateReSourceList()
{
    std::string list;
    rcCount_ = 0;
    for(auto rc : *resourceList)
    {
        if(rc)
        {
            std::string prefix;
            if(rc->resourceID() < 10) prefix = '0';
            list += prefix + std::to_string(rc->resourceID()) + ' ' + rc->name() + '\n';
            ++rcCount_;
        }
    }
    rcListPlane_.setHeight((rcCount_+1) * rcListPlane_.lineHeight());
    rcListPlane_.setTexts(std::wstring(list.begin(), list.end()));
    rcListPlane_.setPosition(0, height() - rcListPlane_.height() - RCLIST_TITLE_HEIGHT);

    rcCountPlane_.setTexts(L"Count: " + std::to_wstring(rcCount_));

    if(rcListBackground_.height() < rcListPlane_.height())
    {
        double ratio = rcListBackground_.height()*1.0 / rcListPlane_.height();
        rcScrollBar_.setHeight(static_cast<int>(rcListBackground_.height()*ratio));
        rcScrollBar_.setPositionY(height() - rcScrollBar_.height() - RCLIST_TITLE_HEIGHT);

        checkScrollBarOffer();
        rcListPlane_.setPositionY(rcListPlane_.y() + rcListPlaneOffsetY_);
        rcScrollBar_.setPositionY(rcScrollBar_.y() - rcScrollBarOffsetY_);
    } else {
        rcScrollBar_.setHeight(rcListBackground_.height());
        rcScrollBar_.setPositionY(RCLIST_TITLE_HEIGHT);
    }
}

int RResourceWindow::updateControllerTree(RController *node, RPoint2 pos)
{
    auto tree = node->getChildren();
    int posOffset = 0;
    for(auto child : tree)
    {
        posOffset += updateControllerTree(child, RPoint2(pos.x() + CHILD_NODE_OFFSET, pos.y() - CHILD_NODE_OFFSET - posOffset));
        posOffset += CHILD_NODE_OFFSET;
    }
    auto name = node->name();
    nodeLabel_.setTexts(std::wstring(name.begin(), name.end()));
    nodeLabel_.setPosition(pos.x(), pos.y());
    nodeLabel_.render();
    return posOffset;
}

std::string RResourceWindow::getDefaultName() const
{
    return "ResourceWindow";
}

void RResourceWindow::initEvent(RInitEvent *)
{
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    rcWidgetWidth_ = RCLIST_WIDTH;

    windowShaders_ = RShaderProgram::getStanderdShaderProgram();
    windowShaders_.rename("ResourceWindowShaders");
    windowShaders_.use();
    windowShaders_.setViewprot("projection", 0, width(), 0, height());
    windowShaders_.setCameraPos("view", 0, 0);
    windowShaders_.nonuse();

    rcListBackground_ = RPlane(RCLIST_WIDTH - RCSCROLL_BAR_WIDTH, height() - RCLIST_TITLE_HEIGHT*2, "rcListBackground", RPoint(0, RCLIST_TITLE_HEIGHT));
    rcListBackground_.setColorTexture(CONTENT_COLOR);
    rcListBackground_.setShaderProgram(windowShaders_, "model");

    rcTitle_ = RTextPlane(L"Resource List", RCLIST_WIDTH, RCLIST_TITLE_HEIGHT, "rcTitle", RPoint(0, height() - RCLIST_TITLE_HEIGHT));
    rcTitle_.setPadding(0, 0, RCLIST_LEFT_ALIGNMENT, 0);
    rcTitle_.setFontColor(FONT_COLOR);
    rcTitle_.setWordSpacing(1.0f);
    rcTitle_.setFontSize(12);
    rcTitle_.setAlignment(RPlane::Align_Left, RPlane::Align_Mind);
    rcTitle_.setShaderProgram(windowShaders_, "model");
    rcTitle_.setBackgroundColor(BACKGROUND_COLOR);

    rcScrollBar_ = RPlane(RCSCROLL_BAR_WIDTH, rcListBackground_.height(), "rcScrollBar", RPoint(rcListBackground_.width(), rcListBackground_.y()));
    rcScrollBar_.setShaderProgram(windowShaders_, "model");
    rcScrollBarBack_ = rcScrollBar_;
    rcScrollBarBack_.rename("rcScrollBarBack");
    rcScrollBarBack_.setColorTexture(BACKGROUND_TOW_COLOR);
    rcScrollBar_.setMinimumSize(RCSCROLL_BAR_WIDTH, 5);
    rcScrollBar_.setColorTexture(KEY_COLOR);


    rcListPlane_ = RTextPlane(L"", 400, 0, "rcListPlane", rcListBackground_.pos());
    rcListPlane_.setPadding(RCLIST_LEFT_ALIGNMENT, 0, RCLIST_LEFT_ALIGNMENT, 0);
    rcListPlane_.setlineSpacing(1.7f);
    rcListPlane_.setWordSpacing(1.0f);
    rcListPlane_.setFontSize(12);
    rcListPlane_.setFontColor(FONT_COLOR);
    rcListPlane_.setShaderProgram(windowShaders_, "model");
    rcListPlane_.setAlignment(RPlane::Align_Left, RPlane::Align_Top);

    rcCountPlane_ = RTextPlane(L"", RCLIST_WIDTH, RCLIST_TITLE_HEIGHT, "rcCountPlane", RPoint(0, 0));
    rcCountPlane_.setPadding(0, 0, RCLIST_LEFT_ALIGNMENT, 0);
    rcCountPlane_.setAlignment(RPlane::Align_Left, RPlane::Align_Mind);
    rcCountPlane_.setWordSpacing(1.0f);
    rcCountPlane_.setFontSize(12);
    rcCountPlane_.setFontColor(FONT_COLOR);
    rcCountPlane_.setBackgroundColor(BACKGROUND_COLOR);
    rcCountPlane_.setShaderProgram(windowShaders_, "model");

    resourceList = RResource::queryResourceList();

    ctrlTreeTitle_ = rcTitle_;
    ctrlTreeTitle_.rename("ctrlTreeTitle");
    ctrlTreeTitle_.setPositionX(rcTitle_.width() + 1);
    ctrlTreeTitle_.setWidth(width() - ctrlTreeTitle_.x());
    ctrlTreeTitle_.setTexts(L"Controller Tree");

    nodeLabel_ = RTextPlane(L"", NODE_LABEL_LENGTH, RCLIST_TITLE_HEIGHT, "nodeLabel", RPoint());
    nodeLabel_.setPosition(ctrlTreeTitle_.x() + RCLIST_LEFT_ALIGNMENT, height() - RCLIST_TITLE_HEIGHT*2 - RCLIST_LEFT_ALIGNMENT);
    nodeLabel_.setAlignment(RPlane::Align_Left, RPlane::Align_Mind);
    nodeLabel_.setPadding(0, 0, RCLIST_LEFT_ALIGNMENT, 0);
    nodeLabel_.setFontSize(12);
    nodeLabel_.setFontColor(FONT_COLOR);
    nodeLabel_.setShaderProgram(windowShaders_, "model");
    nodeLabel_.setBackgroundColor(BACKGROUND_COLOR);

    cursor_.creatorCursor(RCursor::ARROW_CURSOR);
    setCursor(cursor_);

    updateReSourceList();
    timer_.start();
}

void RResourceWindow::closeEvent(RCloseEvent *)
{

}

void RResourceWindow::resizeEvent(RResizeEvent *event)
{
    windowShaders_.use();
    windowShaders_.setViewprot("projection", 0, width(), 0, height());
    windowShaders_.nonuse();

    rcListBackground_.setHeight(event->height - RCLIST_TITLE_HEIGHT*2);
    rcScrollBarBack_.setHeight(rcListBackground_.height());
    rcTitle_.setPositionY(event->height - RCLIST_TITLE_HEIGHT);
    updateReSourceList();

    ctrlTreeTitle_.setPositionY(rcTitle_.y());
    ctrlTreeTitle_.setPositionX(rcTitle_.width() + 1);
    ctrlTreeTitle_.setWidth(width() - ctrlTreeTitle_.x());
    nodeLabel_.setPosition(ctrlTreeTitle_.x() + RCLIST_LEFT_ALIGNMENT, height() - RCLIST_TITLE_HEIGHT*2 - RCLIST_LEFT_ALIGNMENT);
}

void RResourceWindow::scrollEvent(RScrollEvent *event)
{
    RPoint2 pos(event->x, event->y);

    if(rcListBackground_.getPlaneRect().contains(pos))
    {
        if(rcListPlane_.height() > rcListBackground_.height())
        {
            rcListPlaneOffsetY_ += (-event->direct) * rcListPlane_.lineHeight();
            updateReSourceList();
        }
    }
}

void RResourceWindow::inputEvent(RInputEvent *event)
{
    RCursor::CursorShape shape = RCursor::ARROW_CURSOR;

    if(event->press(RInputModule::KEY_ESCAPE))
        breakLoop();

    if(RMath::abs(event->cursorPos().x() - rcWidgetWidth_) < rcListHResizeRange_)
    {
        shape = RCursor::HRESIZE_CURSOR;

        if(event->press(RInputModule::MOUSE_BUTTON_LEFT) && RMath::abs(event->cursorPos().x() - rcWidgetWidth_) < rcListHResizeRange_)
            rcListHResizeRange_ = 1024;

        if(event->status(RInputModule::MOUSE_BUTTON_LEFT) == RInputModule::PRESS && rcListHResizeRange_ > 3)
        {
            if(event->cursorPos().x() > RCLIST_WIDTH)
            {
                rcWidgetWidth_ = event->cursorPos().x();
                rcTitle_.setWidth(rcWidgetWidth_);
                rcListBackground_.setWidth(rcWidgetWidth_ - RCSCROLL_BAR_WIDTH);
                rcScrollBar_.setPositionX(rcListBackground_.width());
                rcScrollBarBack_.setPositionX(rcListBackground_.width());
                rcCountPlane_.setWidth(rcWidgetWidth_);

                ctrlTreeTitle_.setPositionX(rcTitle_.width() + 1);
                ctrlTreeTitle_.setWidth(width() - ctrlTreeTitle_.x());

                nodeLabel_.setPositionX(ctrlTreeTitle_.x() + RCLIST_LEFT_ALIGNMENT);
            }
        }
        else {
            rcListHResizeRange_ = 3;
        }
    }

    if(cursor_.shape() != shape)
    {
        cursor_.creatorCursor(shape);
        setCursor(cursor_);
    }
}

void RResourceWindow::checkScrollBarOffer()
{
    int poffset = rcListPlane_.height() - rcListBackground_.height();
    rcListPlaneOffsetY_ = RMath::clamp(rcListPlaneOffsetY_, 0, poffset);

    int soffset = rcScrollBarBack_.height() - rcScrollBar_.height();
    double ratio = soffset*1.0 / poffset;
    rcScrollBarOffsetY_ = RMath::clamp(static_cast<int>(rcListPlaneOffsetY_ * ratio), 0, soffset);
}
