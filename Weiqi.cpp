#include "Weiqi.h"

#include <RResourceWindow.h>
#include <RJson.h>
#include <fstream>

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

    std::ifstream config("../config.json");
    RJosn j;
    config >> j;
    config.close();

    RDebug() << '<' << j["Name"].get<std::string>() << "> "
            << "Author: " << j["Author"].get<std::string>() << ' '
            << "Vertion: " << j["Vertion"][0] << '.' << j["Vertion"][1] << ' '
            << "Date: " << j["Date"].get<std::string>();
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
                RSigSlot::connect(&finished, &reWindow, &RResourceWindow::breakLoop);
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
