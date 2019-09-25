#ifndef RTEXOBJECT_H
#define RTEXOBJECT_H

#include <RObject.h>
#include <RTexture.h>
#include <map>

class RTexObject : public RObject
{
public:
    enum TextureSizePattern {
        Length,
        Auto,
        Cover,
        Contain
    };
    enum Alignment {
        Left,
        Right,
        Mind,
        Top,
        Bottom
    };

    RTexObject(int width, int height);
    RTexObject(int width, int height, const char *path);

    void render(RShaderProgram *shader) override;

    void addTexture(std::string name, const std::string &path);
    void addTexture(std::string name, const RImage &image);
    bool setCurrentTexture(std::string name);
    void setTextureSizePattern(TextureSizePattern pattern);
    void setAlignment(Alignment vAlign, Alignment hAlign);

protected:
    static unsigned texVAO, texVBO;

    void allocation() override;
    void renderControl(RShaderProgram *shader) override;
    void updataSizeMat();

    std::map<std::string, RTexture> textures;
    std::string currentTex;

    TextureSizePattern _sizePattern = Auto;
    Alignment _vAlign = Bottom;
    Alignment _hAlign = Left;
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

inline void RTexObject::setTextureSizePattern(TextureSizePattern pattern)
{
    _sizePattern = pattern;
}

inline void RTexObject::setAlignment(Alignment vAlign, Alignment hAlign)
{
    _vAlign = vAlign;
    _hAlign = hAlign;
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
