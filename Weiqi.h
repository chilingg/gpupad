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

    void control() override;
    void translation(const TranslationInfo &info) override;

protected:
    void inputEvent(RInputEvent *event) override;

private:
    RShaderProgram shaders_;
    RThread reWindowThread_;
    WeiqiBoard board_;
};

#endif // WEIQI_H
