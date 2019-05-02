#include "NPCCustomSkillPack.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

NPCCustomSkillPack::NPCCustomSkillPack(const QString &name, const QStringList &specs, QWidget *parent)
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

void NPCCustomSkillPack::addSkills(const QStringList &skills)
{
    for ( int i{0}; i<3; ++i ) {
        QComboBox *pComboBox = new QComboBox(this);
        pComboBox->insertItems(0, skills);
        pComboBox->setCurrentIndex(i);
        SkillSpinBox *pSpinBox = new SkillSpinBox(this);
        m_pSkillsLayout->addWidget( pComboBox );
        m_pSkillsLayout->addWidget( pSpinBox );
        m_skillNames.push_back( pComboBox );
        m_skillValues.push_back( pSpinBox );
    }
}

bool NPCCustomSkillPack::hasSkill(const QString &name) const
{
    for ( const QComboBox *pSkill: m_skillNames )
        if ( name == pSkill->currentText() )
            return true;
    return false;
}

void NPCCustomSkillPack::onBougth(const bool &checked)
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

void NPCCustomSkillPack::enableBougthCheckbox()
{
    int sum{0};

    for ( const SkillSpinBox *pSpinBox: m_skillValues )
        sum += pSpinBox->value();

    bool enable = (0 == sum) || (m_pBougth->isChecked());

    m_pBougth->setEnabled( enable );
}
