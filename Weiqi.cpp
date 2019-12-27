#include "Weiqi.h"

#include <RResourceWindow.h>

Weiqi::Weiqi():
    RWindowCtrl("WeiQi-Core"),
    shaders_(RShaderProgram::getStanderdShaderProgram()),
    board(this, shaders_)
{
    setWindowSize(540, 540);
    setWindowTitle("WeiQi");
    setVSync();
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

void Weiqi::resizeEvent(RResizeEvent *event)
{
    shaders_.use();
    shaders_.setViewprot("projection", 0, event->width, 0, event->height);
    shaders_.nonuse();
}
