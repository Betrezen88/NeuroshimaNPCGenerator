#include "NPCAbstractSkillPack.h"

NPCAbstractSkillPack::NPCAbstractSkillPack(const QString &name,
                                           const QStringList &specs,
                                           QWidget *parent)
    : NPCCustomWidget (parent),
      m_name(name),
      m_specs(specs),
      m_pName(new QLabel(this)),
      m_pBougth(new QCheckBox("Pakiet", this))
{
    connect( m_pBougth, &QCheckBox::toggled,
             [this](const bool &checked){
                emit bougth(checked, m_specs);
    } );
    connect( m_pBougth, &QCheckBox::toggled,
             this, &NPCAbstractSkillPack::onBougth);

    setObjectName( "SkillPack" );
    setStyleSheet( m_skillPackWidgetStyle );

    m_pName->setObjectName( "SkillPackTitle" );
    m_pName->setStyleSheet( m_skillPackTitleStyle );

    m_pSkillsLayout = new QGridLayout;
    m_pSkillsLayout->setColumnStretch( 0, 1 );
    m_pSkillsLayout->setColumnStretch( 1, 1 );

    createTitleLabel( m_name, m_specs );

    QHBoxLayout *pTitleLayout = new QHBoxLayout;
    pTitleLayout->addWidget( m_pName, 0, Qt::AlignLeft );
    pTitleLayout->addWidget( m_pBougth, 0, Qt::AlignRight );

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->setMargin( 5 );
    pLayout->setSpacing( 1 );
    pLayout->addLayout( pTitleLayout );
    pLayout->addLayout( m_pSkillsLayout );

    setLayout( pLayout );
}

QStringList NPCAbstractSkillPack::specializations() const
{
    return m_specs;
}

bool NPCAbstractSkillPack::isBougth() const
{
    return m_pBougth->isChecked();
}

SkillSpinBox *NPCAbstractSkillPack::skill(const QString &name) const
{

}

void NPCAbstractSkillPack::onAvailableSkillPointsChanged(const int &skill, const int &specs, const QString &spec)
{
    const int &value = (m_specs.contains(spec)) ? specs+skill : skill;
    if ( m_pBougth->isEnabled() ) {
        const bool disable = (5 > value) && !m_pBougth->isChecked();
        m_pBougth->setDisabled( disable );
    }
    else {
        const bool enable = (value >= 5) && !m_pBougth->isEnabled();
        m_pBougth->setEnabled( enable );
    }
    for ( SkillSpinBox *pSkill: m_skillValues )
        pSkill->onAvailableSkillPointsChanged( value );
}

void NPCAbstractSkillPack::createTitleLabel(const QString &name, const QStringList &specs)
{
    QStringList tSpecs;
    for ( const QString &spec: specs )
        tSpecs << spec.at(0);
    m_pName->setText( QString("%1 (%2)").arg(name).arg(tSpecs.join(',')) );
}
