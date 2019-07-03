#include "NPCArmor.h"

#include <QGridLayout>
#include <QLabel>
#include <QJsonArray>

NPCArmor::NPCArmor(QWidget *parent)
    : NPCCustomWidget(parent),
      m_pTitle( new QLabel("Pancerz", this) ),
      m_pHead( new QLabel("0", this) ),
      m_pHeadItem( new QLabel(this) ),
      m_pTorso( new QLabel("0", this) ),
      m_pTorsoItem( new QLabel(this) ),
      m_pLeftHand( new QLabel("0", this) ),
      m_pLeftHandItem( new QLabel(this) ),
      m_pRightHand( new QLabel("0", this) ),
      m_pRightHandItem( new QLabel(this) ),
      m_pLeftLeg( new QLabel("0", this) ),
      m_pLeftLegItem( new QLabel(this) ),
      m_pRightLeg( new QLabel("0", this) ),
      m_pRightLegItem( new QLabel(this) )
{
    m_pTitle->setAlignment( Qt::AlignCenter );
    m_pTitle->setObjectName( "Title" );
    m_pTitle->setStyleSheet( m_titleStyle );

    QLabel *pHandsL = new QLabel( "Ręce", this );
    pHandsL->setAlignment( Qt::AlignCenter );
    QLabel *pLegsL = new QLabel( "Nogi", this );
    pLegsL->setAlignment( Qt::AlignCenter );

    QGridLayout *pLayout = new QGridLayout;
    pLayout->setSpacing( 1 );
    setLayout( pLayout );

    QHBoxLayout *pHeadL = new QHBoxLayout;
    pHeadL->setSpacing( 1 );
    pHeadL->addWidget( new QLabel("Głowa:"), 0, Qt::AlignLeft );
    pHeadL->addWidget( m_pHead, 0, Qt::AlignLeft );
    pHeadL->addWidget( m_pHeadItem, 1, Qt::AlignLeft );

    QHBoxLayout *pTorsoL = new QHBoxLayout;
    pTorsoL->setSpacing( 1 );
    pTorsoL->addWidget( new QLabel("Tułów:"), 0, Qt::AlignLeft );
    pTorsoL->addWidget( m_pTorso, 0, Qt::AlignLeft );
    pTorsoL->addWidget( m_pTorsoItem, 1, Qt::AlignLeft );

    QHBoxLayout *pLeftHandL = new QHBoxLayout;
    pLeftHandL->setSpacing( 1 );
    pLeftHandL->addWidget( new QLabel("Lewa:"), 0, Qt::AlignLeft );
    pLeftHandL->addWidget( m_pLeftHand, 0, Qt::AlignLeft );
    pLeftHandL->addWidget( m_pLeftHandItem, 1, Qt::AlignLeft );

    QHBoxLayout *pRightHandL = new QHBoxLayout;
    pRightHandL->setSpacing( 1 );
    pRightHandL->addWidget( new QLabel("Prawa:"), 0, Qt::AlignLeft );
    pRightHandL->addWidget( m_pRightHand, 0, Qt::AlignLeft );
    pRightHandL->addWidget( m_pRightHandItem, 1, Qt::AlignLeft );

    QHBoxLayout *pLeftLegL = new QHBoxLayout;
    pLeftLegL->setSpacing( 1 );
    pLeftLegL->addWidget( new QLabel("Lewa:"), 0, Qt::AlignLeft );
    pLeftLegL->addWidget( m_pLeftLeg, 0, Qt::AlignLeft );
    pLeftLegL->addWidget( m_pLeftLegItem, 1, Qt::AlignLeft );

    QHBoxLayout *pRightLegL = new QHBoxLayout;
    pRightLegL->setSpacing( 1 );
    pRightLegL->addWidget( new QLabel("Prawa:"), 0, Qt::AlignLeft );
    pRightLegL->addWidget( m_pRightLeg, 0, Qt::AlignLeft );
    pRightLegL->addWidget( m_pRightLegItem, 1, Qt::AlignLeft );

    pLayout->addWidget( m_pTitle, 0, 0, 1, 2 );
    pLayout->addLayout( pHeadL, 1, 0 );
    pLayout->addLayout( pTorsoL, 1, 1 );
    pLayout->addWidget( pHandsL, 2, 0 );
    pLayout->addLayout( pLeftHandL, 3, 0 );
    pLayout->addLayout( pRightHandL, 4, 0 );
    pLayout->addWidget( pLegsL, 2, 1 );
    pLayout->addLayout( pLeftLegL, 3, 1 );
    pLayout->addLayout( pRightLegL, 4, 1 );
}

void NPCArmor::addArmor(const QJsonObject &item)
{
    QString name = item.value("name").toString();

    for ( const QJsonValue defence: item.value("defence").toArray() ) {
        const QJsonObject &tDefence = defence.toObject();
        if ( "Głowa" == tDefence.value("name").toString() ) {
            m_pHead->setText( QString::number(tDefence.value("value").toInt()) );
            m_pHeadItem->setText( "("+name+")" );
        }
        else if ( "Tułów" == tDefence.value("name").toString() ) {
            m_pTorso->setText( QString::number(tDefence.value("value").toInt()) );
            m_pTorsoItem->setText( "("+name+")" );
        }
        else if ( "Ręce" == tDefence.value("name").toString() ) {
            m_pLeftHand->setText( QString::number(tDefence.value("value").toInt()) );
            m_pLeftHandItem->setText( "("+name+")" );
            m_pRightHand->setText( QString::number(tDefence.value("value").toInt()) );
            m_pRightHandItem->setText( "("+name+")" );
        }
        else if ( "Nogi" == tDefence.value("name").toString() ) {
            m_pLeftLeg->setText( QString::number(tDefence.value("value").toInt()) );
            m_pLeftLegItem->setText( "("+name+")" );
            m_pRightLeg->setText( QString::number(tDefence.value("value").toInt()) );
            m_pRightLegItem->setText( "("+name+")" );
        }
    }
}
