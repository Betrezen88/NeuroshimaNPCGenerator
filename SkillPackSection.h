#ifndef SKILLPACKSECTION_H
#define SKILLPACKSECTION_H

#include "Section.h"

#include <QMap>

class QCheckBox;
class QSpinBox;

class SkillPackSection : public Section
{
public:
    explicit SkillPackSection(const QString& name, const QJsonDocument& json, QWidget *parent = nullptr);

private slots:
    void packBought(const bool& bougth);

private:
    QGroupBox* create(const QString& name);
    void changeSkillsValue(const int& value);

private:
    QMap<QString, QSpinBox*> m_skills;
    QCheckBox* m_packBought;
};

#endif // SKILLPACKSECTION_H
