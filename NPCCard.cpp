#include "NPCCard.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>

#include <QGridLayout>

#include <QDebug>

NPCCard::NPCCard(QWidget *parent) : QWidget(parent)
{
    m_json = json();
    // Sekcja postać
    m_personal = new PersonalSection( "Postać", m_json, this );

    // Sekcja atrybuty
    m_attributes = new AttributeSection( "Atrybuty", m_json, this );

    // Sekcja sztuczki

    // Sekcja Umiejętności
    m_skillsSection = new SkillsSection( "Umiejętności", m_json, this );

    QGridLayout* all = new QGridLayout;
    all->addWidget( m_personal, 0, 0 );
    all->addWidget( m_attributes, 1, 0 );
    all->addWidget( m_skillsSection, 0, 1, 0, 1 );

    setLayout( all );
}

QJsonDocument NPCCard::json()
{
    QFile file( ":/data/data/NPCCard.json" );
    file.open( QIODevice::ReadOnly );

    if ( file.isOpen() )
        return QJsonDocument::fromJson( file.readAll() );

    return QJsonDocument();
}
