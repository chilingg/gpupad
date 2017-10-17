#ifndef VALIDATESOURCE_H
#define VALIDATESOURCE_H

#include "RenderTask.h"
#include "session/Item.h"
#include "editors/SourceEditor.h"

class GLShader;
class ScriptEngine;

class ValidateSource : public RenderTask
{
    Q_OBJECT
public:
    explicit ValidateSource(QObject *parent = nullptr);
    ~ValidateSource();

    void setSource(QString fileName, SourceEditor::SourceType sourceType);
    QSet<ItemId> usedItems() const override;

private:
    void prepare(bool itemsChanged, bool manualEvaluation) override;
    void render() override;
    void finish(bool steadyEvaluation) override;
    void release() override;

    QScopedPointer<GLShader> mNewShader;
    QScopedPointer<GLShader> mShader;
    QString mFileName;
    SourceEditor::SourceType mSourceType{ };

    QString mScriptSource;
    QScopedPointer<ScriptEngine> mScriptEngine;
};

#endif // VALIDATESOURCE_H
