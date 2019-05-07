#ifndef NPCCUSTOMSKILLPACK_H
#define NPCCUSTOMSKILLPACK_H

#include "NPCAbstractSkillPack.h"
#include "SkillSpinBox.h"

#include <QComboBox>
#include <QVector>

class NPCCustomSkillPack : public NPCAbstractSkillPack
{
public:
    NPCCustomSkillPack(const QString &name, const QStringList &specs, QWidget *parent = nullptr);

    void addSkills(const QStringList &skills);
    bool hasSkill(const QString &name) const;

private slots:
    void onBougth(const bool &checked);
    void enableBougthCheckbox();

private:
};

#endif // NPCCUSTOMSKILLPACK_H
