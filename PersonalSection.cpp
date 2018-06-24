#include "PersonalSection.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include <QLabel>
#include <QGridLayout>

PersonalSection::PersonalSection(const QString &name, const QJsonDocument &json, QWidget *parent)
    : Section(json, parent)
{
    QHBoxLayout* all = new QHBoxLayout;
    all->addWidget( create(name) );

    setLayout( all );
}

QGroupBox *PersonalSection::create(const QString& name)
{
    QGroupBox* groupBox = new QGroupBox( name );
    if ( !m_json.isEmpty() && m_json.object().contains("Postać") ) {
        QGridLayout* layout = new QGridLayout;
        QJsonArray array = m_json.object().take( "Postać" ).toArray();
        for ( const QJsonValue& item: array ) {
            int row = item.toObject().value( "no" ).toInt();
            QString text = item.toObject().value( "text" ).toString();
            QString type = item.toObject().value( "type" ).toString();

            QWidget* widget = createWidget( type );

            m_values.insert( text, widget );

            layout->addWidget( new QLabel(text), row, 0 );
            layout->addWidget( widget, row, 1 );
        }
        groupBox->setLayout( layout );
    }
    return groupBox;
}
