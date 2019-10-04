#include "NPCGeneralSkillpack.h"

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>

NPCGeneralSkillpack::NPCGeneralSkillpack(const QString &name,
                                         const QStringList &specs,
                                         const QStringList &skills,
                                         QWidget *parent)
    : NPCAbstractSkillpackView(name, specs, parent)
{
    addSkills( skills );
}

QVector<QPair<QString, int> > NPCGeneralSkillpack::skills() const
{
    QVector<QPair<QString, int>> result;

    for ( const QPair<QComboBox*, QLabel*> &skill: m_skills )
        result.push_back( QPair<QString, int>(skill.first->currentText(),
                                              skill.second->text().toInt()) );

    return result;
}

bool NPCGeneralSkillpack::hasSkill(const QString &name) const
{
    for ( const QPair<QComboBox*, QLabel*> &skill: m_skills )
        if ( name == skill.first->currentText() )
            return true;
    return false;
}

void NPCGeneralSkillpack::setSkillValue(const QString &name, const int &value)
{
    for ( const QPair<QComboBox*, QLabel*> &skill: m_skills )
        if ( name == skill.first->currentText() ) {
            skill.second->setNum( value );
            break;
        }
}

void NPCGeneralSkillpack::setSkillValueBy(const QString &name, const int &value)
{
    for ( const QPair<QComboBox*, QLabel*> &skill: m_skills )
        if ( name == skill.first->currentText() ) {
            int skillValue = skill.second->text().toInt();
            skill.second->setNum( skillValue + value );
            break;
        }
}

void NPCGeneralSkillpack::checkSkillSelection(QComboBox *pSkill, const QString &name)
{
    for ( QPair<QComboBox*, QLabel*> skill: m_skills ) {
        if ( pSkill != skill.first )
            if ( name == skill.first->currentText() )
                skill.first->setCurrentIndex( skill.first->currentIndex()+1 );
    }
}

void NPCGeneralSkillpack::addSkills(const QStringList &skills)
{
    for ( int i{0}; i<=2; ++i ) {
        QComboBox *pSkill = new QComboBox( this );
        connect( pSkill, &QComboBox::currentTextChanged,
                 [this, pSkill](const QString &name){ this->checkSkillSelection(pSkill, name); } );
        pSkill->addItems( skills );

        m_skills.push_back( QPair<QComboBox*, QLabel*>(pSkill, new QLabel("0", this)) );
        m_pLayout->addWidget( pSkill, m_skills.count(), 0 );
        m_pLayout->addWidget( m_skills.last().second, m_skills.count(), 1 );
    }
}
