#include "WoundsWidget.h"

#include <QJsonValue>

WoundsWidget::WoundsWidget(const QJsonArray &wounds, QWidget *parent)
    : CardWidget ("Rany", parent)
{

    QVBoxLayout *pAll = new QVBoxLayout;
    pAll->addWidget( m_pTitle );
    pAll->addWidget( createWoundRow(wounds) );
    pAll->setSpacing( 0 );

    setLayout( pAll );
}

CardWidget *WoundsWidget::createWoundRow(const QJsonArray &wounds)
{
    CardWidget *pWidget = new CardWidget("",this);
    pWidget->setObjectName("WoundsWidget");
    pWidget->setStyleSheet( m_widgetStyle );
    pWidget->setLayout( new QVBoxLayout );
    pWidget->layout()->setSpacing( 1 );

    for (const QJsonValue &wound: wounds) {
        const QString &name = wound.toString();
        QHBoxLayout *pLayout = new QHBoxLayout;
        QSpinBox *pSpinBox = new QSpinBox(this);
        m_wounds.insert( name, pSpinBox );
        pLayout->addWidget( new QLabel(name, this) );
        pLayout->addWidget( pSpinBox );
        qobject_cast<QVBoxLayout*>( pWidget->layout() )->addLayout(pLayout);
    }

    return pWidget;
}
