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
      m_pName(new QLabel(name, this)),
      m_pBougth(new QCheckBox("Pakiet",this)),
      m_pSkillLayout(new QGridLayout)
{
    setObjectName( "SkillPack" );
    setStyleSheet( m_skillPackWidgetStyle );
    m_pName->setObjectName( "SkillPackTitle" );
    m_pName->setStyleSheet( m_skillPackTitleStyle );

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
}

void NPCSkillPackWidget::addSkill(const QString &name, SkillSpinBox *skillBox)
{
    const int &row = m_skills.count();
    QLabel *pLabel = new QLabel( name, this );
    m_pSkillLayout->addWidget( pLabel, row, 0 );
    m_pSkillLayout->addWidget( skillBox, row, 1 );
    m_skills.push_back( QPair<const QLabel *, SkillSpinBox *>{ pLabel, skillBox } );
}

bool NPCSkillPackWidget::hasSkill(const QString &name) const
{
    for ( const QPair<const QLabel*, SkillSpinBox*> &tSkill: m_skills ) {
        if ( tSkill.first->text() == name )
            return true;
    }
    return false;
}

SkillSpinBox *NPCSkillPackWidget::skill(const QString &name)
{
    for ( const QPair<const QLabel*, SkillSpinBox*> &tSkill: m_skills ) {
        if ( tSkill.first->text() == name )
            return tSkill.second;
    }
    return nullptr;
}

QVector<SkillSpinBox *> NPCSkillPackWidget::skills()
{
    QVector<SkillSpinBox *> tSkills = QVector<SkillSpinBox *>(3);

    for ( QPair<const QLabel *, SkillSpinBox *> tSkill: m_skills )
        tSkills.push_back( tSkill.second );

    return tSkills;
}

void NPCSkillPackWidget::addSpecialization(const QString &spec)
{
    m_specs << spec;
}

QStringList NPCSkillPackWidget::specializations() const
{
    return m_specs;
}

void NPCSkillPackWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void NPCSkillPackWidget::onAvailableSkillPointsChanged(const int value)
{
    const int disable = (5 > value) && !m_pBougth->isChecked();
    m_pBougth->setDisabled( disable );
}

void NPCSkillPackWidget::onBougth(const bool &checked)
{
    int value = (checked) ? 1 : 0;
    for ( QPair<const QLabel*, SkillSpinBox*> skill: m_skills) {
        skill.second->setMinimum( value );
        skill.second->setValue( value );
    }
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
