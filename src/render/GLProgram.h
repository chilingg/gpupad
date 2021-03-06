#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include "GLShader.h"
#include <map>

class GLTexture;
class GLBuffer;
class ScriptEngine;

struct GLUniformBinding
{
    ItemId bindingItemId;
    QString name;
    Binding::BindingType type;
    QStringList values;
    bool transpose;
};

struct GLSamplerBinding
{
    ItemId bindingItemId;
    QString name;
    GLTexture *texture;
    Binding::Filter minFilter;
    Binding::Filter magFilter;
    Binding::WrapMode wrapModeX;
    Binding::WrapMode wrapModeY;
    Binding::WrapMode wrapModeZ;
    QColor borderColor;
    Binding::ComparisonFunc comparisonFunc;
};

struct GLImageBinding
{
    ItemId bindingItemId;
    QString name;
    GLTexture *texture;
    int level;
    bool layered;
    int layer;
    GLenum access;
};

struct GLBufferBinding
{
    ItemId bindingItemId;
    QString name;
    GLBuffer *buffer;
};

struct GLSubroutineBinding
{
    ItemId bindingItemId;
    QString name;
    QString subroutine;
    Shader::ShaderType type;
};

class GLProgram
{
public:
    explicit GLProgram(const Program &program);
    bool operator==(const GLProgram &rhs) const;

    bool bind(MessagePtrSet *callMessages);
    void unbind(ItemId callItemId);
    int getAttributeLocation(const QString &name) const;
    bool apply(const GLUniformBinding &binding, ScriptEngine &scriptEngine);
    bool apply(const GLSamplerBinding &binding, int unit);
    bool apply(const GLImageBinding &binding, int unit);
    bool apply(const GLBufferBinding &binding, int unit);
    bool apply(const GLSubroutineBinding &subroutine);
    void reapplySubroutines();
    void checkUniforms();
    const QSet<ItemId> &usedItems() const { return mUsedItems; }

private:
    struct SubroutineUniform
    {
        QString name;
        QList<QString> subroutines;
        ItemId bindingItemId;
        QString boundSubroutine;
    };

    bool link();
    QString getUniformBaseName(const QString &name) const;
    void uniformSet(const QString &name);
    void uniformBlockSet(const QString &name);

    ItemId mItemId{ };
    QSet<ItemId> mUsedItems;
    MessagePtrSet mLinkMessages;
    std::vector<GLShader> mShaders;
    QMap<Shader::ShaderType, QList<SubroutineUniform>> mSubroutineUniforms;
    QMap<QString, GLenum> mUniformDataTypes;
    GLObject mProgramObject;
    bool mFailed{ };
    MessagePtrSet *mCallMessages{ };
    std::map<QString, bool> mUniformsSet;
    std::map<QString, bool> mUniformBlocksSet;
    mutable std::map<QString, bool> mAttributesSet;
};

#endif // GLPROGRAM_H
