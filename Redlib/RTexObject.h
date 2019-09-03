#ifndef RTEXOBJECT_H
#define RTEXOBJECT_H

#include <RObject.h>
#include <RTexture.h>

class RTexObject : public RObject
{
public:
    RTexObject(int widht, int height, const char *path);
    void allocation() override;

protected:
    float* getPlantArray(int widht, int height);
    void renderControl(RShaderProgram *shader) override;

    RTexture texture;
};

#endif // RTEXOBJECT_H
