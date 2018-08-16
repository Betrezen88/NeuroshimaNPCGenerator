#include "NPCCardEditor.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonObject>

#include <QDebug>

NPCCardEditor::NPCCardEditor(QWidget *parent)
    : QWidget(parent)
{
    m_NPCCard = loadJson(":/NPCCard.json");

    m_pPersonal = new Personal( m_NPCCard.object().value("Postać").toArray(), this );

    QGridLayout *all = new QGridLayout;
    all->addWidget( m_pPersonal, 0, 0 );

    setLayout( all );
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
