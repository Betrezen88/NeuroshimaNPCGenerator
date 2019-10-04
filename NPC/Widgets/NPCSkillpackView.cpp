#include "NPCSkillpackView.h"

#include <QLabel>
#include <QGridLayout>

NPCSkillpackView::NPCSkillpackView(const QString &name, const QStringList &specs, const QStringList &skills,
                                   QWidget *parent)
    : NPCAbstractSkillpackView(name, specs, parent)
{
    addSkills( skills );
}

QVector<QPair<QString, int> > NPCSkillpackView::skills() const
{
    QVector<QPair<QString, int>> result;
    for ( const QPair<QLabel*, QLabel*> &skill: m_skills )
        result.push_back( QPair<QString, int>(skill.first->text(),
                                              skill.second->text().toInt()) );
    return result;
}

bool NPCSkillpackView::hasSkill(const QString &name) const
{
    for ( const QPair<QLabel*, QLabel*> &skill: m_skills )
        if ( name == skill.first->text() )
            return true;
    return false;
}

void NPCSkillpackView::setSkillValue(const QString &name, const int &value)
{
    for ( QPair<QLabel*, QLabel*> skill: m_skills )
        if ( name == skill.first->text() ) {
            skill.second->setNum( value );
            break;
        }
}

void NPCSkillpackView::setSkillValueBy(const QString &name, const int &value)
{
    for ( QPair<QLabel*, QLabel*> skill: m_skills )
        if ( name == skill.first->text() ) {
            int skillValue = skill.second->text().toInt();
            skill.second->setNum( skillValue + value );
            break;
        }
}

void NPCSkillpackView::addSkills(const QStringList &skills)
{
    for ( const QString &skill: skills ) {
        m_skills.push_back( QPair<QLabel*, QLabel*>(new QLabel( skill, this ), new QLabel( "0", this )) );
        m_pLayout->addWidget( m_skills.last().first, m_skills.count(), 0 );
        m_pLayout->addWidget( m_skills.last().second, m_skills.count(), 1 );
    }
}
