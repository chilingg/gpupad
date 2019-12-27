#ifndef WEIQIBOARD_H
#define WEIQIBOARD_H

#include <RController.h>
#include <RPlane.h>

class WeiqiBoard : public RController
{
public:
    WeiqiBoard(RController *parent, const RShaderProgram &shaders);

    void control() override;
    void translation(const TranslationInfo &info) override;

private:
    RPlane board_;
};

#endif // WEIQIBOARD_H
