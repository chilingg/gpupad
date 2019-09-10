#ifndef IEDITOR_H
#define IEDITOR_H

#include "EditActions.h"
#include "SourceType.h"
#include <QList>
#include <QMetaObject>

class IEditor
{
public:
    virtual ~IEditor() = default;
    virtual QList<QMetaObject::Connection> connectEditActions(
        const EditActions &actions) = 0;
    virtual QString fileName() const = 0;
    virtual void setFileName(QString fileName) = 0;
    virtual bool load() = 0;
    virtual bool save() = 0;
    virtual int tabifyGroup() = 0;
    virtual SourceType sourceType() const { return SourceType::None; }
    virtual void setSourceType(SourceType) { }
};

#endif // IEDITOR_H