#include "AttributeSection.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <QLabel>
#include <QSpinBox>

#include "AttributeRow.h"

AttributeSection::AttributeSection(const QString &name, const QJsonDocument &json, QWidget *parent)
    : Section(json, parent)
{
    QVBoxLayout* all = new QVBoxLayout;
    all->addWidget( create(name) );

    setLayout( all );
}

QGroupBox *AttributeSection::create(const QString &name)
{
    QGroupBox* groupBox = new QGroupBox( name );

    QVBoxLayout* attributesL = new QVBoxLayout;
    attributesL->addLayout( createLabelRow() );
    createAttributeRows( attributesL );

    groupBox->setLayout( attributesL );

    return groupBox;
}

QHBoxLayout* AttributeSection::createLabelRow()
{
    QHBoxLayout* rowL = new QHBoxLayout;
    QLabel* empty = new QLabel( "PT", this );
    empty->setFixedWidth( 50 );
    rowL->addWidget( empty );

    if ( !m_json.isEmpty() && m_json.object().contains("PT") ) {
        QJsonArray array = m_json.object().take("PT").toArray();
        for ( const QJsonValue& item: array ) {
            QString text = item.toObject().value( "label" ).toString();
            QLabel* label = new QLabel( text, this );
            label->setFixedWidth( 30 );
            rowL->addWidget( label );
        }
    }

    return rowL;
}

void AttributeSection::createAttributeRows(QVBoxLayout *layout)
{
    if ( !m_json.isEmpty() && m_json.object().contains("Atrybuty") ) {
        QJsonArray array = m_json.object().take("Atrybuty").toArray();
        QJsonArray ptArray = m_json.object().take("PT").toArray();
        int row = 1;
        for ( const QJsonValue& item: array ) {
            QString name = item.toObject().value("name").toString();
            AttributeRow* arow = new AttributeRow(name, ptArray);
            layout->addWidget( arow , row, 0);
            row++;
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
