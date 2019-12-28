#include "Weiqi.h"

#include <RResourceWindow.h>

Weiqi::Weiqi():
    RWindowCtrl("WeiQi-Core"),
    shaders_(RShaderProgram::getStanderdShaderProgram()),
    board_(this, shaders_)
{
    setWindowSize(540, 540);
    setWindowTitle("WeiQi");
    setViewportPattern(RWindowCtrl::FixedSize);
    setWindowIcon(RImage(":/image/WeiQi-icon-32x32.png", "WeiQi-icon"));

    shaders_.use();
    shaders_.setViewprot("projection", 0, width(), 0, height());
    shaders_.setCameraPos("view", 0, 0);
    shaders_.nonuse();

}

void Weiqi::control()
{

}

void Weiqi::translation(const RController::TranslationInfo &info)
{
    if(info.sender != this)
        return;

    shaders_.use();
    shaders_.setViewprot("projection", 0, width(), 0, height());
    shaders_.nonuse();

    board_.translation({this, info.size, info.pos});
}

void Weiqi::inputEvent(RInputEvent *event)
{
    if(event->press(RInputModule::KEY_F12))
    {
        if(!getFreeTree()->getTreeNode("ResourceWindow"))
        {
            reWindowThread_ = RThread([this](){
                RResourceWindow reWindow;
                finished.connect(dynamic_cast<RController*>(&reWindow), &RResourceWindow::breakLoop);
                reWindow.showWindow();
                reWindow.exec();
            });
        }
    }

    if(event->press(RInputModule::KEY_ESCAPE))
    {
        breakLoop();
    }
}
