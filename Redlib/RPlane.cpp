#include "RPlane.h"

#include "RDebug.h"

void RPlane::setViewpro(int left, int right, int buttom, int top, int near, int far)
{
    planeSProgram.use();
    RMatrix4 projection = Rglm::ortho(static_cast<float>(left),
                                      static_cast<float>(right),
                                      static_cast<float>(buttom),
                                      static_cast<float>(top),
                                      static_cast<float>(near),
                                      static_cast<float>(far));
    planeSProgram.setUniformMatrix(projectionLoc, 4, Rglm::value_ptr(projection));
    planeSProgram.nonuse();
    //RDebug() << projection << "projection";
}

void RPlane::setViewproMove(int x, int y, int z)
{
    planeSProgram.use();
    RMatrix4 view = Rglm::translate(RMatrix4(1), {-x, -y, -z});
    planeSProgram.setUniformMatrix(viewLoc, 4, Rglm::value_ptr(view));
    planeSProgram.nonuse();
    //RDebug() << view << "view";
}

RPlane::RPlane(): RPlane("plane", 32, 32)
{

}

RPlane::RPlane(const RPlane &plane):
    name_(plane.name_),
    width_(plane.width_),
    height_(plane.height_),
    pos_(plane.pos_),
    rotateMat_(plane.rotateMat_),
    modelMat_(plane.modelMat_),
    texture_(plane.texture_),
    shaders_(plane.shaders_),
    dirty_(plane.dirty_),
    flipH_(plane.flipH_),
    flipV_(plane.flipV_),
    marginTop_(plane.marginTop_),
    marginBottom_(plane.marginBottom_),
    marginLeft_(plane.marginLeft_),
    marginRight_(plane.marginRight_),
    paddingTop_(plane.paddingTop_),
    paddingBottom_(plane.paddingBottom_),
    paddingLeft_(plane.paddingLeft_),
    paddingRight_(plane.paddingRight_),
    sizeMode_(plane.sizeMode_),
    vAlign_(plane.vAlign_),
    hAlign_(plane.hAlign_)
{
    ++count;
}

RPlane::RPlane(const std::string &name, int width, int height, RPoint pos):
    name_(name),
    width_(width),
    height_(height),
    pos_(pos),
    rotateMat_(1.0f),
    modelMat_(1.0f),
    texture_(),
    shaders_()
{
    if(++count == 1)
    {
        glGenVertexArrays(1, &planeVAO);
        glGenBuffers(1, &planeVBO);
        glBindVertexArray(planeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);

        float plane[24]{
                0.5f,-0.5f, 0.0f, 1.0f, 0.0f,//右下
                0.5f, 0.5f, 0.0f, 1.0f, 1.0f,//右上
               -0.5f,-0.5f, 0.0f, 0.0f, 0.0f,//左下
               -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,//左上
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), offsetBuffer(3*sizeof(float)));
        glBindVertexArray(0);

        planeSProgram.rename("DefaultPlanProgram");
        planeSProgram.attachShader(":/shader/Vertex.vert", ShaderType::VertexShader);
        planeSProgram.attachShader(":/shader/Fragment.frag", ShaderType::FragmentShader);
        planeSProgram.linkProgram();

        const unsigned char white[] = { 0xff, 0xff, 0xff };
        whiteTex.rename("DefaultTexture");
        whiteTex.setTexFilter(RTexture::Nearest);
        whiteTex.generate(1, 1, 3, white);

        planeSProgram.use();
        modelLoc = planeSProgram.getUniformLocation("model");
        viewLoc = planeSProgram.getUniformLocation("view");
        projectionLoc = planeSProgram.getUniformLocation("projection");
        //setViewpro(0, 900, 0, 1600);
        setViewpro(0, 1600, 0, 900, -127, 128);
        setViewproMove(0, 0);
    }

    shaders_= planeSProgram;
    texture_ = whiteTex;
}

RPlane::~RPlane()
{
    if(--count == 0)
    {
        whiteTex.freeTexture();
        planeSProgram.freeShaderProgram();
        glDeleteBuffers(1, &planeVBO);
        glDeleteVertexArrays(1, &planeVAO);
#ifdef R_DEBUG
        if(lineBoxsProgram.isValid())
        {
            lineBoxsProgram.freeShaderProgram();
            glDeleteVertexArrays(1, &lineBoxVAO);
        }
#endif
    }
}

void RPlane::setSize(int width, int height)
{
    width_ = width; height_ = height;
    updataModelMat();
}

void RPlane::setSize(RSize size)
{
    width_ = size.width(); height_ = size.height();
    updataModelMat();
}

void RPlane::setPosition(int x, int y, int z)
{
    pos_ = RPoint(x, y, z);
    updataModelMat();
}

void RPlane::setPositionX(int x)
{
    pos_.setX(x);
    updataModelMat();
}

void RPlane::setPositionY(int y)
{
    pos_.setY(y);
    updataModelMat();
}

void RPlane::setPositionZ(int z)
{
    pos_.setZ(z);
    updataModelMat();
}

void RPlane::setMargin(int top, int bottom, int left, int right)
{
    marginTop_ = top; marginBottom_ = bottom; marginLeft_ = left; marginRight_ = right;
}

void RPlane::setMargin(int value)
{
    marginTop_ = value; marginBottom_ = value; marginLeft_ = value; marginRight_ = value;
}

void RPlane::setPadding(int top, int bottom, int left, int right)
{
    paddingTop_ = top; paddingBottom_ = bottom; paddingLeft_ = left; paddingRight_ = right;
    updataModelMat();
}

void RPlane::setPadding(int value)
{
    paddingTop_ = value; paddingBottom_ = value; paddingLeft_ = value; paddingRight_ = value;
    updataModelMat();
}

void RPlane::setColorTexture(RColor color)
{
    R_RGBA rgba = color.rgba();
    setColorTexture(rgba);
}

void RPlane::setColorTexture(R_RGBA rgba)
{
    const unsigned char *colorData = reinterpret_cast<unsigned char*>(&rgba);
    RTexture colorTex;
    colorTex.rename("ColorTexture");
    colorTex.setTexFilter(RTexture::Nearest);
    colorTex.generate(1, 1, 4, colorData, 4);
    texture_ = colorTex;
}

void RPlane::setColorTexture(unsigned r, unsigned g, unsigned b, unsigned a)
{
    RColor color(r, g, b, a);
    setColorTexture(color.rgba());
}

void RPlane::setTexture(const RImage &image)
{
    texture_.generate(image);
}

void RPlane::setTexture(const RTexture &texture)
{
    texture_ = texture;
}

void RPlane::setSizeMode(RPlane::SizeMode mode)
{
    sizeMode_ = mode;
}

void RPlane::setAlignment(RPlane::Alignment hAlign, RPlane::Alignment vAlign)
{
    hAlign_ = hAlign; vAlign_ = vAlign;
}

void RPlane::rotateX(float value)
{
    rotateMat_ = Rglm::rotate(RMatrix4(1), value, {1.0f, 0.0f, 0.0f});
    updataModelMat();
}

void RPlane::rotateY(float value)
{
    rotateMat_ = Rglm::rotate(RMatrix4(1), value, {0.0f, 1.0f, 0.0f});
    updataModelMat();
}

void RPlane::rotateZ(float value)
{
    rotateMat_ = Rglm::rotate(RMatrix4(1), value, {0.0f, 0.0f, 1.0f});
    updataModelMat();
}

void RPlane::render()
{
    glBindVertexArray(planeVAO);
    shaders_.use();

    if(dirty_) updataModelMatNow();
    texture_.bind();

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void RPlane::flipH()
{
    flipH_ = true;
    updataModelMat();
}

void RPlane::flipV()
{
    flipV_ = true;
    updataModelMat();
}

#ifdef R_DEBUG
void RPlane::displayLineBox(const RMatrix4 &projection, const RMatrix4 &view)
{
    static UniformLocation posLoc;
    static UniformLocation sizeLoc;

    if(!lineBoxsProgram.isValid())
    {
        RShader lineBoxV(":/shader/LineBox.vert", ShaderType::VertexShader, "LineBoxShaderVert");
        RShader lineBoxF(":/shader/LineBox.frag", ShaderType::FragmentShader, "LineBoxShaderFrag");
        RShader lineBoxG(":/shader/LineBox.geom", ShaderType::GeometryShader, "LineBoxShaderGeom");
        lineBoxsProgram.rename("LineBoxProgram");
        lineBoxsProgram.attachShader(lineBoxV);
        lineBoxsProgram.attachShader(lineBoxF);
        lineBoxsProgram.attachShader(lineBoxG);
        lineBoxsProgram.linkProgram();
        glGenVertexArrays(1, &lineBoxVAO);
        lineBoxsProgram.use();
        posLoc = lineBoxsProgram.getUniformLocation("aPos");
        sizeLoc = lineBoxsProgram.getUniformLocation("aSize");
    }

    glBindVertexArray(lineBoxVAO);
    lineBoxsProgram.use();

    RVector4 vec[3];

    float lineBoxs[3][2];
    //外边距框
    vec[0].x = outerWidth();
    vec[0].y = outerHeight();
    vec[0].z = pos_.z();
    vec[0].w = 1.0;
    vec[0] = projection * vec[0];
    lineBoxs[0][0] = vec[0].x - -1.0f;
    lineBoxs[0][1] = vec[0].y - -1.0f;
    //实际边框
    vec[0].x = width_;
    vec[0].y = height_;
    vec[0].z = pos_.z();
    vec[0].w = 1.0;
    vec[0] = projection * vec[0];
    lineBoxs[1][0] = vec[0].x - -1.0f;
    lineBoxs[1][1] = vec[0].y - -1.0f;
    //内边距框
    vec[0].x = innerWidth();
    vec[0].y = innerHeight();
    vec[0].z = pos_.z();
    vec[0].w = 1.0;
    vec[0] = projection * vec[0];
    lineBoxs[2][0] = vec[0].x - -1.0f;
    lineBoxs[2][1] = vec[0].y - -1.0f;

    vec[0] = {pos_.x()-marginLeft_, pos_.y()-marginBottom_, pos_.z(), 1.0f};
    vec[1] = {pos_.x(), pos_.y(), pos_.z(), 1.0f};
    vec[2] = {pos_.x()+paddingLeft_, pos_.y()+paddingLeft_, pos_.z(), 1.0f};

    vec[0] = projection * view * vec[0];
    vec[1] = projection * view * vec[1];
    vec[2] = projection * view * vec[2];
    lineBoxsProgram.setUniform(posLoc, 4, &vec[0].x, 3);
    lineBoxsProgram.setUniform(sizeLoc, 2, &lineBoxs[0][0], 3);
    //平面的三个线框
    glDrawArraysInstanced(GL_POINTS, 0, 1, 3);

    lineBoxsProgram.nonuse();
}

void RPlane::displayLineBox(int left, int right, int buttom, int top, RPoint pos)
{
    RMatrix4 projection = Rglm::ortho(left*1.0f, right*1.0f, buttom*1.0f, top*1.0f);
    RMatrix4 view = Rglm::translate(RMatrix4(1), { pos.x(), pos.y(), 0 });
    displayLineBox(projection, view);
}

void RPlane::updataModelMat()
{
    dirty_ = true;
}

void RPlane::updataModelMatNow()
{
    modelMat_ = RMatrix4(1);
    modelMat_ = Rglm::translate(modelMat_, {pos_.x()+width_/2.0f,
                                            pos_.y()+height_/2.0f, pos_.z()});
    modelMat_ *= rotateMat_;
    modelMat_ = Rglm::scale(modelMat_, {innerWidth(), innerHeight(), 1.0f});

    if(flipH_)
    {
        modelMat_[0][0] *= -1;
        modelMat_[0][1] *= -1;
        modelMat_[0][2] *= -1;
        modelMat_[0][3] *= -1;
    }
    if(flipV_)
    {
        modelMat_[1][0] *= -1;
        modelMat_[1][1] *= -1;
        modelMat_[1][2] *= -1;
        modelMat_[1][3] *= -1;
    }
    shaders_.setUniformMatrix(modelLoc, 4, Rglm::value_ptr(modelMat_));
    dirty_ = false;
}
#endif

RShaderProgram RPlane::lineBoxsProgram;
GLuint RPlane::lineBoxVAO;
RShaderProgram RPlane::planeSProgram;
GLuint RPlane::planeVAO, RPlane::planeVBO;
RTexture RPlane::whiteTex;
UniformLocation RPlane::modelLoc;
UniformLocation RPlane::viewLoc;
UniformLocation RPlane::projectionLoc;
int RPlane::count = 0;
