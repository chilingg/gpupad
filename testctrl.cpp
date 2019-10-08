#include "testctrl.h"
#include "constant.h"
#include <RDebug.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

TestCtrl::TestCtrl(RController *parent):
    RController(parent),
    VIEW_PROT_WIDTH(1600.0f),
    VIEW_PROT_HEIGHT(900.0f),
    viewProt({0.0f, 0.0f}, VIEW_PROT_WIDTH, VIEW_PROT_HEIGHT),
    charBox({0.0f, 0.0f}, 256, 256),
    identitymat(1),
    _move(0.0f, 0.0f),
    step(0.1f),
    ob(64, 64),
    moveAnimation(48, 30),
    textOb(200, 64)
{
    //timer.start();
}

TestCtrl::~TestCtrl()
{
    for(auto p : platform)
    {
        delete p;
    }
}

void TestCtrl::control()
{
    timer.elapsed(1.0/60.0);
    update();
    timer.start();
}

void TestCtrl::paintEvent()
{
    //FPS();

    //开启面剔除
    //glEnable(GL_CULL_FACE);

    //projection = glm::ortho(viewProt.leftF(), viewProt.rightF(), viewProt.bottomF(), viewProt.topF(), -1.0f, 1.0f);
    projection = glm::ortho(0.0f, VIEW_PROT_WIDTH, 0.0f, VIEW_PROT_HEIGHT, -1.0f, 1.0f);
    //视图移动
    charBox.setPos({ob.x() + (ob.width()/2 - charBox.width()/2), ob.y() + (ob.height()/2 - charBox.height()/2)});
    if(!viewProt.contains(charBox))
    {
        //RDebug() << charBox << viewProt;
        glm::vec2 vp = viewProt.getPos();
        if(!viewProt.contains(charBox, true, false, false, false))
        {
            //RDebug() << "T " << charBox.topF() << viewProt.topF();
            vp.y += charBox.topF() - viewProt.topF();
        }
        if(!viewProt.contains(charBox, false, true, false, false))
        {
            //RDebug() << "B ";
            vp.y += charBox.bottomF() - viewProt.bottomF();
        }
        if(!viewProt.contains(charBox, false, false, true, false))
        {
            //RDebug() << "L ";
            vp.x += charBox.leftF() - viewProt.leftF();
        }
        if(!viewProt.contains(charBox, false, false, false, true))
        {
            //RDebug() << "R ";
            vp.x += charBox.rightF() - viewProt.rightF();
        }
        viewProt.setPos(vp);
    }
    view = identitymat;
    view = glm::translate(view, {-viewProt.getPos(), 0.0f});


    colorProgram.use();
    colorProgram.setUniformMatrix4fv("projection", glm::value_ptr(projection));
    colorProgram.setUniformMatrix4fv("view", glm::value_ptr(view));

    texProgram.use();
    texProgram.setUniformMatrix4fv("projection", glm::value_ptr(projection));
    texProgram.setUniformMatrix4fv("view", glm::value_ptr(view));

    for(auto p : platform)
    {
        p->render(&colorProgram);
    }

    //给予引力
    if(ob.getVelocity().y > GRAVITATION)
        ob.giveVelocity(0, -1);

    //移动
    if(_move.x < 0)
    {
        ob.flipH(true);
        moveAnimation.flipH(true);
    }
    else if(_move.x > 0)
    {
        ob.flipH(false);
        moveAnimation.flipH(false);
    }
    //RDebug() << sprint;
    if(sprint > 10.0f || sprint < -10.0f)
        ob.setVelocityX(sprint);
    else
        ob.setVelocityX(0);
    sprint *= 0.95f;
    ob.motion();
    ob.move(_move, FORWARD);

    //检查碰撞
    glm::vec2 velocity = ob.getVelocity();
    velocity += _move * static_cast<float>(FORWARD);
    for(auto p : platform)
    {
        if(ob.moveCollision(velocity, *p))
        {
            if(velocity.y == 0.0f)
            {
                ob.setVelocityY(0);
                if(velocity.x > 0.0f)
                {
                    moveAnimation.setPositionX(ob.volume().left() - moveAnimation.width());
                    moveAnimation.setPositionY(ob.volume().bottom());
                    moveAnimation.render(&texProgram);
                }
                if(velocity.x < 0.0f)
                {
                    moveAnimation.setPositionX(ob.volume().right());
                    moveAnimation.setPositionY(ob.volume().bottom());
                    moveAnimation.render(&texProgram);
                }
            }
        }
        //platformCllision(ob, *p);
    }

    if(velocity == glm::vec2{0.0f, 0.0f} && ob.state() == Character::moved)
        ob.setState(Character::quiet);
    else if(velocity != glm::vec2{0.0f, 0.0f} && ob.state() == Character::quiet)
        ob.setState(Character::moved);
    ob.render(&texProgram);
    //ob.displayVolume(projection, view);
    //moveAnimation.displayVolume(projection, view);

    //projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f);
    projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 1.0f);
    texProgram.use();
    texProgram.setUniformMatrix4fv("view", glm::value_ptr(identitymat));
    texProgram.setUniformMatrix4fv("projection", glm::value_ptr(projection));
    textOb.textProgram->use();
    textOb.textProgram->setUniformMatrix4fv("projection", glm::value_ptr(projection));
    textOb.textProgram->setUniformMatrix4fv("view", glm::value_ptr(identitymat));

    textOb.render(&texProgram);
    textOb.displayVolume(projection, identitymat);
}

void TestCtrl::keyPressEvent(RKeyEvent *event)
{
    //移动
    if(event->key() == RKeyEvent::KEY_RIGHT)
        _move.x += 1.0f;
    if(event->key() == RKeyEvent::KEY_LEFT)
        _move.x -= 1.0f;
    if(event->key() == RKeyEvent::KEY_Z)
        ob.setVelocityY(20);
    if(event->key() == RKeyEvent::KEY_C && (sprint < 2.0f && sprint > -2.0f))
    {
        sprint = ob.isFlipH() ? -SPRINT : SPRINT;
        //textOb.setFontSizeRatio(100.0f);
    }
}

void TestCtrl::keyReleaseEvent(RKeyEvent *event)
{
    //移动
    if(event->key() == RKeyEvent::KEY_RIGHT)
        _move.x -= 1.0f;
    if(event->key() == RKeyEvent::KEY_LEFT)
        _move.x += 1.0f;
    if(event->key() == RKeyEvent::KEY_Z)
    {
        if(ob.getVelocity().y > 0.0f)
            ob.stop();
    }
}

void TestCtrl::mousePressEvent(RMouseEvent *event)
{
    RDebug() << event->x()*(VIEW_PROT_WIDTH/width) << (height-event->y())*(VIEW_PROT_HEIGHT/height);
}

void TestCtrl::mouseReleaseEvent(RMouseEvent *event)
{
    //RDebug() << event->x() << event->y();
}

void TestCtrl::resizeEvent(RResizeEvent *event)
{
    //RDebug() << event->width() << event->height();
    width = event->width();
    height = event->height();

    //textOb.setFontSizeRatio(height, VIEW_PROT_HEIGHT);
    //textOb.setPosition(150, 150);
    textOb.setPosition(1, height-textOb.outerHeight()-1);
}

void TestCtrl::joystickPresentEvent(RJoystickEvent *event)
{
    if(joystick.isDisconnected())
    {
        if(event->isConnected())
            joystick.setJid(event->jid());
    }
    else
    {
        if(event->isDisconnected() && event->jid() == joystick.jid())
            joystick.setInvalid();
    }
}

void TestCtrl::joystickInputEvent(RJoystickEvent *event)
{
    if(event->button() == RJoystick::GAMEPAD_BUTTON_A)
    {
        if(event->buttonValue())
            ob.setVelocityY(20);
        else
            if(ob.getVelocity().y > 0.0f)
                ob.stop();
    }
    if(event->button() == RJoystick::GAMEPAD_BUTTON_DPAD_LEFT)
    {
        event->buttonValue() ? _move.x -= 1.0f : _move.x += 1.0f;
    }
    if(event->button() == RJoystick::GAMEPAD_BUTTON_DPAD_RIGHT)
    {
        event->buttonValue() ? _move.x += 1.0f : _move.x -= 1.0f;
    }
    if(event->axis() == RJoystick::GAMEPAD_AXIS_RIGHT_TRIGGER && (sprint < 2.0f && sprint > -2.0f))
    {
        sprint = ob.isFlipH() ? -SPRINT : SPRINT;
    }
}

void TestCtrl::initEvent()
{
    RShader vertex(RE_PATH + "shaders/vertex.vert", RShader::VERTEX_SHADER);
    RShader fragment((RE_PATH + "shaders/fragment.frag"), RShader::FRAGMENT_SHADER);
    RShader texShader((RE_PATH + "shaders/texture.frag"), RShader::FRAGMENT_SHADER);
    colorProgram.attachShader(vertex);
    colorProgram.attachShader(fragment);
    colorProgram.linkProgram();
    texProgram.attachShader(vertex);
    texProgram.attachShader(texShader);
    texProgram.linkProgram();

    //model = glm::translate(model, {16.0f/2, 9.0f/2, 0.0f});
    ob.setPosition(800, 10);
    ob.setMargin(-10, 0, 0, 0);
    ob.setTextureSizePattern(RTexObject::Auto);

    moveAnimation.addTexture("Moved_1", RE_PATH+"texture/Moved_1.png");
    moveAnimation.addTexture("Moved_2", RE_PATH+"texture/Moved_2.png");
    moveAnimation.setCurrentTextureArray({"Moved_1", "Moved_2"});
    moveAnimation.setPadding(0, 0, 10, 10);

    platform.push_back(new RObject(32, 800));
    platform.back()->setPosition(100, 200);

    platform.push_back(new RObject(200, 64));
    platform.back()->setPosition(132, 400);

    platform.push_back(new RObject(200, 32));
    platform.back()->setPosition(325, 110);

    platform.push_back(new RObject(100, 32));
    platform.back()->setPosition(460, 330);

    platform.push_back(new RObject(400, 32));
    platform.back()->setPosition(700, 200);

    //地板
    platform.push_back(new RObject(1700, 100));
    platform.back()->setPosition(-50, -91);

    //textOb.setMargin(10);
    textOb.setPadding(5, 0, 0, 0);
    textOb.setPosition(1, height-textOb.outerHeight()-1);
    //textOb.setPosition(50, 50);
    textOb.setAlignment(RTexObject::Align_Top, RTexObject::Align_Mind);
    //textOb.setTextureSizePattern(RTexObject::Length);
    textOb.setColor(255, 100, 0);
    //textOb.setTexts(L"行文本^测.试a1, 完毕。-_-（)1234567890");
    textOb.setTexts(L"abcdefghijklmnopqrstuvwxyz");
    //textOb.setTexts(L"12345678901234567890123456789012345678901234567890");
    textOb.setFontSize(24);
    textOb.setRowSpacing(1.0f);
    //textOb.setSize(200, 200);
    textOb.setFont(RE_PATH+"fonts/SourceHanSerifSC_EL-M/SourceHanSerifSC-Regular.otf");
}

void TestCtrl::FPS()
{
    static RTimer t;
    static int fps = 0;
    ++fps;
    if(t.elapsed() >= 1.0)
    {
        RDebug() << "fps:" << fps;//5000 1700
        fps = 0;
        t.start();
    }

}

bool TestCtrl::platformCllision(Character &ob, const RObject &platform)
{
    //平台碰撞检测
    if(platform.checkCollision(ob))
    {
        glm::vec2 temp = ob.getVelocity();
        temp += _move * static_cast<float>(FORWARD);

        float intervalY = temp.y > 0.0f ? 1.0f : -1.0f;
        float tempY = temp.y;
        //RDebug() << temp.y << (temp.y <= -0.5f || temp.y >= 0.5f) << p->checkCollision(ob);
        while(tempY <= -0.5f || tempY >= 0.5f)
        {
            ob.ry() -= intervalY;
            tempY -= intervalY;
            if(!platform.checkCollision(ob))
            {
                if(temp.y < 0.0f)
                {
                    ob.setVelocityY(0);
                }
                return true;
            }
        }
        float intervalX = temp.x > 0.0f ? 1.0f : -1.0f;
        while(temp.x <= -0.5f || temp.x >= 0.5f)
        {
            ob.rx() -= intervalX;
            temp.x -= intervalX;
            if(!platform.checkCollision(ob))
            {
                break;
            }
        }
        if(platform.checkCollision(ob))
        {
            return false;
        }
        else
        {
            while(temp.y <= -0.5f || temp.y >= 0.5f)
            {
                ob.ry() += intervalY;
                temp.y -= intervalY;
                if(platform.checkCollision(ob))
                {
                    RDebug() << "Y axis error!";
                    //ob.ry() += intervalY;
                    //return true;
                }
            }
        }
    }
    return false;
}

void TestCtrl::move(int lr)
{
    if(lr == Left)
        _move.x -= 1.0f;
    else if(lr == Right)
        _move.x += 1.0f;
}
