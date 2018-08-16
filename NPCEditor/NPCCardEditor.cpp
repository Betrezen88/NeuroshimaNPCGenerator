#include "NPCCardEditor.h"

#include <QFile>
#include <QJsonParseError>
#include <QJsonObject>

#include <QDebug>

NPCCardEditor::NPCCardEditor(QWidget *parent)
    : QWidget(parent)
{
    m_json.insert( "Sections", loadJson(":/NPCCard.json") );
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
