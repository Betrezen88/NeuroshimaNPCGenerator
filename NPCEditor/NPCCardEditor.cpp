#include "NPCCardEditor.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonObject>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

#include <QDebug>

NPCCardEditor::NPCCardEditor(QWidget *parent)
    : QWidget(parent)
{
    m_NPCCard = loadJson(":/NPCCard.json");

    m_pPersonal = new PersonalWidget( m_NPCCard.object().value("Postać").toArray(), this );
    fillAttributes();

    QHBoxLayout *pAll = new QHBoxLayout;
    pAll->addLayout( createColumn1() );
    pAll->addLayout( createColumn4() );
    pAll->addLayout( createColumn2() );
    pAll->addLayout( createColumn3() );

    pAll->setSpacing( 5 );

    setLayout( pAll );
}

NPCCardEditor::~NPCCardEditor()
{

}

QJsonDocument NPCCardEditor::loadJson(const QString &fileName)
{
    QJsonDocument json;
    QFile file(fileName);
    if ( file.open(QIODevice::ReadOnly) ) {
        QJsonParseError error;
        json = QJsonDocument::fromJson( file.readAll(), &error );
        if ( json.isEmpty() )
            qDebug() << "JSON Error: " << error.errorString();
    }
    else
        qDebug() << "File " << fileName << " could not be opened !";
    return json;
}

QVBoxLayout *NPCCardEditor::createColumn1()
{
    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addWidget( m_pPersonal );

    return pLayout;
}

QVBoxLayout *NPCCardEditor::createColumn2()
{
    QVBoxLayout *pLayout = new QVBoxLayout;

//    pLayout->addWidget( new CardWidget("Współczynniki \ni umiejętności", this) );
    pLayout->addWidget( m_attributes.value("Budowa") );
    pLayout->addWidget( m_attributes.value("Zręczność") );
    pLayout->setSpacing( 1 );

    return pLayout;
}

QVBoxLayout *NPCCardEditor::createColumn3()
{
    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addWidget( m_attributes.value("Charakter") );
    pLayout->addWidget( m_attributes.value("Percepcja") );
    pLayout->setSpacing( 1 );

    return pLayout;
}

QVBoxLayout *NPCCardEditor::createColumn4()
{
    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addWidget( new CardWidget("Współczynniki i umiejętności", this) );
    pLayout->addWidget( m_attributes.value("Spryt"), 0, Qt::AlignTop );
    pLayout->setSpacing( 1 );

    return pLayout;
}

void NPCCardEditor::fillAttributes()
{
    const QJsonArray &attributes = m_NPCCard.object().value("Atrybuty").toArray();

    for ( const QJsonValue &attribute : attributes ) {
        const QJsonObject &obj = attribute.toObject();
        const QString &name = obj.value("name").toString();
        const QJsonArray &skillPacks = obj.value("skillPacks").toArray();
        m_attributes.insert( name, new AttributeWidget(name, skillPacks) );
    }
}
