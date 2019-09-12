#ifndef RTEXOBJECT_H
#define RTEXOBJECT_H

#include <RObject.h>
#include <RTexture.h>
#include <map>

class RTexObject : public RObject
{
public:
    RTexObject(int width, int height);
    RTexObject(int width, int height, const char *path);

    void allocation() override;
    void addTexture(std::string name, std::string path);
    bool setCurrentTexture(std::string name);

protected:
    float* getPlantArray(int width, int height) override;
    void renderControl(RShaderProgram *shader) override;

    std::map<std::string, RTexture> textures;
    std::string currentTex;
};

inline void RTexObject::addTexture(std::string name, std::string path)
{
    auto pr = textures.emplace(name, path);
    if(pr.second)
    {
        textures.erase(pr.first);
        textures.emplace(name, path);
    }
}

inline bool RTexObject::setCurrentTexture(std::string name)
{
    auto it = textures.find(name);
    if(it != textures.end())
    {
        currentTex = name;
        return true;
    }
    return false;
}

#endif // RTEXOBJECT_H
