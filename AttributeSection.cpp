#include "AttributeSection.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <QLabel>
#include <QSpinBox>

AttributeSection::AttributeSection(const QString &name, const QJsonDocument &json, QWidget *parent)
    : Section(json, parent)
{
    QHBoxLayout* all = new QHBoxLayout;
    all->addWidget( create(name) );

    setLayout( all );
}

QGroupBox *AttributeSection::create(const QString &name)
{
    QGroupBox* groupBox = new QGroupBox( name );

    QGridLayout* attributesL = new QGridLayout;
    createLabelRow( attributesL );
    createAttributeRows( attributesL );

    groupBox->setLayout( attributesL );

    return groupBox;
}

void AttributeSection::createLabelRow(QGridLayout *layout)
{
    if ( !m_json.isEmpty() && m_json.object().contains("PT") ) {
        QJsonArray array = m_json.object().take("PT").toArray();
        int index = 1;
        for ( const QJsonValue& item: array ) {
            QString text = item.toObject().value( "label" ).toString();
            QLabel* label = new QLabel( text, this );
            layout->addWidget( label, 0, index++ );
        }
    }
}

void AttributeSection::createAttributeRows(QGridLayout *layout)
{
    if ( !m_json.isEmpty() && m_json.object().contains("Atrybuty") ) {
        QJsonArray array = m_json.object().take("Atrybuty").toArray();
        int row = 1;
        for ( const QJsonValue& item: array ) {
            int column = 0;
            layout->addWidget( new QLabel( item.toObject().value("name").toString(), this ), row, column++ );
            if ( !m_json.isEmpty() && m_json.object().contains("PT") ) {
                QJsonArray ptArray = m_json.object().take("PT").toArray();
                for ( const QJsonValue& item: ptArray ) {
                    layout->addWidget( createWidget(item.toObject().value("no").toInt()), row, column++ );
                }
                row++;
            }
        }
    }
}

QWidget *AttributeSection::createWidget(const int &index)
{
    QWidget* widget = nullptr;

    if ( 2 != index )
        widget = new QLabel( "0", this );
    else
        widget = new QSpinBox( this );

    return widget;
}
