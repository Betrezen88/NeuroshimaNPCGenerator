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
    void createLabelRow(QGridLayout *layout);
    void createAttributeRows(QGridLayout *layout);
    QWidget* createWidget(const int& index);

private:
};

#endif // ATTRIBUTESECTION_H
