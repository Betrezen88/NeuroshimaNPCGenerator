#include "NPCAttributesModsInfoWidget.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QJsonValue>
#include <QJsonObject>

NPCAttributesModsInfoWidget::NPCAttributesModsInfoWidget(
        const QJsonArray &attributesMods,
        QWidget *parent)
    : QWidget(parent),
      m_pTitleLabel(new QLabel(m_title, this))
{
    setFixedHeight( 80 );
    m_pTitleLabel->setObjectName( "Title" );
    m_pTitleLabel->setAlignment( Qt::AlignCenter );
    m_pTitleLabel->setStyleSheet( m_titleStyle );

    QVBoxLayout *pAll = new QVBoxLayout;
    pAll->setMargin( 0 );
    pAll->setSpacing( 0 );
    setLayout( pAll );

    pAll->addWidget( m_pTitleLabel );
    pAll->addWidget( createInfoWidget(attributesMods) );
}

QWidget *NPCAttributesModsInfoWidget::createInfoWidget(const QJsonArray &attributesMods)
{
    QWidget *pWidget = new QWidget( this );
    pWidget->setObjectName( "ModWidget" );
    pWidget->setStyleSheet( m_modWidgetStyle );

    QGridLayout *pLayout = new QGridLayout;
    pLayout->setSpacing( 2 );

    int col{0};
    for ( const QJsonValue mod: attributesMods ) {
        const QJsonObject &tMod = mod.toObject();
        QLabel *pModNameLabel = new QLabel(tMod.value("short").toString());
        pModNameLabel->setObjectName( "ModName" );
        pModNameLabel->setStyleSheet( m_modNameStyle );

        QLabel *pModValueLabel = new QLabel(QString::number(tMod.value("value").toInt()));
        pModValueLabel->setAlignment( Qt::AlignCenter );
        pModValueLabel->setObjectName( "ModValue" );
        pModValueLabel->setStyleSheet( m_modValueStyle );
        pModValueLabel->setFixedSize( 30, 30 );

        pLayout->addWidget( pModNameLabel, 0, col, Qt::AlignCenter );
        pLayout->addWidget( pModValueLabel, 1, col, Qt::AlignCenter );
        ++col;
    }

    pWidget->setLayout( pLayout );
    return pWidget;
}
