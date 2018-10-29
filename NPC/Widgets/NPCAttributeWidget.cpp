#include "NPCAttributeWidget.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QDebug>

NPCAttributeWidget::NPCAttributeWidget(const QString &name,
                                       const QVector<QPair<QString, int> > &mods,
                                       QWidget *parent)
    : QWidget (parent),
      m_pName(new QLabel(name))
{
    m_pName->setObjectName( "NameStyle" );
    m_pName->setStyleSheet( m_nameStyle );
    m_pName->setFixedSize( 100, 40 );

    QVBoxLayout *pLayout = new QVBoxLayout;
    setLayout( pLayout );
    pLayout->addWidget( createTitleBar(mods) );
}

QHash<const QString &, NPCSkillPackWidget *> *NPCAttributeWidget::skillPacks()
{
    return &m_skillPacks;
}

void NPCAttributeWidget::addSkillPack(const QString &name, NPCSkillPackWidget *skillPack)
{
    m_skillPacks.insert( name, skillPack );
    layout()->addWidget( skillPack );
}

const int *NPCAttributeWidget::value() const
{
    return &m_value;
}

QWidget *NPCAttributeWidget::createTitleBar(const QVector<QPair<QString, int>> &mods)
{
    QWidget *pWidget = new QWidget( this );
    pWidget->setObjectName( "TitleStyle" );
    pWidget->setStyleSheet( m_titleStyle );
    pWidget->setFixedHeight( 50 );
    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget( m_pName );

    for ( const QPair<QString, int> &mod: mods ) {
        pLayout->addWidget( createValueWidget(mod.first, mod.second) );
    }

    pWidget->setLayout( pLayout );
    pLayout->setMargin( 0 );
    pLayout->setSpacing( 1 );
    return pWidget;
}

QWidget *NPCAttributeWidget::createValueWidget(const QString &name, const int &value)
{
    QWidget *pWidget = new QWidget( this );
    pWidget->setObjectName( "ValueStyle" );
    pWidget->setStyleSheet( m_valueStyle );
    pWidget->setFixedSize( 30, 35 );
    QVBoxLayout *pLayout = new QVBoxLayout;

    QLabel *pLabel = new QLabel( name, pWidget );
    pLabel->setObjectName( "LabelStyle" );
    pLabel->setStyleSheet( m_labelStyle );

    QLabel *pValue = new QLabel( QString::number(m_value + value), pWidget );

    pLayout->addWidget( pLabel, 0, Qt::AlignHCenter );
    pLayout->addWidget( pValue, 0, Qt::AlignHCenter );

    pLayout->setSpacing( 1 );
    pLayout->setMargin( 0 );
    pWidget->setLayout( pLayout );
    return pWidget;
}
