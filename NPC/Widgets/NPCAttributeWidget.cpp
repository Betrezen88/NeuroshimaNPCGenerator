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
      m_pName(new QLabel(name)),
      m_mods(mods)
{
    m_pName->setObjectName( "NameStyle" );
    m_pName->setStyleSheet( m_nameStyle );
    m_pName->setFixedSize( 100, 40 );

    QVBoxLayout *pLayout = new QVBoxLayout;
    setLayout( pLayout );
    pLayout->addWidget( createTitleBar(mods) );
}

QHash<const QString, NPCSkillPackWidget *> *NPCAttributeWidget::skillPacks()
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

void NPCAttributeWidget::setValue(const int value)
{
    m_value = value;
    emit valueChanged( m_value );
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
    NPCAttributeValueWidget *pValue = new NPCAttributeValueWidget( name, value );
    pValue->updateValue( m_value );
    connect( this, &NPCAttributeWidget::valueChanged,
             pValue, &NPCAttributeValueWidget::updateValue );
    return pValue;
}
