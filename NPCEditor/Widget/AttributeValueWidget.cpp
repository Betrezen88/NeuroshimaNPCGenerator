#include "AttributeValueWidget.h"

#include <QVBoxLayout>

AttributeValueWidget::AttributeValueWidget(const QString &name, QWidget *parent)
    : CardWidget("", parent)
{
    setObjectName("ValueWidget");
    setStyleSheet(m_widgetStyle);
    setFixedSize( 30, 40 );

    m_pNameLabel = new QLabel( name, this );
    m_pValueLabel = new QLabel( "0", this );

    m_pNameLabel->setObjectName("Label");
    m_pNameLabel->setStyleSheet( m_labelStyle );
    m_pValueLabel->setObjectName("Value");
    m_pValueLabel->setStyleSheet( m_valueStyle );
    m_pValueLabel->setAlignment(Qt::AlignHCenter);

    QVBoxLayout *pAll = new QVBoxLayout;
    pAll->addWidget( m_pNameLabel, 0, Qt::AlignHCenter );
    pAll->addWidget( m_pValueLabel );
    pAll->setSpacing( 0 );
    pAll->setMargin( 0 );

    setLayout( pAll );
}
