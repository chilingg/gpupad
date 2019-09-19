#include "RObject.h"

RObject::RObject(int width, int height):
#ifndef RO_NO_DEBUGE
    allocationed(false),
#endif
    _pos(0.0f, 0.0f),
    velocity(0.0f, 0.0f),
    color(1.0f),
    _width(width),
    _height(height)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

RObject::~RObject()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

bool RObject::touchSide(const RObject &platform, RVolume::Side side, int extend) const
{
    bool b = false;
    switch(side)
    {
    case RVolume::Top:
        b = volume().checkCollision(platform.volume(), true, false, extend) && volume().containsAxisY(platform.volume().top());
        break;
    case RVolume::Bottom:
        b = volume().checkCollision(platform.volume(), true, false, extend) && volume().containsAxisY(platform.volume().bottom());
        break;
    case RVolume::Left:
        b = volume().checkCollision(platform.volume(), false, true, extend) && volume().containsAxisX(platform.volume().left());
        break;
    case RVolume::Right:
        b = volume().checkCollision(platform.volume(), false, true, extend) && volume().containsAxisX(platform.volume().right());
        break;
    default:
        break;
    }
    return b;
}

void RObject::displayVolume(const glm::mat4 &projection, const glm::mat4 &view)
{
    if(!volumeShader)
    {
        const char *vTexCode =
                "#version 330 core\n"

                "uniform vec2 aPos;\n"
                "uniform float aWidth;\n"
                "uniform float aHeight;\n"

                "out vec2 pos;\n"
                "out float width;\n"
                "out float height;\n"

                "void main(void)\n"
                "{\n"
                    "pos = aPos;\n"
                    "width = aWidth;\n"
                    "height = aHeight;\n"
                "}";
        const char *vFragCode =
                "#version 330 core\n"

                "void main(void)\n"
                "{\n"
                    "gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
                "}";
        const char *vGeomCode =
                "#version 330 core\n"

                "layout (points) in;\n"
                "layout (line_strip, max_vertices = 5) out;\n"

                "in vec2 pos[];\n"
                "in float width[];\n"
                "in float height[];\n"

                "void main(void)\n"
                "{\n"
                    "vec4 position = vec4(pos[0], 0.0, 1.0);\n"

                    "gl_Position = position;\n"
                    "EmitVertex();\n"
                    "gl_Position = position + vec4(0.0, height[0], 0.0, 0.0);\n"
                    "EmitVertex();\n"
                    "gl_Position = position + vec4(width[0], height[0], 0.0, 0.0);\n"
                    "EmitVertex();\n"
                    "gl_Position = position + vec4(width[0], 0.0, 0.0, 0.0);\n"
                    "EmitVertex();\n"
                    "gl_Position = position;\n"
                    "EmitVertex();\n"

                    "EndPrimitive();\n"
                "}";

        RShader vTex;
        RShader vFrag;
        RShader vGeom;
        vTex.compileShaderCode(vTexCode, RShader::VERTEX_SHADER);
        vFrag.compileShaderCode(vFragCode, RShader::FRAGMENT_SHADER);
        vGeom.compileShaderCode(vGeomCode, RShader::GEOMETRY_SHADER);

        //全局存在 不需要delete
        volumeShader = new RShaderProgram();
        volumeShader->attachShader(vTex);
        volumeShader->attachShader(vFrag);
        volumeShader->attachShader(vGeom);
        volumeShader->linkProgram();

        glGenVertexArrays(1, &vVAO);
    }

    glBindVertexArray(vVAO);
    volumeShader->use();

    glm::mat4 model(1.0f);
    model = glm::translate(model, {_pos, 0.0f});
    glm::vec4 p(volume().getPos(), 0.0f, 1.0f);
    p = projection * view * p;
    volumeShader->setUniform2F("aPos", p.x, p.y);
    p.x = volume().widthF();
    p.y = volume().heightF();
    p = projection * p;
    volumeShader->setUniform1F("aWidth", p.x - -1.0f);
    volumeShader->setUniform1F("aHeight", p.y - -1.0f);
    glDrawArrays(GL_POINTS, 0, 1);
}

bool RObject::moveCollision(glm::vec2 &velocity, const RObject &platform)
{
    if(platform.checkCollision(*this))
    {
        float intervalY = velocity.y > 0.0f ? 1.0f : -1.0f;
        float velocityY = velocity.y;

        while(velocity.y <= -0.001f || velocity.y >= 0.001f)
        {
            _pos.y -= intervalY;
            velocity.y  -= intervalY;
            if(!platform.checkCollision(*this))
            {
                return true;
            }
        }
        float intervalX = velocity.x > 0.0f ? 1.0f : -1.0f;
        while(velocity.x <= -0.001f || velocity.x >= 0.001f)
        {
            _pos.x -= intervalX;
            velocity.x -= intervalX;
            if(!platform.checkCollision(*this))
            {
                break;
            }
        }
        if(platform.checkCollision(*this))
        {
            RDebug() << "Object stuck in platform!" << _pos;
            return false;
        }
        else
        {
            velocity.y = velocityY;
            _pos.y  += velocityY;
            if(platform.checkCollision(*this))
                RDebug() << "Y axis error!";
            return true;
        }
    }

    return false;
}

void RObject::render(RShaderProgram *shader)
{
#ifndef RO_NO_DEBUGE
    if(!allocationed)
    {
        printErro("Unallocation content!");
        RDebug() << VAO;
    }
#endif

    glBindVertexArray(VAO);
    shader->use();
    renderControl(shader);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

float *RObject::getPlantArray()
{
    float *plant = new float[12]{
            0.0f, 0.0f,//左下
            0.0f, static_cast<float>(_height),//左上
            static_cast<float>(_width), static_cast<float>(_height),//右上
            static_cast<float>(_width), static_cast<float>(_height),//右上
            static_cast<float>(_width), 0.0f,//右下
            0.0f, 0.0f,//左下
    };

    return plant;
}

void RObject::renderControl(RShaderProgram *shader)
{
    shader->setUniform4F("color", color);

    glm::mat4 model(1.0f);
    model = glm::translate(model, {_pos, 0.0f});
    if(_flipH)
    {
        model = glm::translate(model, {_width, 0.0f, 0.0f});
        model[0][0] = -model[0][0];
    }
    if(_flipV)
    {
        model = glm::translate(model, {0.0f, _height, 0.0f});
        model[1][1] = -model[1][1];
    }

    shader->setUniformMatrix4fv("model", glm::value_ptr(model));
}

void RObject::allocation()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    float *plant = getPlantArray();
    glBufferData(GL_ARRAY_BUFFER, sizeof(*plant)*12, plant, GL_STATIC_DRAW);
    delete [] plant;

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

#ifndef RO_NO_DEBUGE
    allocationed = true;
#endif
}

RShaderProgram *RObject::volumeShader(nullptr);
unsigned RObject::vVAO(0);
