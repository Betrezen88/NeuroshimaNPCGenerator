#include "DataLoader.h"

#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>

#include <QDebug>

DataLoader::DataLoader()
{

}

QJsonArray DataLoader::loadJson(const QString &fileName)
{
    QJsonArray array;

    QFile json( fileName );

    if ( json.open(QIODevice::ReadOnly) ) {
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson( json.readAll(), &error );
        if ( doc.isEmpty() ) {
            qDebug() << "Parsing error in file: " << fileName
                     << "\nError: " << error.errorString();
        }
        array = doc.array();
    }
    else {
        qDebug() << "File " << fileName << " not open !";
    }

    return array;
}
