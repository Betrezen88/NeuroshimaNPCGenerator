#ifndef NPCATTRIBUTE_H
#define NPCATTRIBUTE_H

#include <QString>
#include <QVector>

#include "NPCSkillPack.h"

class NPCAttribute
{
public:
    NPCAttribute();
    NPCAttribute(const QString &name, const int &value, const int &modificator);
    ~NPCAttribute() = default;

    QString name() const;
    int value() const;
    int modificator() const;
    NPCSkillPack* skillPack(const QString &name) const;
    QVector<NPCSkillPack *> skillPacks() const;
    int skill(const QString &skillPackName, const QString &skillName) const;

    void setName(const QString &name);
    void setValue(const int &value);
    void increaseValue(const int &value);
    void setModificator(const int &modificator);
    void addSkillPack(NPCSkillPack *skillPack);

private:
    QString m_name;
    int m_value{0};
    int m_modificator{0};

    QVector<NPCSkillPack*> m_skillPacks;
};

#endif // NPCATTRIBUTE_H
