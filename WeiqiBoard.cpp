#include "WeiqiBoard.h"

WeiqiBoard::WeiqiBoard(RController *parent, const RShaderProgram &shaders):
    RController("WeiqiBoard", parent),
    board_(540, 540, "Board", RPoint(0))
{
    board_.setTexture(RImage(":/image/board.png", "boardImg", true));
    board_.setShaderProgram(shaders, "model");
}

void WeiqiBoard::control()
{
    board_.render();
}

void WeiqiBoard::translation(const RController::TranslationInfo &info)
{
    board_.setSize(info.size);
}
