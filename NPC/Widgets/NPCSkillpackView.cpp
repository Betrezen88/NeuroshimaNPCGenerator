#include "NPCSkillpackView.h"

#include <QStyleOption>
#include <QPainter>

NPCSkillpackView::NPCSkillpackView(const QString &name,
                                   QWidget *parent)
    : QWidget(parent),
      m_pName(new QLabel(name, this)),
      m_pSpecs(new QLabel(this)),
      m_pLayout(new QGridLayout)
{
    connect( this, &NPCSkillpackView::specsChanged,
             this, &NPCSkillpackView::updateSpecLabel );
    setFixedWidth( 220 );
    setObjectName( "Skillpack" );
    setStyleSheet( m_skillpackStyle );
    m_pName->setObjectName( "Title" );
    m_pSpecs->setObjectName( "Title" );
    m_pName->setStyleSheet( m_titleStyle );
    m_pSpecs->setStyleSheet( m_titleStyle );
    setLayout( m_pLayout );

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget( m_pName );
    pLayout->addWidget( m_pSpecs, Qt::AlignLeft );
    m_pLayout->addLayout( pLayout, 0, 0 );
}

void NPCSkillpackView::addSpecialization(const QString &name)
{
    m_specializations << name;
    emit specsChanged();
}

void NPCSkillpackView::addSkill(const QString &name)
{
    QLabel *pSkillName = new QLabel(name, this);
    QLabel *pSkillValue = new QLabel("0", this);
    pSkillValue->setObjectName( "Value" );
    pSkillValue->setStyleSheet( m_valueStyle );
    m_skills.insert( pSkillName, pSkillValue );
    int row = m_skills.count();
    m_pLayout->addWidget( pSkillName, row, 0, Qt::AlignLeft );
    m_pLayout->addWidget( pSkillValue, row, 1, Qt::AlignRight );
}

const QString NPCSkillpackView::name() const
{
    return m_pName->text();
}

void NPCSkillpackView::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void NPCSkillpackView::updateSpecLabel()
{
    QString text = "(";
    for ( const QString &spec: m_specializations ) {
        if ( spec != m_specializations.first() )
            text += ",";
        text += spec.at(0);
    }
    text += ")";
    m_pSpecs->setText( text );
}
