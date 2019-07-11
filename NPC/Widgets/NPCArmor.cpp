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

    QLabel *pHandsL = infoLabel( "Ręce", "Subtitle", m_subtitleStyle );
    pHandsL->setAlignment( Qt::AlignCenter );
    QLabel *pLegsL = infoLabel( "Nogi", "Subtitle", m_subtitleStyle );
    pLegsL->setAlignment( Qt::AlignCenter );

    QGridLayout *pLayout = new QGridLayout;
    pLayout->setSpacing( 1 );
    setLayout( pLayout );

    QHBoxLayout *pHeadL = new QHBoxLayout;
    pHeadL->setSpacing( 1 );
    pHeadL->addWidget( infoLabel("Głowa:", "Info", m_infoStyle), 0, Qt::AlignLeft );
    pHeadL->addWidget( m_pHead, 0, Qt::AlignLeft );
    pHeadL->addWidget( m_pHeadItem, 1, Qt::AlignLeft );

    QHBoxLayout *pTorsoL = new QHBoxLayout;
    pTorsoL->setSpacing( 1 );
    pTorsoL->addWidget( infoLabel("Tułów:", "Info", m_infoStyle), 0, Qt::AlignLeft );
    pTorsoL->addWidget( m_pTorso, 0, Qt::AlignLeft );
    pTorsoL->addWidget( m_pTorsoItem, 1, Qt::AlignLeft );

    QHBoxLayout *pLeftHandL = new QHBoxLayout;
    pLeftHandL->setSpacing( 1 );
    pLeftHandL->addWidget( infoLabel("Lewa:", "Info", m_infoStyle), 0, Qt::AlignLeft );
    pLeftHandL->addWidget( m_pLeftHand, 0, Qt::AlignLeft );
    pLeftHandL->addWidget( m_pLeftHandItem, 1, Qt::AlignLeft );

    QHBoxLayout *pRightHandL = new QHBoxLayout;
    pRightHandL->setSpacing( 1 );
    pRightHandL->addWidget( infoLabel("Prawa:", "Info", m_infoStyle), 0, Qt::AlignLeft );
    pRightHandL->addWidget( m_pRightHand, 0, Qt::AlignLeft );
    pRightHandL->addWidget( m_pRightHandItem, 1, Qt::AlignLeft );

    QHBoxLayout *pLeftLegL = new QHBoxLayout;
    pLeftLegL->setSpacing( 1 );
    pLeftLegL->addWidget( infoLabel("Lewa:", "Info", m_infoStyle), 0, Qt::AlignLeft );
    pLeftLegL->addWidget( m_pLeftLeg, 0, Qt::AlignLeft );
    pLeftLegL->addWidget( m_pLeftLegItem, 1, Qt::AlignLeft );

    QHBoxLayout *pRightLegL = new QHBoxLayout;
    pRightLegL->setSpacing( 1 );
    pRightLegL->addWidget( infoLabel("Prawa:", "Info", m_infoStyle), 0, Qt::AlignLeft );
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

    QJsonArray defence = item.value("defence").toArray();
    QJsonArray overlaping = overlapingArmor( defence );
    for ( const QJsonValue armor: overlaping ) {
        removeArmor( armor.toObject() );
    }

    for ( const QJsonValue defence: item.value("defence").toArray() ) {
        const QJsonObject &tDefence = defence.toObject();
        const QString location = tDefence.value("name").toString();
        addArmorPiece( location, name, tDefence.value("value").toInt() );
        m_equiped.insert( location, item );
    }
}

QLabel *NPCArmor::infoLabel(QString text, QString name, QString style)
{
    QLabel *pLabel = new QLabel(text, this);
    pLabel->setObjectName( name );
    pLabel->setStyleSheet( style );
    return pLabel;
}

void NPCArmor::removeArmor(const QJsonObject &item)
{
    emit unequip( item );
    for ( const QJsonValue defence: item.value("defence").toArray() ) {
        const QString &location = defence.toObject().value("name").toString();
        removeArmorPiece( location );
        m_equiped.remove( location );
    }
}

void NPCArmor::addArmorPiece(const QString &location, const QString &name, const int &value)
{
    if ( "Głowa" == location ) {
        m_pHead->setText( QString::number(value) );
        m_pHeadItem->setText( "("+name+")" );
    }
    else if ( "Tułów" == location ) {
        m_pTorso->setText( QString::number(value) );
        m_pTorsoItem->setText( "("+name+")" );
    }
    else if ( "Ręce" == location ) {
        m_pLeftHand->setText( QString::number(value) );
        m_pLeftHandItem->setText( "("+name+")" );
        m_pRightHand->setText( QString::number(value) );
        m_pRightHandItem->setText( "("+name+")" );
    }
    else if ( "Nogi" == location ) {
        m_pLeftLeg->setText( QString::number(value) );
        m_pLeftLegItem->setText( "("+name+")" );
        m_pRightLeg->setText( QString::number(value) );
        m_pRightLegItem->setText( "("+name+")" );
    }
}

void NPCArmor::removeArmorPiece(const QString &location)
{
    if ( "Głowa" == location ) {
        m_pHead->setText( "0" );
        m_pHeadItem->setText( "" );
    }
    else if ( "Tułów" == location ) {
        m_pTorso->setText( "0" );
        m_pTorsoItem->setText( "" );
    }
    else if ( "Ręce" == location ) {
        m_pLeftHand->setText( "0" );
        m_pLeftHandItem->setText( "" );
        m_pRightHand->setText( "0" );
        m_pRightHandItem->setText( "" );
    }
    else if ( "Nogi" == location ) {
        m_pLeftLeg->setText( "0" );
        m_pLeftLegItem->setText( "" );
        m_pRightLeg->setText( "0" );
        m_pRightLegItem->setText( "" );
    }
}

QJsonArray NPCArmor::overlapingArmor(QJsonArray &defence)
{
    QJsonArray overlaping;

    for ( const QJsonValue location: defence ) {
        const QString &name = location.toObject().value("name").toString();
        if ( m_equiped.contains(name) )
            overlaping.push_back( m_equiped.value(name) );
    }

    return overlaping;
}
