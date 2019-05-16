#include "NPCOtherSkillsView.h"

#include <QGridLayout>
#include <QLabel>

NPCOtherSkillsView::NPCOtherSkillsView(const QString &name, QWidget *parent)
    : NPCCustomWidget(parent),
      m_pLayout(new QGridLayout),
      m_pName(new QLabel(name, this))
{
    setFixedWidth( 220 );
    setObjectName( "Skillpack" );
    setStyleSheet( m_skillpackStyle );
    m_pName->setObjectName( "Title" );
    m_pName->setStyleSheet( m_titleStyle );

    m_pLayout->addWidget( m_pName, 0, 0, 1, 3 );
    setLayout( m_pLayout );
}

void NPCOtherSkillsView::addSkill(const QString &name, const QString &attribute, const int &value)
{
    QLabel *pName = new QLabel(name);
    QLabel *pAttribute = new QLabel(attribute);
    QLabel *pValue = new QLabel(QString::number(value));
    pValue->setObjectName( "Value" );
    pValue->setStyleSheet( m_valueStyle );
    int row = m_skills.count() + 1;

    m_pLayout->addWidget( pName, row, 0 );
    m_pLayout->addWidget( pAttribute, row, 1, Qt::AlignRight );
    m_pLayout->addWidget( pValue, row, 2, Qt::AlignRight );

    m_skills.insert( pName, QPair<QLabel*,QLabel*>(pAttribute, pValue) );
}

bool NPCOtherSkillsView::hasSkill(const QString &name) const
{
    for ( QLabel *pName: m_skills.keys() )
        if ( name == pName->text() )
            return true;
    return false;
}

QHash<QLabel *, QPair<QLabel *, QLabel *> > NPCOtherSkillsView::skills() const
{
    return m_skills;
}
