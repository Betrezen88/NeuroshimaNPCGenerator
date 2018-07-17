#ifndef SKILLSSECTION_H
#define SKILLSSECTION_H

#include "Section.h"
#include "SkillPackSection.h"

#include <QGridLayout>
#include <QMap>

class SkillsSection : public Section
{
public:
    SkillsSection(const QString& name, const QJsonDocument& json, QWidget* parent=0);

private:
    QGroupBox* create(const QString& name);
    QGroupBox* createAttribute(const QJsonValue &value);
    void addAttributeGroupBox(QGridLayout* layout);

private:
    QMap<QString, SkillPackSection*> m_skillPacks;
};

#endif // SKILLSSECTION_H
