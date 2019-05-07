#ifndef NPCSKILLPACK_H
#define NPCSKILLPACK_H

#include "NPCAbstractSkillPack.h"

class NPCSkillPack : public NPCAbstractSkillPack
{
public:
    NPCSkillPack(const QString &name, const QStringList &specs, QWidget *parent = nullptr);

    void addSkills(const QStringList &skills);
    bool hasSkill(const QString &name) const;
    QVector<QPair<QString, int>> skills();

private slots:
    void onBougth(const bool &checked);
    void enableBougthCheckbox();

private:
    int skillIndex(const QString &name) const;
};

#endif // NPCSKILLPACK_H
