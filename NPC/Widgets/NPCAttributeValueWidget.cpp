#include "NPCAttributeValueWidget.h"

#include <QPainter>
#include <QStyleOption>
#include <QVBoxLayout>

NPCAttributeValueWidget::NPCAttributeValueWidget(const QString &modName,
                                                 const int &modValue,
                                                 QWidget *parent)
    : QWidget(parent),
      m_pNameLabel( new QLabel(modName, this) ),
      m_pValueLabel( new QLabel("", this) ),
      m_modValue( modValue )
{
    setFixedSize( 30, 35 );
    setObjectName( "ValueStyle" );
    setStyleSheet( m_valueStyle );

    m_pNameLabel->setObjectName( "LabelStyle" );
    m_pNameLabel->setStyleSheet( m_nameStyle );

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( m_pNameLabel, 0, Qt::AlignHCenter );
    pLayout->addWidget( m_pValueLabel, 0, Qt::AlignHCenter );
    pLayout->setSpacing( 1 );
    pLayout->setMargin( 0 );
    setLayout( pLayout );
}

void NPCAttributeValueWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void NPCAttributeValueWidget::updateValueLabel(const int &value)
{
    int tValue = value + m_modValue;
    m_pValueLabel->setText( (tValue > 0) ? QString::number(tValue) : "NA" );
}
