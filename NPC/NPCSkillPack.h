#ifndef NPCSKILLPACK_H
#define NPCSKILLPACK_H

#include <QPair>
#include <QString>
#include <QStringList>
#include <QVector>

class NPCSkillPack
{
public:
    NPCSkillPack(const QString &name,
                 const QStringList &specializations,
                 const QStringList &skills);
    ~NPCSkillPack() = default;

    QString name() const;
    QStringList specializations() const;
    QString specializationsShort() const;
    int skill(const QString &name) const;
    QVector<QPair<QString, int>> skills() const;

private:
    QString m_name;
    QStringList m_specializations;
    QVector<QPair<QString, int>> m_skills;
};

#endif // NPCSKILLPACK_H
