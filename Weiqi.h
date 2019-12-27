#ifndef WEIQI_H
#define WEIQI_H

#include <RWindowCtrl.h>
#include <RResource/RShaderProgram.h>
#include <RThread.h>

#include "WeiqiBoard.h"

class Weiqi : public RWindowCtrl
{
public:
    Weiqi();

protected:
    void control() override;
    void inputEvent(RInputEvent *event) override;
    void resizeEvent(RResizeEvent *event) override;

private:
    RShaderProgram shaders_;
    RThread reWindowThread_;
    WeiqiBoard board;
};

#endif // WEIQI_H
