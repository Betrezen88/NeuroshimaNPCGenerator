#include "NPCSkillPack.h"

NPCSkillPack::NPCSkillPack(const QString &name,
                           const QStringList &specializations,
                           const QStringList &skills)
    : m_name(name),
      m_specializations(specializations)
{
    for ( const QString &skill: skills )
        m_skills.push_back( QPair<QString, int>(skill, 0) );
}

QString NPCSkillPack::name() const
{
    return m_name;
}

QStringList NPCSkillPack::specializations() const
{
    return m_specializations;
}

QString NPCSkillPack::specializationsShort() const
{
    QString result = " (";
    for ( int i=0; i<m_specializations.size(); ++i ) {
        result += m_specializations.at(i).at(0);
        if ( i != m_specializations.size()-1 )
            result += ",";
    }
    result += ")";
    return result;
}

int NPCSkillPack::skill(const QString &name) const
{
    int value = 0;
    for ( const QPair<QString, int> &tSkill: m_skills )
        if ( tSkill.first == name ) {
            value = tSkill.second;
            break;
        }

    return value;
}

QVector<QPair<QString, int> > NPCSkillPack::skills() const
{
    return m_skills;
}
