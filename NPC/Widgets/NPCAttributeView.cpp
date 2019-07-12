#include "NPCAttributeView.h"

#include "../Utils/DataLoader.h"
#include "NPCAttributeValueWidget.h"

#include <QPainter>
#include <QStyleOption>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include <QDebug>

NPCAttributeView::NPCAttributeView(const QString &name,
                                   const int &value,
                                   const int &modValue,
                                   QWidget *parent)
    : QWidget(parent),
      m_pName(new QLabel(name)),
      m_value(value),
      m_modValue(modValue),
      m_pLayout(new QHBoxLayout),
      m_pSkillsLayout(new QVBoxLayout)
{
    connect( this, &NPCAttributeView::currentValueChanged,
             this, &NPCAttributeView::setCurrentValue );

    m_pName->setObjectName( "Title" );
    m_pName->setStyleSheet( m_titleStyle );
    m_pName->setFixedHeight( 35 );
    m_pSkillsLayout->setSpacing( 1 );
    m_pSkillsLayout->setContentsMargins( 0, 0, 0, 0 );
    m_pSkillsLayout->addWidget( m_pName, Qt::AlignTop );
    m_pSkillsLayout->addStretch();
    m_pLayout->addLayout( pointsView() );
    m_pLayout->addLayout( m_pSkillsLayout, Qt::AlignTop );
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );

    setLayout( m_pLayout );
}

void NPCAttributeView::addSkillpack(NPCSkillpackView *skillpack)
{
    m_skillpacks.insert( skillpack->name(), skillpack );
    m_pSkillsLayout->addWidget( skillpack, Qt::AlignTop );
}

QHash<QString, NPCSkillpackView *> NPCAttributeView::skillpacks() const
{
    return m_skillpacks;
}

void NPCAttributeView::setValue(const int &value)
{
    m_value = value;
    emit currentValueChanged( m_value + m_modValue );
}

int NPCAttributeView::value() const
{
    return m_value;
}

void NPCAttributeView::setModValue(const int &value)
{
    m_modValue = value;
    emit currentValueChanged( m_value + m_modValue );
}

int NPCAttributeView::modValue() const
{
    return m_modValue;
}

int *NPCAttributeView::currentValue()
{
    return &m_currentValue;
}

void NPCAttributeView::setSkillValue(const QString &skillpack, const QString &skill, const int &value)
{
    m_skillpacks.value(skillpack)->setSkillValue(skill, value);
}

void NPCAttributeView::setCurrentValue(const int &value)
{
    m_currentValue = value;
}

QVBoxLayout *NPCAttributeView::pointsView()
{
    QVBoxLayout *pLayout = new QVBoxLayout;
    const QJsonArray &mods = DataLoader::loadJson( ":/data/json/DifficultyLevel.json" );

    for ( const QJsonValue &tMod: mods ) {
        const QJsonObject &mod = tMod.toObject();
        NPCAttributeValueWidget *pValue = new NPCAttributeValueWidget( mod.value("short").toString(),
                                                                       mod.value("value").toInt(),
                                                                       this );
        connect( this, &NPCAttributeView::currentValueChanged,
                 pValue, &NPCAttributeValueWidget::updateValueLabel );
        pLayout->addWidget( pValue );
    }
    pLayout->setSpacing( 1 );
    pLayout->addStretch();

    return pLayout;
}
