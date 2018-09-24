#include "NPCSkillPackWidget.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>

#include <QDebug>

NPCSkillPackWidget::NPCSkillPackWidget(const QString &name,
                                       const QJsonArray &skills,
                                       const QJsonArray &specializations,
                                       QWidget *parent)
    : QWidget(parent),
      m_pName(new QLabel(createSkillPackName(name, specializations), this)),
      m_pBougth(new QCheckBox("Pakiet",this))
{
    setObjectName( "SkillPack" );
    setStyleSheet( m_skillPackWidgetStyle );
    m_pName->setObjectName( "SkillPackTitle" );
    m_pName->setStyleSheet( m_skillPackTitleStyle );
    fillSpecs( specializations );
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->setMargin( 5 );
    pLayout->setSpacing( 1 );
    setLayout( pLayout );

    QHBoxLayout *pTitleBar = new QHBoxLayout;
    pTitleBar->addWidget( m_pName, 0, Qt::AlignLeft );
    pTitleBar->addWidget( m_pBougth, 0, Qt::AlignRight );

    pLayout->addLayout( pTitleBar );
    createSkills( skills );

    connect( m_pBougth, &QCheckBox::toggled,
             [this](const bool &checked){
                emit bougth(checked, m_specs);
    } );
    connect( m_pBougth, &QCheckBox::toggled,
             this, &NPCSkillPackWidget::onBougth);
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

void NPCSkillPackWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void NPCSkillPackWidget::onBougth(const bool &checked)
{
    int value = (checked) ? 1 : 0;
    for ( QPair<const QLabel*, SkillSpinBox*> skill: m_skills) {
        skill.second->setMinimum( value );
        skill.second->setValue( value );
    }
}

void NPCSkillPackWidget::createSkills(const QJsonArray &skills)
{
    QVBoxLayout *pLayout = qobject_cast<QVBoxLayout*>( layout() );
    for ( const QJsonValue &skill: skills ) {
        const QString &name = skill.toString();

        QLabel *pLabel = new QLabel( name, this );
        SkillSpinBox *pSpinBox = new SkillSpinBox( this );
        QHBoxLayout* pSkillRowL = new QHBoxLayout;
        pSkillRowL->addWidget( pLabel );
        pSkillRowL->addWidget( pSpinBox );
        pLayout->addLayout( pSkillRowL );
        m_skills.push_back( QPair<const QLabel*, SkillSpinBox*>(pLabel, pSpinBox) );
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

void NPCSkillPackWidget::fillSpecs(const QJsonArray &specs)
{
    for ( const QJsonValue &spec: specs )
        m_specs << spec.toString();
}
