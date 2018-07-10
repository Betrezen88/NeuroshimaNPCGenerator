#ifndef SKILLPACKSECTION_H
#define SKILLPACKSECTION_H

#include "Section.h"

class SkillPackSection : public Section
{
public:
    explicit SkillPackSection(const QString& name, const QJsonDocument& json, QWidget *parent = nullptr);

private:
    QGroupBox* create(const QString& name);
};

#endif // SKILLPACKSECTION_H
