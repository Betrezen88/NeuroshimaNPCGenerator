#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include "Section.h"

#include "../Widget/AttributeWidget.h"

#include <QLabel>
#include <QHash>

class Attributes : public Section
{
public:
    Attributes(const QJsonArray &json, QWidget *parent = nullptr);
    ~Attributes() = default;

private:
    QVBoxLayout *createLayout(const QJsonArray &json);


private:
    QHash<QString, AttributeWidget*> m_attributes;
};

#endif // ATTRIBUTES_H
