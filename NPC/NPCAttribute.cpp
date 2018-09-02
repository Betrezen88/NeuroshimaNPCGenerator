#include "NPCAttribute.h"

NPCAttribute::NPCAttribute()
{

}

NPCAttribute::NPCAttribute(const QString &name, const int &value, const int &modificator)
    : m_name(name),
      m_value(value),
      m_modificator(modificator)
{

}

QString NPCAttribute::name() const
{
    return m_name;
}

int NPCAttribute::value() const
{
    return m_value;
}

int NPCAttribute::modificator() const
{
    return m_modificator;
}

NPCSkillPack *NPCAttribute::skillPack(const QString &name) const
{
    NPCSkillPack* pSkillPack{nullptr};

    for ( NPCSkillPack* tSkillPack: m_skillPacks )
        if ( tSkillPack->name() == name ) {
            pSkillPack = tSkillPack;
            break;
        }

    return pSkillPack;
}

QVector<NPCSkillPack *> NPCAttribute::skillPacks() const
{
    return m_skillPacks;
}

int NPCAttribute::skill(const QString &skillPackName, const QString &skillName) const
{
    int tValue = 0;

    for ( NPCSkillPack *tSkillPack: m_skillPacks )
        if ( tSkillPack->name() == skillPackName ) {
            tValue = tSkillPack->skill( skillName );
            break;
        }

    return tValue;
}

void NPCAttribute::setName(const QString &name)
{
    m_name = name;
}

void NPCAttribute::setValue(const int &value)
{
    m_value = value;
}

void NPCAttribute::increaseValue(const int &value)
{
    m_value += value;
}

void NPCAttribute::setModificator(const int &modificator)
{
    m_modificator = modificator;
}

void NPCAttribute::addSkillPack(NPCSkillPack* skillPack)
{
    m_skillPacks.push_back( skillPack );
}
