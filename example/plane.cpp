#include <RWindow.h>
#include <RDebug.h>
#include <RPlane.h>
#include <RTextbox.h>

using namespace Redopera;

class TestCtl: public RController
{
public:
    TestCtl(RController *parent, const std::string &name):
        RController(parent, name),
        plane(32, 32, RPoint(10, 0), RImage::redoperaIcon()),
        texts("Text", 60, 20, 10, 10)
    {
        texts.setFontColor(180, 180, 255);
        texts.setBackColor(180, 180, 255, 50);
        pro = RPlane::planeShader().getUniformLocation("projection");
        view = RPlane::planeShader().getUniformLocation("view");
    }

    void control() override
    {
        //plane.render();
        //plane.edgingAll();
        texts.render();
        texts.edgingAll();
    }

protected:
    void startEvent(RStartEvent &) override
    {
        RWindow * window = RWindow::getMainWindow();
        plane.setPosY(window->height() / 2 - 16);
        plane.flipV();
        plane.setMargin(2);
        plane.setPadding(2);

        // 必须设置一次的视口与视口位移
        {
        RShaderProgram::Interface inter = RPlane::planeShader().useInterface();
        inter.setViewprot(pro, 0, window->width(), 0, window->height());
        inter.setCameraMove(view, 0, 0, 0);
        }

        const RShaderProgram &shaders = RTextsbxo::textboxShader();
        RShaderProgram::Interface inter = shaders.useInterface();
        inter.setViewprot(shaders.getUniformLocation("projection"), 0, window->width(), 0, window->height());
        inter.setCameraMove(shaders.getUniformLocation("view"), 0, 0, 0);
    }

    void translation(const TranslationInfo &info) override
    {
        {
        RShaderProgram::Interface inter = RPlane::planeShader().useInterface();
        inter.setViewprot(pro, 0, info.size.width(), 0, info.size.height());
        }

        const RShaderProgram &shaders = RTextsbxo::textboxShader();
        RShaderProgram::Interface inter = shaders.useInterface();
        inter.setViewprot(shaders.getUniformLocation("projection"), 0, info.size.width(), 0, info.size.height());
    }

    void inputEvent(RInputEvent &e) override
    {
        // inputEvent只能监测感兴趣的按键
        if(e.press(Keys::KEY_ESCAPE))
            getParent()->breakLoop();

        if(e.status(Keys::KEY_LEFT) == ButtonAction::PRESS)
            plane.rx() -= 5;
        if(e.status(Keys::KEY_RIGHT) == ButtonAction::PRESS)
            plane.rx() += 5;
        if(e.status(Keys::KEY_UP) == ButtonAction::PRESS)
            plane.ry() += 5;
        if(e.status(Keys::KEY_DOWN) == ButtonAction::PRESS)
            plane.ry() -= 5;
    }

private:
    RPlane plane;
    RTextsbxo texts;
    GLuint pro, view;
};

int main()
{
    RWindow::Format format;
    format.initWidth = 800;
    format.initHeight = 520;
    format.debug = true;
    format.background = 0x181010;
    RWindow window(format, nullptr, "Plane");

    TestCtl t(&window, "ChildNode");

    window.show();
    return window.exec();
}
