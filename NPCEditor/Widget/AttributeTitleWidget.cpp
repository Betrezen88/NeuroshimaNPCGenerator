#include "AttributeTitleWidget.h"

#include "AttributeValueWidget.h"

#include <QHBoxLayout>
#include <QJsonObject>

AttributeTitleWidget::AttributeTitleWidget(const QString &name,
                                           const QJsonArray &modificators,
                                           QWidget *parent)
    : CardWidget("", parent),
      m_pTitleText(new QLabel(name, this))
{
    setObjectName("TitleWidget");
    setStyleSheet( m_widgetStyle );
    setFixedHeight( 50 );
    m_pTitleText->setObjectName( "TitleLabel" );
    m_pTitleText->setStyleSheet( m_labelStyle );
    m_pTitleText->setFixedWidth( 90 );

    QHBoxLayout* pAll = new QHBoxLayout;
    pAll->addWidget( m_pTitleText );
    pAll->addLayout( createLayout(modificators) );
    pAll->setMargin( 5 );

    setLayout( pAll );
}

QHBoxLayout *AttributeTitleWidget::createLayout(const QJsonArray &modificators)
{
    QHBoxLayout *pLayot = new QHBoxLayout;

    for ( const QJsonValue &mod: modificators ) {
        const QJsonObject &obj = mod.toObject();
        const QString &label = obj.value("short").toString();
        const int &value = obj.value("value").toInt();
        AttributeValueWidget *pWidget = new AttributeValueWidget(label, value, this);
        pLayot->addWidget( pWidget );
    }

    pLayot->setSpacing( 2 );
    return  pLayot;
}
