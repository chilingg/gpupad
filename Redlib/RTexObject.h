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
    void addTexture(std::string name, const std::string &path);
    void addTexture(std::string name, const RImage &image);
    bool setCurrentTexture(std::string name);

protected:
    float* getPlantArray() override;
    void renderControl(RShaderProgram *shader) override;

    std::map<std::string, RTexture> textures;
    std::string currentTex;
};

inline void RTexObject::addTexture(std::string name, const std::string &path)
{
    RImage image(path, true);
    addTexture(name, image);
}

inline void RTexObject::addTexture(std::string name, const RImage &image)
{
    auto pr = textures.emplace(name, image);
    if(pr.second)
    {
        textures.erase(pr.first);
        textures.emplace(name, image);
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
