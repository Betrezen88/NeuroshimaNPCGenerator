#ifndef NPCSKILLPACK_H
#define NPCSKILLPACK_H

#include "NPCAbstractSkillPack.h"

class NPCSkillPack : public NPCAbstractSkillPack
{
public:
    NPCSkillPack(const QString &name, const QStringList &specs, QWidget *parent = nullptr);

    void addSkills(const QStringList &skills);
    bool hasSkill(const QString &name) const;

private slots:
    void onBougth(const bool &checked);
    void enableBougthCheckbox();

private:
};

#endif // NPCSKILLPACK_H
