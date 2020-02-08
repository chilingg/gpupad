#include "RPlane.h"
#include "RKeeper.h"

using namespace Redopera;

thread_local RShaderProgram RPlane::tPlaneShaders;
thread_local GLuint RPlane::modelLoc;
thread_local GLuint RPlane::edgingLoc;

void RPlane::setPlaneShadersAsThread(const RShaderProgram &shaders, GLuint mLoc)
{
    tPlaneShaders = shaders;
    modelLoc = mLoc;
    edgingLoc = -1;
}

void RPlane::setPlaneShadersAsThread(const RShaderProgram &shaders, GLuint mLoc, GLuint eLoc)
{
    tPlaneShaders = shaders;
    modelLoc = mLoc;
    edgingLoc = eLoc;
}

const RShaderProgram &RPlane::planeShader()
{
    return planeRenderTool().shaders;
}

RPlane::RPlane():
    mats_{ glm::mat4(1), glm::mat4(1), glm::mat4(1) },
    model_(1),
    texture_(defaultTexture())
{

}

RPlane::RPlane(int width, int height, int x, int y, int z, const RTexture &tex):
    RArea(width, height, x, y, z),
    mats_{ glm::mat4(1), glm::mat4(1), glm::mat4(1) },
    model_(1),
    texture_(tex)
{

}

RPlane::RPlane(int width, int height, const RPoint &pos, const RTexture &tex):
    RArea(width, height, pos),
    mats_{ glm::mat4(1), glm::mat4(1), glm::mat4(1) },
    model_(1),
    texture_(tex)
{

}

RPlane::RPlane(const RSize &size, const RPoint &pos, const RTexture &tex):
    RArea(size, pos),
    mats_{ glm::mat4(1), glm::mat4(1), glm::mat4(1) },
    model_(1),
    texture_(tex)
{

}

RPlane::RPlane(const RRect &rect, int z, const RTexture &tex):
    RArea(rect, z),
    mats_{ glm::mat4(1), glm::mat4(1), glm::mat4(1) },
    model_(1),
    texture_(tex)
{

}

RPlane::RPlane(const RArea::Format &format, const RTexture &tex):
    RArea(format),
    mats_{ glm::mat4(1), glm::mat4(1), glm::mat4(1) },
    model_(1),
    texture_(tex)
{

}

RPlane::RPlane(const RPlane &plane):
    RArea(plane),
    mats_(plane.mats_),
    model_(plane.model_),
    texture_(plane.texture_)
{

}

RPlane::RPlane(const RPlane &&plane):
    RArea(plane),
    mats_(std::move(plane.mats_)),
    model_(std::move(plane.model_)),
    texture_(std::move(plane.texture_))
{

}

const RPlane::ModelMat &RPlane::modelMat() const
{
    return mats_;
}

void RPlane::setColorTexture(const RColor &color)
{
    R_RGBA rgba = color.rgba();
    setColorTexture(rgba);
}

void RPlane::setColorTexture(R_RGBA rgba)
{
    static RTexture::Format format = RTexture::makeTexFormat();

    const RData *colorData = reinterpret_cast<RData*>(&rgba);
    texture_.load(colorData, 1, 1, 4, format);
}

void RPlane::setColorTexture(unsigned r, unsigned g, unsigned b, unsigned a)
{
    RColor color(r, g, b, a);
    setColorTexture(color.rgba());
}

void RPlane::setTexture(const RImage &img)
{
    texture_.load(img);
}

void RPlane::setTexture(const RTexture &tex)
{
    texture_ = tex;
}

void RPlane::update()
{
    float w, h, tw = texture_.width(), th = texture_.height();
    float min = innerWidth() / tw;
    float max = innerHeight() / th;
    if(min > max) std::swap(min, max);

    switch(format().mode)
    {
    case RArea::Mode::Fixed:
        w = tw;
        h = th;
        break;
    case RArea::Mode::Auto:
        w = innerWidth();
        h = innerHeight();
        break;
    case RArea::Mode::Cover:
        w = tw * max;
        h = th * max;
        break;
    case RArea::Mode::Contain:
        w = tw * min;
        h = th * min;
        break;
    }

    float x, y;
    switch(format().align.h)
    {
    case RArea::Align::Left:
        x = w / 2.0f + format().padding.l;
        break;
    case RArea::Align::Right:
        x = format().size.width() - w/2.0f - format().padding.r;
        break;
    default: // RArea::Align::Mind
        x = format().size.width() / 2.0f;
        break;
    }
    switch(format().align.v)
    {
    case RArea::Align::Bottom:
        y = h / 2.0f + format().padding.b;
        break;
    case RArea::Align::Top:
        y = format().size.height() - h/2.0f - format().padding.t;
        break;
    default: // RArea::Align::Mind
        y = format().size.height() / 2.0f;
        break;
    }

    mats_.tran = glm::translate(glm::mat4(1), { format().pos.x() + x, format().pos.y() + y, 0 });
    mats_.rotate = glm::mat4_cast(glm::qua<float>(glm::vec3{ format().rotate.x, format().rotate.x, format().rotate.x }));
    mats_.scale = glm::scale(glm::mat4(1), { w, h, 0.0f });

    if(format().flipH)
    {
        mats_.scale[0][0] *= -1;
        mats_.scale[0][1] *= -1;
        mats_.scale[0][2] *= -1;
        mats_.scale[0][3] *= -1;
    }
    if(format().flipV)
    {
        mats_.scale[1][0] *= -1;
        mats_.scale[1][1] *= -1;
        mats_.scale[1][2] *= -1;
        mats_.scale[1][3] *= -1;
    }

    model_ = mats_.tran * mats_.rotate * mats_.scale;
    clearDirty();
}

void RPlane::render()
{
    const RenderTool& rt = planeRenderTool();
    glBindVertexArray(rt.vao);

    if(isDirty())
        update();

    RShaderProgram::Interface inter = rt.shaders.useInterface();
    inter.setUniform(rt.edgingLoc, 0);
    renderControl(rt.shaders, rt.modelLoc);

    glBindVertexArray(0);
}

void RPlane::render(const RShaderProgram &shaders, GLuint mLoc)
{
    const RenderTool& rt = planeRenderTool();
    glBindVertexArray(rt.vao);

    if(isDirty())
        update();

    RShaderProgram::Interface inter = shaders.useInterface();
    inter.setUniform(mLoc, 0);
    renderControl(shaders, mLoc);

    glBindVertexArray(0);
}

void RPlane::edging()
{
    const RenderTool& rt = planeRenderTool();
    glBindVertexArray(rt.edgingVAO);

    glm::mat4 mats[3] { glm::mat4(1), glm::mat4(1), glm::mat4(1)};

    mats[0] = glm::translate(mats[0], { format().pos.x(), format().pos.y(), format().pos.z() });
    mats[0] = glm::scale(mats[0], { width(), height(), 0 });
    mats[1] = glm::translate(mats[1], { innerPos().x(), innerPos().y(), format().pos.z() });
    mats[1] = glm::scale(mats[1], { innerWidth(), innerHeight() , 0 });
    mats[2] = glm::translate(mats[2], { outerPos().x(), outerPos().y(), format().pos.z() });
    mats[2] = glm::scale(mats[2], { outerWidth(), outerHeight() , 0 });

    RShaderProgram::Interface inter = rt.shaders.useInterface();
    inter.setUniformMatrix(rt.modelLoc, mats, 3);
    inter.setUniform(rt.edgingLoc, 1);
    texture_.bind();

    glDrawArraysInstanced(GL_LINE_LOOP, 0, 4, 3);
    glBindVertexArray(0);
}

void RPlane::edging(const RShaderProgram &shaders, GLuint mLoc, GLuint eLoc)
{
    const RenderTool& rt = planeRenderTool();
    glBindVertexArray(rt.edgingVAO);

    if(isDirty())
        update();

    glm::mat4 mats[3] { model_ };
    mats[1] = mats_.tran * mats_.rotate
            * glm::translate(glm::mat4(1), { -format().padding.l - format().padding.r, -format().padding.t - format().padding.b , 0})
            * mats_.scale;
    mats[2] = mats_.tran * mats_.rotate
            * glm::translate(glm::mat4(1), { format().margin.l + format().margin.r, format().margin.t + format().margin.b , 0})
            * mats_.scale;

    RShaderProgram::Interface inter = shaders.useInterface();
    inter.setUniformMatrix(mLoc, mats, 3);
    inter.setUniform(eLoc, 1);
    texture_.bind();

    glDrawArraysInstanced(GL_LINE_LOOP, 0, 4, 3);
    glBindVertexArray(0);
}

const RPlane::RenderTool RPlane::planeRenderTool()
{
    thread_local static std::unique_ptr<GLuint[], std::function<void(GLuint *p)>> vao(new GLuint[2], [](GLuint *p){
    glDeleteVertexArrays(2, p); delete [] p; });
    thread_local static std::unique_ptr<GLuint[], std::function<void(GLuint *p)>> vbo(new GLuint[2], [](GLuint *p){
    glDeleteBuffers(2, p); delete [] p; });

    if(!vao[0])
    {
        glGenVertexArrays(2, vao.get());
        glGenBuffers(2, vbo.get());

        float plane[24]{
                0.5f,-0.5f, 0.0f, 1.0f, 0.0f,//右下
                0.5f, 0.5f, 0.0f, 1.0f, 1.0f,//右上
               -0.5f,-0.5f, 0.0f, 0.0f, 0.0f,//左下
               -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,//左上
        };
        glBindVertexArray(vao[0]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), bufOff(3*sizeof(float)));
        glBindVertexArray(0);

        float edge[12]{
                1.0f, 0.0f, 0.0f,//右下
                1.0f, 1.0f, 0.0f,//右上
                0.0f, 1.0f, 0.0f,//左上
                0.0f, 0.0f, 0.0f,//左下
        };
        glBindVertexArray(vao[1]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(edge), edge, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);
        glBindVertexArray(0);
    }

    if(!tPlaneShaders.isValid())
    {
        static const GLchar *vCode = {
            "#version 330 core\n"
            "layout(location = 0) in vec3 aPos;\n"
            "layout(location = 1) in vec2 aTexCoor;\n"
            "uniform int edging;\n"
            "uniform mat4 model[3];\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "out vec4 texCoor; // 末位检测是否渲染边框\n"
            "void main(void)\n"
            "{\n"
            "	if(edging == 1)\n"
            "	{\n"
                    "gl_Position = projection * view * model[gl_InstanceID] * vec4(aPos, 1.0);\n"
                    "if(gl_InstanceID == 0)\n"
                        "texCoor = vec4(1.0, 0, 0, 1.0);\n"
                    "else if(gl_InstanceID == 1)\n"
                        "texCoor = vec4(0, 1.0, 0, 1.0);\n"
                    "else\n"
                        "texCoor = vec4(0, 0, 1.0, 1.0);\n"
                "} \n"
                "else {\n"
                    "gl_Position = projection * view * model[0] * vec4(aPos, 1.0);\n"
                    "texCoor = vec4(aTexCoor, 0, 0.0);\n"
                "}\n"
            "}\n"
        };
        static const GLchar *fCode = {
            "#version 330 core\n"
            "in vec4 texCoor; // 边框渲染时用作颜色值\n"
            "out vec4 outColor;\n"
            "uniform sampler2D tex;\n"
            "void main(void)\n"
            "{\n"
            "	if(texCoor.a == 0)\n"
                    "outColor = texture(tex, texCoor.st);\n"
                "else\n"
                    "outColor = texCoor;\n"
            "}\n"
        };

        tPlaneShaders.releaseShader();
        tPlaneShaders.attachShader({ RShader(vCode, RShader::Type::Vertex), RShader(fCode, RShader::Type::Fragment)});
        tPlaneShaders.linkProgram();
        auto inter = tPlaneShaders.useInterface();
        modelLoc = tPlaneShaders.getUniformLocation("model");
        edgingLoc = tPlaneShaders.getUniformLocation("edging");
    }

    return { tPlaneShaders, modelLoc, vao[0], vao[1], edgingLoc };
}

void RPlane::renderControl(const RShaderProgram &shaders, GLuint mLoc)
{
    auto inter = shaders.useInterface();
    inter.setUniformMatrix(mLoc, model_);
    texture_.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

const RTexture &RPlane::defaultTexture()
{
    static const RTexture DEFAULT_TEXTURE(reinterpret_cast<const RData*>("\xff"), 1, 1, 1);
    return DEFAULT_TEXTURE;
}
