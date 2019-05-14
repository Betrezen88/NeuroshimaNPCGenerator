#include "NPCSkillChoose.h"

#include "../Utils/DataLoader.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>

NPCSkillChoose::NPCSkillChoose(QWidget *parent)
    : QWidget(parent),
      m_pName(new QLineEdit(this)),
      m_pAttribute(new QLabel(this)),
      m_pNames(new QComboBox(this)),
      m_pAttributes(new QComboBox(this)),
      m_pChose(new QRadioButton("Wybierz", this)),
      m_pAdd(new QRadioButton("Dodaj", this)),
      m_pOkBtn(new QPushButton("Ok", this)),
      m_pCloseBtn(new QPushButton("Anuluj", this))
{
    setWindowTitle( "Wybierz/Dodaj umiejętność" );
    setMinimumSize( 250, 150 );

    connect( m_pChose, &QRadioButton::toggled,
             m_pNames, &QComboBox::setEnabled );
    connect( m_pChose, &QRadioButton::toggled,
             m_pAttribute, &QLabel::setEnabled );
    connect( m_pNames, &QComboBox::currentTextChanged,
             this, &NPCSkillChoose::setAttributeName );
    connect( m_pAdd, &QRadioButton::toggled,
             m_pName, &QLineEdit::setEnabled );
    connect( m_pAdd, &QRadioButton::toggled,
             m_pAttributes, &QComboBox::setEnabled );
    connect( m_pCloseBtn, &QPushButton::clicked,
             this, &NPCSkillChoose::closeWidget );
    connect( m_pOkBtn, &QPushButton::clicked,
             this, &NPCSkillChoose::okBtnClicked );

    loadData();

    m_pChose->toggle();
    m_pName->setEnabled( false );
    m_pAttributes->setEnabled( false );

    QHBoxLayout *pChoseL = new QHBoxLayout;
    pChoseL->addWidget( m_pNames );
    pChoseL->addWidget( m_pAttribute );

    QHBoxLayout *pAddL = new QHBoxLayout;
    pAddL->addWidget( m_pName );
    pAddL->addWidget( m_pAttributes );

    QHBoxLayout *pButtonsL = new QHBoxLayout;
    pButtonsL->addWidget( m_pOkBtn );
    pButtonsL->addWidget( m_pCloseBtn );

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( m_pChose );
    pLayout->addLayout( pChoseL );
    pLayout->addWidget( m_pAdd );
    pLayout->addLayout( pAddL );
    pLayout->addLayout( pButtonsL );

    setLayout( pLayout );
}

void NPCSkillChoose::setAttributeName(const QString &skillName)
{
    m_pAttribute->setText( m_skills.value(skillName) );
}

void NPCSkillChoose::okBtnClicked()
{
    QString name = (m_pChose->isChecked()) ? m_pNames->currentText() : m_pName->text();
    QString attribute = (m_pChose->isChecked()) ? m_pAttribute->text() : m_pAttributes->currentText();
    emit otherSkillChoosed( name, attribute );
    emit closeWidget();
}

void NPCSkillChoose::loadData()
{
    const QJsonArray &skills = DataLoader::loadJson( ":/data/json/OtherSkills.json" );

    for ( const QJsonValue skill: skills ) {
        const QJsonObject &tSkill = skill.toObject();
        m_skills.insert( tSkill.value("name").toString(),
                         tSkill.value("attribute").toString() );
    }
    m_pNames->insertItems( 0, m_skills.keys() );

    const QJsonArray &attributes = DataLoader::loadJson( ":data/json/Attributes.json" );

    for ( const QJsonValue attribute: attributes ) {
        const QJsonObject &tAttribute = attribute.toObject();
        m_pAttributes->addItem( tAttribute.value("name").toString() );
    }
}
