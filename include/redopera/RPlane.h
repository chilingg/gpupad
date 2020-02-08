#ifndef RPLANE_H
#define RPLANE_H

#include "RArea.h"
#include "rsc/RTexture.h"
#include "rsc/RShaderProgram.h"

namespace Redopera {

class RPlane : public RArea
{
    struct RenderTool
    {
        RShaderProgram &shaders;
        GLuint modelLoc;
        GLuint vao;
        GLuint edgingVAO;
        GLuint edgingLoc;
    };

    struct ModelMat
    {
        glm::mat4 tran, rotate, scale;
    };

public:
    static void setPlaneShadersAsThread(const RShaderProgram &shaders, GLuint modelLoc);
    static void setPlaneShadersAsThread(const RShaderProgram &shaders, GLuint modelLoc, GLuint edgingLoc);
    static const RShaderProgram& planeShader();

    RPlane();
    RPlane(int width, int height, int x, int y, int z = 0, const RTexture &tex = defaultTexture());
    RPlane(int width, int height, const RPoint &pos, const RTexture &tex = defaultTexture());
    RPlane(const RSize &size, const RPoint &pos, const RTexture &tex = defaultTexture());
    explicit RPlane(const RRect &rect, int z = 0, const RTexture &tex = defaultTexture());
    explicit RPlane(const Format &format, const RTexture &tex = defaultTexture());
    RPlane(const RPlane &plane);
    RPlane(const RPlane &&plane);
    ~RPlane() override = default;

    const glm::mat4& modelMat() const;
    const ModelMat& modelMats() const;
    const RTexture& texture() const;

    void setColorTexture(const RColor &color);
    void setColorTexture(R_RGBA rgba);
    void setColorTexture(unsigned r, unsigned g, unsigned b, unsigned a = 0xffu);
    void setTexture(const RImage &img);
    void setTexture(const RTexture &tex);

    void update();
    void render();
    void render(const RShaderProgram &shaders, GLuint mLoc);
    void edging();
    void edging(const RShaderProgram &shaders, GLuint mLoc, GLuint eLoc);

protected:
    static const RenderTool planeRenderTool();

    virtual void renderControl(const RShaderProgram &shaders, GLuint mLoc);

private:
    static const RTexture& defaultTexture();

    thread_local static RShaderProgram tPlaneShaders;
    thread_local static GLuint modelLoc;
    thread_local static GLuint edgingLoc;

    ModelMat mats_;
    glm::mat4 model_;
    RTexture texture_;
};

} // Redopera

#endif // RPLANE_H
