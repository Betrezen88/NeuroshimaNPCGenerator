#include "AttributeTitleWidget.h"

#include "AttributeValueWidget.h"

#include <QHBoxLayout>

AttributeTitleWidget::AttributeTitleWidget(const QString &name, QWidget *parent)
    : CardWidget("", parent),
      m_pTitleText(new QLabel(name, this))
{
    setObjectName("TitleWidget");
    m_pTitleText->setObjectName( "TitleLabel" );
    m_pTitleText->setStyleSheet( m_labelStyle );
    setStyleSheet( m_widgetStyle );

    QHBoxLayout* pAll = new QHBoxLayout;
    pAll->addWidget( m_pTitleText );
    pAll->addLayout( createLayout() );
    pAll->setMargin( 5 );

    setLayout( pAll );
}

QHBoxLayout *AttributeTitleWidget::createLayout()
{
    QHBoxLayout *pLayot = new QHBoxLayout;

    for ( const QString &label: list ) {
        AttributeValueWidget *pWidget = new AttributeValueWidget(label, this);
        pLayot->addWidget( pWidget );
    }

    pLayot->setSpacing( 2 );
    return  pLayot;
}
