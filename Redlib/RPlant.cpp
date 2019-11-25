#include "RPlant.h"

#include "RDebug.h"

void RPlant::setViewpro(int left, int right, int buttom, int top)
{
    plantSProgram.use();
    RMatrix4 projection = Rglm::ortho(left, right, buttom, top);
    plantSProgram.setUniformMatrix(projectionLoc, 4, Rglm::value_ptr(projection));
    plantSProgram.nonuse();
}

void RPlant::setViewproMove(int x, int y, int z)
{
    plantSProgram.use();
    RMatrix4 view = Rglm::translate(RMatrix4(1), {x, y, z});
    plantSProgram.setUniformMatrix(viewLoc, 4, Rglm::value_ptr(view));
    plantSProgram.nonuse();
}

RPlant::RPlant(): RPlant("Plant", 32, 32)
{

}

RPlant::RPlant(const RPlant &plant):
    name_(plant.name_),
    width_(plant.width_),
    height_(plant.height_),
    pos_(plant.pos_),
    rotateMat_(plant.rotateMat_),
    modelMat_(plant.modelMat_),
    texture_(plant.texture_),
    flipH_(plant.flipH_),
    flipV_(plant.flipV_),
    marginTop_(plant.marginTop_),
    marginBottom_(plant.marginBottom_),
    marginLeft_(plant.marginLeft_),
    marginRight_(plant.marginRight_),
    paddingTop_(plant.paddingTop_),
    paddingBottom_(plant.paddingBottom_),
    paddingLeft_(plant.paddingLeft_),
    paddingRight_(plant.paddingRight_),
    sizeMode_(plant.sizeMode_),
    vAlign_(plant.vAlign_),
    hAlign_(plant.hAlign_)
{
    ++count;
}

RPlant::RPlant(const std::string &name, int width, int height, RPoint pos):
    name_(name),
    width_(width),
    height_(height),
    pos_(pos),
    rotateMat_(1.0f),
    modelMat_(1.0f),
    texture_()
{
    if(++count == 1)
    {
        glGenVertexArrays(1, &plantVAO);
        glGenBuffers(1, &plantVBO);
        glBindVertexArray(plantVAO);
        glBindBuffer(GL_ARRAY_BUFFER, plantVBO);

        float plant[24]{
                0.5f,-0.5f, 0.0f, 1.0f, 0.0f,//右下
                0.5f, 0.5f, 0.0f, 0.0f, 1.0f,//右上
               -0.5f,-0.0f, 0.0f, 0.0f, 0.0f,//左下
               -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,//左上
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(plant), plant, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), offsetBuffer(3*sizeof(float)));
        glBindVertexArray(0);

        plantSProgram.rename("DefaultPlanProgram");
        plantSProgram.attachShader(":/shader/Vertex.vert", ShaderType::VertexShader);
        plantSProgram.attachShader(":/shader/Fragment.frag", ShaderType::FragmentShader);
        plantSProgram.linkProgram();

        const unsigned char white = 0xff;
        whiteTex.rename("DefaultTexture");
        whiteTex.setTexFilter(RTexture::Nearest);
        whiteTex.generate(1, 1, 1, &white);

        plantSProgram.use();
        modelLoc = plantSProgram.getUniformLocation("model");
        viewLoc = plantSProgram.getUniformLocation("view");
        projectionLoc = plantSProgram.getUniformLocation("projection");
        //setViewpro(0, 900, 0, 1600);
        setViewpro(-1, 1, -1, 1);
        setViewproMove(0, 0);
    }

    texture_ = whiteTex;
}

RPlant::~RPlant()
{
    if(--count == 0)
    {
        whiteTex.freeTexture();
        plantSProgram.freeShaderProgram();
        glDeleteBuffers(1, &plantVBO);
        glDeleteVertexArrays(1, &plantVAO);
#ifdef R_DEBUG
        if(lineBoxsProgram.isValid())
        {
            lineBoxsProgram.freeShaderProgram();
            glDeleteVertexArrays(1, &lineBoxVAO);
        }
#endif
    }
}

void RPlant::setSize(int width, int height)
{
    width_ = width; height_ = height;
    updataModelMat();
}

void RPlant::setSize(RSize size)
{
    width_ = size.width(); height_ = size.height();
    updataModelMat();
}

void RPlant::setPosition(int x, int y, int z)
{
    pos_ = RPoint(x, y, z);
    updataModelMat();
}

void RPlant::setPositionX(int x)
{
    pos_.setX(x);
    updataModelMat();
}

void RPlant::setPositionY(int y)
{
    pos_.setY(y);
    updataModelMat();
}

void RPlant::setPositionZ(int z)
{
    pos_.setZ(z);
    updataModelMat();
}

void RPlant::setMargin(int top, int bottom, int left, int right)
{
    marginTop_ = top; marginBottom_ = bottom; marginLeft_ = left; marginRight_ = right;
}

void RPlant::setMargin(int value)
{
    marginTop_ = value; marginBottom_ = value; marginLeft_ = value; marginRight_ = value;
}

void RPlant::setPadding(int top, int bottom, int left, int right)
{
    paddingTop_ = top; paddingBottom_ = bottom; paddingLeft_ = left; paddingRight_ = right;
    updataModelMat();
}

void RPlant::setPadding(int value)
{
    paddingTop_ = value; paddingBottom_ = value; paddingLeft_ = value; paddingRight_ = value;
    updataModelMat();
}

void RPlant::setTexture(const RImage &image)
{
    texture_.generate(image);
}

void RPlant::setTexture(const RTexture &texture)
{
    texture_ = texture;
}

void RPlant::setSizeMode(RPlant::SizeMode mode)
{
    sizeMode_ = mode;
}

void RPlant::setAlignment(RPlant::Alignment hAlign, RPlant::Alignment vAlign)
{
    hAlign_ = hAlign; vAlign_ = vAlign;
}

void RPlant::rotateX(float value)
{
    rotateMat_ = Rglm::rotate(rotateMat_, value, {1.0f, 0.0f, 0.0f});
    updataModelMat();
}

void RPlant::rotateY(float value)
{
    rotateMat_ = Rglm::rotate(rotateMat_, value, {0.0f, 0.0f, 1.0f});
    updataModelMat();
}

void RPlant::rotateZ(float value)
{
    rotateMat_ = Rglm::rotate(rotateMat_, value, {0.0f, 0.0f, 1.0f});
    updataModelMat();
}

void RPlant::render()
{
    glBindVertexArray(plantVAO);
    plantSProgram.use();

    plantSProgram.setUniformMatrix(modelLoc, 4, Rglm::value_ptr(modelMat_));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void RPlant::render(const RShaderProgram &program)
{
#ifdef R_DEBUG
    if(printError(program.isValid(), "Failed set Shader Rorgram in Plan, " + program.nameID() + " is invalid!"))
        return;
#endif

    glBindVertexArray(plantVAO);
    program.use();

    program.setUniformMatrix(program.getUniformLocation("model"), 4, Rglm::value_ptr(modelMat_));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void RPlant::flipH()
{
    modelMat_[0][0] *= -1;
    modelMat_[1][0] *= -1;
    modelMat_[2][0] *= -1;
    modelMat_[3][0] *= -1;
    flipH_ = true;
}

void RPlant::flipV()
{
    modelMat_[0][1] *= -1;
    modelMat_[1][1] *= -1;
    modelMat_[2][1] *= -1;
    modelMat_[3][1] *= -1;
    flipV_ = true;
}

#ifdef R_DEBUG
void RPlant::displayLineBox(const RMatrix4 &projection, const RMatrix4 &view)
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

    RVector4 p(pos_.x(), pos_.y(), pos_.z(), 1.0f);
    p = projection * view * p;
    lineBoxsProgram.setUniform(posLoc, p.x, p.y, p.z, p.w);

    float lineBoxs[2][3];
    p.x = outerWidth();
    p.y = outerHeight();
    p.z = pos_.z();
    p.w = 1.0;
    p = projection * p;
    lineBoxs[0][0] = p.x - -1.0f;
    lineBoxs[1][0] = p.y - -1.0f;
    p.x = width();
    p.y = height();
    p.z = pos_.z();
    p.w = 1.0;
    p = projection * p;
    lineBoxs[0][1] = p.x - -1.0f;
    lineBoxs[1][1] = p.y - -1.0f;
    p.x = innerWidth();
    p.y = innerHeight();
    p.z = pos_.z();
    p.w = 1.0;
    p = projection * p;
    lineBoxs[0][2] = p.x - -1.0f;
    lineBoxs[1][2] = p.y - -1.0f;
    lineBoxsProgram.setUniform(sizeLoc, 2, &lineBoxs[0][0], 3);
    //平面的三个线框
    glDrawArraysInstanced(GL_POINTS, 0, 1, 3);

    lineBoxsProgram.nonuse();
}

void RPlant::displayLineBox(int left, int right, int buttom, int top, RPoint pos)
{
    RMatrix4 projection = Rglm::ortho(left, right, buttom, top);
    RMatrix4 view = Rglm::translate(RMatrix4(1), { pos.x(), pos.y(), 0 });
    displayLineBox(projection, view);
}

void RPlant::updataModelMat()
{
    modelMat_ = RMatrix4(1);
    modelMat_ = translate(modelMat_, {pos_.x(), pos_.y(), pos_.z()});
    modelMat_ = rotateMat_ * modelMat_;
    if(flipH_) flipH();
    if(flipV_) flipV();
}
#endif

RShaderProgram RPlant::lineBoxsProgram;
GLuint RPlant::lineBoxVAO;
RShaderProgram RPlant::plantSProgram;
GLuint RPlant::plantVAO, RPlant::plantVBO;
RTexture RPlant::whiteTex;
UniformLocation RPlant::modelLoc;
UniformLocation RPlant::viewLoc;
UniformLocation RPlant::projectionLoc;
int RPlant::count = 0;
