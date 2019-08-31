#include "testctrl.h"
#include "constant.h"
#include <RDebug.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TestCtrl::TestCtrl(RController *parent):
    RController(parent),
    VIEW_PROT_WIDTH(1600.0f),
    VIEW_PROT_HEIGHT(900.0f),
    move(0.0f, 0.0f),
    step(0.1f),
    ob(32, 32)
{
    RShader vertex(RE_PATH + "shaders/vertex.vert", RShader::VERTEX_SHADER);
    RShader fragment((RE_PATH + "shaders/fragment.frag"), RShader::FRAGMENT_SHADER);
    program.attachShader(vertex);
    program.attachShader(fragment);
    program.linkProgram();

    projection = glm::ortho(0.0f, VIEW_PROT_WIDTH, 0.0f, VIEW_PROT_HEIGHT, -1.0f, 1.0f);
    //projection = glm::mat4(1);

    view = glm::mat4(1);

    //model = glm::translate(model, {16.0f/2, 9.0f/2, 0.0f});
    ob.setPosition(800, 10);

    platform.push_back(new RObject(255, 120));
    platform.back()->setPosition(100, 200);

    platform.push_back(new RObject(400, 100));
    platform.back()->setPosition(700, 200);

    platform.push_back(new RObject(1700, 100));
    platform.back()->setPosition(-50, -91);

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

    glDisable(GL_CULL_FACE);
    program.use();
    program.setUniformMatrix4fv("view", glm::value_ptr(view));
    program.setUniformMatrix4fv("projection", glm::value_ptr(projection));

    for(auto p : platform)
    {
        p->render(&program);
    }

    ob.setColor(127, 127, 127);

    //给予引力
    if(ob.getVelocity().y > gravitation)
        ob.giveVelocity(0, -1);

    ob.motion();
    ob.move(move, forward);

    //RDebug() << ob.pos() << ob.getVelocity();
    for(auto p : platform)
    {
        platformCllision(ob, *p);
    }

    //RDebug() << ob.pos() << ob.getVelocity() << '\n';
    ob.render(&program);
}

void TestCtrl::keyPressEvent(RKeyEvent *event)
{
    //移动
    if(event->key() == RKeyEvent::KEY_RIGHT)
        move.x += 1.0f;
    if(event->key() == RKeyEvent::KEY_LEFT)
        move.x -= 1.0f;
    if(event->key() == RKeyEvent::KEY_Z)
        ob.setVelocityY(20);
}

void TestCtrl::keyReleaseEvent(RKeyEvent *event)
{
    //移动
    if(event->key() == RKeyEvent::KEY_RIGHT)
        move.x -= 1.0f;
    if(event->key() == RKeyEvent::KEY_LEFT)
        move.x += 1.0f;
    if(event->key() == RKeyEvent::KEY_Z)
        ob.stop();
}

void TestCtrl::mousePressEvent(RMouseEvent *event)
{
    //RDebug() << event->x() << event->y();
}

void TestCtrl::mouseReleaseEvent(RMouseEvent *event)
{
    //RDebug() << event->x() << event->y();
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

bool TestCtrl::platformCllision(RObject &ob, const RObject &platform)
{
    //平台碰撞检测
    if(platform.checkCollision(ob))
    {
        ob.setColor(255, 0, 0);
        glm::vec2 temp = ob.getVelocity();
        temp += move * static_cast<float>(forward);
        glm::vec2 oldPos = ob.pos();

        float intervalY = temp.y > 0.0f ? 1.0f : -1.0f;
        float tempY = temp.y;
        //RDebug() << temp.y << (temp.y <= -0.5f || temp.y >= 0.5f) << p->checkCollision(ob);
        while(tempY <= -0.5f || tempY >= 0.5f)
        {
            ob.ry() -= intervalY;
            tempY -= intervalY;
            if(!platform.checkCollision(ob))
            {
                if(tempY <= 0.0f)
                    ob.setVelocityY(0);
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
            ob.ry() = oldPos.y;
            while(temp.y <= -0.5f || temp.y >= 0.5f)
            {
                ob.ry() -= intervalY;
                temp.y -= intervalY;
                if(!platform.checkCollision(ob))
                {
                    if(temp.y <= 0.0f)
                        return true;
                }
            }
        }
    }
    return false;
}
