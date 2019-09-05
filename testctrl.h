#ifndef TESTCTRL_H
#define TESTCTRL_H

#include <RShaderProgram.h>
#include <RController.h>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <RTimer.h>
#include <vector>

#include "character.h"

class TestCtrl : public RController
{
public:
    TestCtrl(RController *parent = nullptr);
    ~TestCtrl() override;

protected:
    void control() override;
    void paintEvent() override;
    void keyPressEvent(RKeyEvent *event) override;
    void keyReleaseEvent(RKeyEvent *event) override;
    void mousePressEvent(RMouseEvent *event) override;
    void mouseReleaseEvent(RMouseEvent *event) override;
    void resizeEvent(RResizeEvent *event) override;

private:
    void FPS();
    bool platformCllision(Character &ob, const RObject &platform);
    bool standIn(const RObject &platform);

    const int forward = 10;
    const int gravitation = -30;
    const float VIEW_PROT_WIDTH;
    const float VIEW_PROT_HEIGHT;
    RVolume viewProt;
    RVolume charBox;
    glm::vec2 move;
    float step;
    Character ob;

    int width;
    int height;
    std::vector<RObject*> platform;
    RTimer timer;
    RShaderProgram colorProgram;
    RShaderProgram texProgram;
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
};

inline bool TestCtrl::standIn(const RObject &platform)
{
    if(ob.touchSide(platform, RVolume::Top))
    {
        ob.setPositionY(platform.volume().top());
        return true;
    }
    return false;
}

#endif // TESTCTRL_H
