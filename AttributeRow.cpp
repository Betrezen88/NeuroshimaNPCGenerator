#include "AttributeRow.h"

#include <QJsonValue>
#include <QHBoxLayout>

AttributeRow::AttributeRow(const QString &name, const QJsonArray &array)
    : m_array( array )
{
    m_value = new QSpinBox( this );
    m_value->setFixedWidth( 30 );
    fillLabels();

    connect( m_value, QOverload<int>::of(&QSpinBox::valueChanged), this, &AttributeRow::updateValues );

    QLabel* nameL = new QLabel( name, this );
    nameL->setFixedWidth( 50 );

    QHBoxLayout* all = new QHBoxLayout;
    all->addWidget( nameL );
    all->addWidget( m_labels.at(0) );
    all->addWidget( m_value );
    all->addWidget( m_labels.at(1) );
    all->addWidget( m_labels.at(2) );
    all->addWidget( m_labels.at(3) );
    all->addWidget( m_labels.at(4) );
    all->addWidget( m_labels.at(5) );

    setLayout( all );
}

void AttributeRow::updateValues(const int& value)
{
    m_labels.at(0)->setText( QString::number(value + 2) );
    m_labels.at(1)->setText( QString::number(value - 2) );
    m_labels.at(2)->setText( QString::number(value - 5) );
    m_labels.at(3)->setText( QString::number(value - 8) );
    m_labels.at(4)->setText( QString::number(value - 11) );
    m_labels.at(5)->setText( QString::number(value - 15) );
}

void AttributeRow::fillLabels()
{
    if ( !m_array.isEmpty() ) {
        for ( const QJsonValue& value: m_array )
            if ( value.toObject().value("no").toInt() != 2 ) {
                QLabel* label = new QLabel("0");
                label->setFixedWidth( 30 );
                m_labels.push_back( label );
            }
    }
}
