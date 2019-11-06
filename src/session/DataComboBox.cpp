#include "DataComboBox.h"

DataComboBox::DataComboBox(QWidget *parent) : QComboBox(parent)
{
    using Overload = void(QComboBox::*)(int);
    connect(this, static_cast<Overload>(&QComboBox::currentIndexChanged),
        [this]() { emit currentDataChanged(currentData()); });
}

void DataComboBox::setCurrentData(QVariant data)
{
    auto index = findData(data);
    if (index >= 0)
        setCurrentIndex(index);
}

QSize DataComboBox::minimumSizeHint() const
{
    return QSize(50, QComboBox::minimumSizeHint().height());
}