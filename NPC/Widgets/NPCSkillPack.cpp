#include "NPCSkillPack.h"

NPCSkillPack::NPCSkillPack(const QString &name,const QStringList &specs, QWidget *parent)
    : NPCAbstractSkillPack (name, specs, parent)
{

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

        connect( pSpinBox, &SkillSpinBox::valueChanged,
                 this, &NPCSkillPack::enableBougthCheckbox );
        connect( pSpinBox, &SkillSpinBox::skillValueChanged,
                 [this](const int &value, const bool &increase) {
                    emit this->skillValueChanged(value, m_specs, increase);
                }
        );
    }
}

bool NPCSkillPack::hasSkill(const QString &name) const
{
    for ( const QWidget *pSkill: m_skillNames ) {
        const QLabel *pLabel = qobject_cast<const QLabel*>(pSkill);
        if ( pLabel->text() == name )
            return true;
    }
    return false;
}

QVector<QPair<QString, int> > NPCSkillPack::skills()
{
    QVector<QPair<QString, int>> result;

    for ( QWidget *pName: m_skillNames ) {
        QLabel *pLabel = qobject_cast<QLabel*>(pName);
        const QString name = pLabel->text();
        result.push_back( QPair<QString, int>(name, m_skillValues.at(skillIndex(name))->value()) );
    }

    return result;
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

int NPCSkillPack::skillIndex(const QString &name) const
{
    int index{0};
    for ( QWidget *pSkill: m_skillNames ) {
        QLabel *pLabel = qobject_cast<QLabel*>(pSkill);
        if ( pLabel->text() == name ) {
            index = m_skillNames.indexOf( pSkill );
            break;
        }
    }
    return index;
}
