#include "NPCSkillPackWidget.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>

#include <QDebug>

NPCSkillPackWidget::NPCSkillPackWidget(const QString &name, QWidget *parent)
    : QWidget(parent),
      m_name(name),
      m_pName(new QLabel(m_name, this)),
      m_pBougth(new QCheckBox("Pakiet",this)),
      m_pSkillLayout(new QGridLayout)
{
    setObjectName( "SkillPack" );
    setStyleSheet( m_skillPackWidgetStyle );
    m_pName->setObjectName( "SkillPackTitle" );
    m_pName->setStyleSheet( m_skillPackTitleStyle );
    m_pSkillLayout->setColumnStretch( 0, 1);
    m_pSkillLayout->setColumnStretch( 1, 1);

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->setMargin( 5 );
    pLayout->setSpacing( 1 );
    setLayout( pLayout );

    QHBoxLayout *pTitleBar = new QHBoxLayout;
    pTitleBar->addWidget( m_pName, 0, Qt::AlignLeft );
    pTitleBar->addWidget( m_pBougth, 0, Qt::AlignRight );

    pLayout->addLayout( pTitleBar );
    pLayout->addLayout( m_pSkillLayout );

    connect( m_pBougth, &QCheckBox::toggled,
             [this](const bool &checked){
                emit bougth(checked, m_specs);
    } );
    connect( m_pBougth, &QCheckBox::toggled,
             this, &NPCSkillPackWidget::onBougth);
    connect( this, &NPCSkillPackWidget::onSpecsChanged,
             this, &NPCSkillPackWidget::updateNameLabel );
}

void NPCSkillPackWidget::addSkill(const QString &name, SkillSpinBox *skillBox)
{
    const int &row = m_skills.count();
    QLabel *pLabel = new QLabel( name, this );
    m_pSkillLayout->addWidget( pLabel, row, 0 );
    m_pSkillLayout->addWidget( skillBox, row, 1 );
    m_skills.push_back( QPair<const QLabel *, SkillSpinBox *>{ pLabel, skillBox } );

    connect( skillBox, &SkillSpinBox::valueChanged,
             this, &NPCSkillPackWidget::enableBougthCheckbox );
    connect( skillBox, &SkillSpinBox::skillValueChanged,
             [this](const int &value, const bool &increase) {
                emit this->skillValueChanged(value, m_specs, increase);
            }
    );
}

bool NPCSkillPackWidget::hasSkill(const QString &name) const
{
    for ( const QPair<const QLabel*, SkillSpinBox*> &tSkill: m_skills ) {
        if ( tSkill.first->text() == name )
            return true;
    }
    return false;
}

SkillSpinBox *NPCSkillPackWidget::skill(const QString &name) const
{
    for ( const QPair<const QLabel*, SkillSpinBox*> &tSkill: m_skills ) {
        if ( tSkill.first->text() == name )
            return tSkill.second;
    }
    return nullptr;
}

QVector<QPair<const QLabel *, SkillSpinBox *> > NPCSkillPackWidget::skills() const
{
    return m_skills;
}

void NPCSkillPackWidget::addSpecialization(const QString &spec)
{
    m_specs << spec;
    emit onSpecsChanged( m_specs );
}

QStringList NPCSkillPackWidget::specializations() const
{
    return m_specs;
}

bool NPCSkillPackWidget::isBougth() const
{
    return m_pBougth->isChecked();
}

void NPCSkillPackWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void NPCSkillPackWidget::onAvailableSkillPointsChanged(const int &skill, const int &specs, const QString &spec)
{
    const int &value = (m_specs.contains(spec)) ? specs+skill : skill;
    if ( m_pBougth->isEnabled() ) {
        const int disable = (5 > value) && !m_pBougth->isChecked();
        m_pBougth->setDisabled( disable );
    }
    for ( QPair<const QLabel*, SkillSpinBox*> pSkill: m_skills )
        pSkill.second->onAvailableSkillPointsChanged( value );
}

void NPCSkillPackWidget::onBougth(const bool &checked)
{
    int value = (checked) ? 1 : 0;

    if ( !checked ) {
        int returnPoints = 0;
        for ( QPair<const QLabel*, SkillSpinBox*> skill: m_skills) {
            for ( int i = skill.second->value(); i>skill.second->minimum(); --i )
                returnPoints += i;
        }
        if ( 0 < returnPoints )
            emit refundPoints( returnPoints, m_specs );
    }

    for ( QPair<const QLabel*, SkillSpinBox*> skill: m_skills) {
        skill.second->setMinimum( value );
        skill.second->setValue( value );
    }
}

void NPCSkillPackWidget::enableBougthCheckbox()
{
    int sum{0};
    for ( QPair<const QLabel*, SkillSpinBox*> skill: m_skills )
        sum += skill.second->value();

    bool enable = (0 == sum) || (m_pBougth->isChecked());

    m_pBougth->setEnabled( enable );
}

void NPCSkillPackWidget::updateNameLabel(const QStringList &specs)
{
    QStringList tSpecs;

    for ( const QString &spec: specs )
        tSpecs << spec.at(0);

    m_pName->setText( QString("%1 (%2)").arg(m_name).arg(tSpecs.join(',')) );
}

const QString NPCSkillPackWidget::createSkillPackName(const QString &name, const QJsonArray &specs)
{
    QString result = name + " (";
    for ( const QJsonValue &tSpec: specs ) {
        const QString &specName = tSpec.toString();
        result += specName.at(0);
        if ( specName != specs.last().toString() )
            result += ",";
    }
    return result += ")";
}
