#include "SkillsSection.h"

#include <QHBoxLayout>

#include <QDebug>

SkillsSection::SkillsSection(const QString &name, const QJsonDocument &json, QWidget *parent)
    : Section(json, parent)
{
    QHBoxLayout* all = new QHBoxLayout;
    all->addWidget( create(name) );

    setLayout( all );
}

QGroupBox *SkillsSection::create(const QString &name)
{
    QGroupBox* groupBox = new QGroupBox( name );

    QGridLayout* skillsLayout = new QGridLayout;
    addAttributeGroupBox( skillsLayout );
    groupBox->setLayout( skillsLayout );
    return groupBox;
}

QGroupBox *SkillsSection::createAttribute(const QJsonValue& value )
{
    const QString name = value.toObject().value("name").toString();

    QGroupBox* groupBox = new QGroupBox( name );
    QVBoxLayout* column = new QVBoxLayout;

    QJsonArray skillPacks = value.toObject().value( "skillPacks" ).toArray();

    for ( const QJsonValue& skillPack: skillPacks ) {
        const QString packName = skillPack.toObject().value("name").toString();
        const QJsonDocument doc( skillPack.toObject().value("skills").toArray() );
        SkillPackSection* pack = new SkillPackSection( packName, doc, this );
        m_skillPacks.insert( packName, pack );
        column->addWidget( pack );
    }

    groupBox->setLayout( column );

    return groupBox;
}

void SkillsSection::addAttributeGroupBox(QGridLayout *layout)
{
    if ( !m_json.isEmpty() && m_json.object().contains("Atrybuty") ) {
        QJsonArray attributes = m_json.object().value( "Atrybuty" ).toArray();
        for ( const QJsonValue& attribute: attributes ) {
            QString name = attribute.toObject().value("name").toString();
            if ( name == "Budowa" ) {
                layout->addWidget( createAttribute(attribute), 0, 0);
            }
            else if ( name == "Charakter" ) {
                layout->addWidget( createAttribute(attribute), 1, 0);
            }
            else if ( name == "Zręczność" ) {
                layout->addWidget( createAttribute(attribute), 0, 1, 0, 1);
            }
            else if ( name == "Spryt" ) {
                layout->addWidget( createAttribute(attribute), 0, 2, 0, 1);
            }
            else if ( name == "Percepcja" ) {
                layout->addWidget( createAttribute(attribute), 0, 3, 0, 1);
            }
        }
    }
}
