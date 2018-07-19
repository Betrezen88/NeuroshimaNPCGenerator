#ifndef ATTRIBUTESECTION_H
#define ATTRIBUTESECTION_H

#include "Section.h"

#include <QVBoxLayout>

class AttributeSection : public Section
{
public:
    AttributeSection(const QString& name, const QJsonDocument& json, QWidget* parent=0);

private:
    QGroupBox* create(const QString &name);
    QHBoxLayout *createLabelRow();
    void createAttributeRows(QVBoxLayout *layout);
    QWidget* createWidget(const int& index);

private:
    QVector<QWidget*> m_attributes;
};

#endif // ATTRIBUTESECTION_H
