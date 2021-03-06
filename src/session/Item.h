#ifndef ITEM_H
#define ITEM_H

#include "ItemEnums.h"
#include <array>
#include <QList>
#include <QVariant>
#include <QOpenGLTexture>

using ItemId = int;
struct Item;

struct Item
{
    using Type = ItemEnums::ItemType;

    virtual ~Item() = default;

    ItemId id{ };
    Type type{ };
    Item *parent{ };
    QList<Item*> items;
    QString name{ };
};

struct FileItem : Item
{
    QString fileName;
};

struct Group : Item
{
    bool inlineScope{ };
};

struct Column : Item
{
    using DataType = ItemEnums::ColumnDataType;

    DataType dataType{ DataType::Float };
    int count{ 1 };
    int padding{ 0 };
};

struct Buffer : FileItem
{
    int offset{ 0 };
    int rowCount{ 1 };
};

struct Texture : FileItem
{
    using Target = QOpenGLTexture::Target;
    using Format = QOpenGLTexture::TextureFormat;

    Target target{ QOpenGLTexture::Target2D };
    Format format{ QOpenGLTexture::RGBA8_UNorm };
    int width{ 256 };
    int height{ 256 };
    int depth{ 1 };
    int layers{ 1 };
    int samples{ 1 };
    bool flipY{ };
};

struct Image : FileItem
{
    using CubeMapFace = QOpenGLTexture::CubeMapFace;

    int level{ };
    int layer{ };
    CubeMapFace face{ };
};

struct Program : Item
{
};

struct Shader : FileItem
{
    using ShaderType = ItemEnums::ShaderType;

    ShaderType shaderType{ ShaderType::Vertex };
};

struct Binding : Item
{
    using BindingType = ItemEnums::BindingType;
    using Filter = QOpenGLTexture::Filter;
    using WrapMode = QOpenGLTexture::WrapMode;
    using ComparisonFunc = ItemEnums::ComparisonFunc;
    using Editor = ItemEnums::BindingEditor;

    BindingType bindingType{ };
    Editor editor{ };
    QStringList values{ "0" };
    ItemId textureId{ };
    ItemId bufferId{ };
    int level{ };
    bool layered{ };
    int layer{ };
    Filter minFilter{ QOpenGLTexture::Nearest };
    Filter magFilter{ QOpenGLTexture::Nearest };
    WrapMode wrapModeX{ QOpenGLTexture::Repeat };
    WrapMode wrapModeY{ QOpenGLTexture::Repeat };
    WrapMode wrapModeZ{ QOpenGLTexture::Repeat };
    QColor borderColor{ Qt::black };
    ComparisonFunc comparisonFunc{ };
    QString subroutine;
};

struct Stream : Item
{
};

struct Attribute : Item
{
    ItemId bufferId{ };
    ItemId columnId{ };
    bool normalize{ };
    int divisor{ };
};

struct Target : Item
{
    using FrontFace = ItemEnums::FrontFace;
    using CullMode = ItemEnums::CullMode;
    using LogicOperation = ItemEnums::LogicOperation;

    FrontFace frontFace{ };
    CullMode cullMode{ };
    LogicOperation logicOperation{ };
    QColor blendConstant{ Qt::white };
};

struct Attachment : Item
{
    using BlendEquation = ItemEnums::BlendEquation;
    using BlendFactor = ItemEnums::BlendFactor;
    using ComparisonFunc = ItemEnums::ComparisonFunc;
    using StencilOperation = ItemEnums2::StencilOperation;

    ItemId textureId{ };
    int level{ };
    bool layered{ };
    int layer{ };

    BlendEquation blendColorEq{ BlendEquation::Add };
    BlendFactor blendColorSource{ BlendFactor::One };
    BlendFactor blendColorDest{ BlendFactor::Zero };
    BlendEquation blendAlphaEq{ BlendEquation::Add };
    BlendFactor blendAlphaSource{ BlendFactor::One };
    BlendFactor blendAlphaDest{ BlendFactor::Zero };
    unsigned int colorWriteMask{ 0xF };

    ComparisonFunc depthComparisonFunc{ ComparisonFunc::Less };
    double depthOffsetFactor{ };
    double depthOffsetUnits{ };
    bool depthClamp{ };
    bool depthWrite{ true };

    ComparisonFunc stencilFrontComparisonFunc{ ComparisonFunc::Always };
    int stencilFrontReference{ };
    unsigned int stencilFrontReadMask{ 0xFF };
    StencilOperation stencilFrontFailOp{ StencilOperation::Keep };
    StencilOperation stencilFrontDepthFailOp{ StencilOperation::Keep };
    StencilOperation stencilFrontDepthPassOp{ StencilOperation::Keep };
    unsigned int stencilFrontWriteMask{ 0xFF };

    ComparisonFunc stencilBackComparisonFunc{ ComparisonFunc::Always };
    int stencilBackReference{ };
    unsigned int stencilBackReadMask{ 0xFF };
    StencilOperation stencilBackFailOp{ StencilOperation::Keep };
    StencilOperation stencilBackDepthFailOp{ StencilOperation::Keep };
    StencilOperation stencilBackDepthPassOp{ StencilOperation::Keep };
    unsigned int stencilBackWriteMask{ 0xFF };
};

struct Call : Item
{
    using CallType = ItemEnums2::CallType;
    using PrimitiveType = ItemEnums::PrimitiveType;
    using ExecuteOn = ItemEnums::ExecuteOn;

    ExecuteOn executeOn{ ExecuteOn::EveryEvaluation };
    bool checked{ true };
    CallType callType{ };
    ItemId programId{ };
    ItemId targetId{ };
    ItemId vertexStreamId{ };

    PrimitiveType primitiveType{ PrimitiveType::Triangles };
    QString count{ "3" };
    QString first{ "0" };

    ItemId indexBufferId{ };
    QString baseVertex{ "0" };

    QString instanceCount{ "1" };
    QString baseInstance{ "0" };

    ItemId indirectBufferId{ };
    QString drawCount{ "1" };

    QString patchVertices{ "3" };

    QString workGroupsX{ "1" };
    QString workGroupsY{ "1" };
    QString workGroupsZ{ "1" };

    ItemId textureId{ };
    ItemId fromTextureId{ };
    QColor clearColor{ Qt::black };
    double clearDepth{ 1.0 };
    int clearStencil{ };

    ItemId bufferId{ };
};

struct Script : FileItem
{
    using ExecuteOn = ItemEnums::ExecuteOn;

    ExecuteOn executeOn{ ExecuteOn::ManualEvaluation };
    QString expression;
};

#endif // ITEM_H
