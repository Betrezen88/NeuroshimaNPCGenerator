#include "NPCOtherSkills.h"

#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

NPCOtherSkills::NPCOtherSkills(QWidget *parent)
    : NPCCustomWidget (parent),
      m_pName(new QLabel("Inne Umiejętności", this)),
      m_pAddBtn(new QPushButton("+", this)),
      m_pSkills(new QListWidget(this))
{
    setObjectName( "OtherSkills" );
    setStyleSheet( m_style );

    m_pAddBtn->setMaximumWidth( 50 );

    connect( m_pAddBtn, &QPushButton::clicked,
             this, &NPCOtherSkills::showSkillChooseDialog );

    QHBoxLayout *pTitleLayout = new QHBoxLayout;
    pTitleLayout->addWidget( m_pName );
    pTitleLayout->addWidget( m_pAddBtn );

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addLayout( pTitleLayout );
    pLayout->addWidget( m_pSkills );

    setLayout( pLayout );
}

QVector<QStringList> NPCOtherSkills::skills() const
{
    QVector<QStringList> values;

    for ( int i=0; i<m_skillNames.count(); ++i ) {
        QStringList row;
        row << m_skillNames.at(i)->text()
            << m_attributeNames.at(i)->text()
            << QString::number(m_skillValues.at(i)->value());
        values.push_back( row );
    }

    return values;
}

void NPCOtherSkills::addSkill(const QString name, const QString attribute)
{
    SkillSpinBox *pSkillBox = new SkillSpinBox(this);
    pSkillBox->setMaximumWidth( 100 );

    connect( pSkillBox, &SkillSpinBox::skillValueChanged,
             [this](const int &value, const bool &increase){
        emit this->skillValueChanged( value, QStringList(), increase );
    } );

    m_skillNames.push_back( new QLabel(name) );
    m_attributeNames.push_back( new QLabel(attribute) );
    m_skillValues.push_back( pSkillBox );

    QListWidgetItem *pItem = new QListWidgetItem();

    QWidget *pWidget = new QWidget();
    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget( m_skillNames.last() );
    pLayout->addWidget( m_attributeNames.last() );
    pLayout->addWidget( pSkillBox );
    pWidget->setLayout( pLayout );
    pItem->setSizeHint( pWidget->sizeHint() );

    m_pSkills->addItem( pItem );
    m_pSkills->setItemWidget( pItem, pWidget );
}


void NPCOtherSkills::onAvailableSkillpointsChanged(const int &skill)
{
    for ( SkillSpinBox *pSkill: m_skillValues )
        pSkill->onAvailableSkillPointsChanged( skill );
}
