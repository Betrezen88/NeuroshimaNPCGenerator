#include "NPCSkillPack.h"

NPCSkillPack::NPCSkillPack(const QString &name,const QStringList &specs, QWidget *parent)
    : NPCAbstractSkillPack (name, specs, parent)
{
    QHBoxLayout *pTitleLayout = new QHBoxLayout;
    pTitleLayout->addWidget( m_pName );
    pTitleLayout->addWidget( m_pBougth );

    m_pSkillsLayout = new QGridLayout;

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addLayout( pTitleLayout );
    pLayout->addLayout( m_pSkillsLayout );

    setLayout( pLayout );
}

void NPCSkillPack::addSkills(const QStringList &skills)
{
    for ( const QString &skill: skills ) {
        QLabel *pLabel = new QLabel(skill, this);
        SkillSpinBox *pSpinBox = new SkillSpinBox(this);
        m_pSkillsLayout->addWidget( pLabel );
        m_pSkillsLayout->addWidget( pSpinBox );
        m_skillNames.push_back( pLabel );
        m_skillValues.push_back( pSpinBox );
    }
}

bool NPCSkillPack::hasSkill(const QString &name) const
{
    for ( const QLabel *pSkill: m_skillNames )
        if ( pSkill->text() == name )
            return true;
    return false;
}

void NPCSkillPack::onBougth(const bool &checked)
{
    int value = (checked) ? 1 : 0;

    if ( !checked ) {
        int returnPoints = 0;
        for ( SkillSpinBox *pSkill: m_skillValues) {
            for ( int i = pSkill->value(); i>pSkill->minimum(); --i )
                returnPoints += i;
        }
        if ( 0 < returnPoints )
            emit refundPoints( returnPoints, m_specs );
    }

    for ( SkillSpinBox *pSkill: m_skillValues) {
        pSkill->setMinimum( value );
        pSkill->setValue( value );
    }
}

void NPCSkillPack::enableBougthCheckbox()
{
    int sum{0};
    for ( SkillSpinBox* pSkill: m_skillValues )
        sum += pSkill->value();

    bool enable = (0 == sum) || (m_pBougth->isChecked());
    m_pBougth->setEnabled( enable );
}
