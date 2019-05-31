#include "NPCShopItem.h"

#include <QFrame>
#include <QGridLayout>
#include <QJsonArray>
#include <QJsonValue>
#include <QLabel>
#include <QPushButton>

NPCShopItem::NPCShopItem(const QJsonObject &item, QWidget *parent)
    : NPCCustomWidget( parent ),
      m_item( item ),
      m_pLayout( new QGridLayout(this) ),
      m_pName( new QLabel(this) ),
      m_pPrice( new QLabel(this) ),
      m_pLine( new QFrame(this) ),
      m_pBuyBtn( new QPushButton("Sprawdź", this) )
{
    m_pLayout->setSpacing( 1 );
    m_pName->setText( m_item.value("name").toString() );
    m_pName->setObjectName( "Name" );
    m_pName->setStyleSheet( m_nameStyle );

    m_pPrice->setText( QString::number(m_item.value("price").toInt())+" gambli" );
    m_pPrice->setObjectName( "Name" );
    m_pPrice->setStyleSheet( m_nameStyle );

    m_pBuyBtn->setFixedSize( 70, 20 );

    m_pLine->setFrameShape( QFrame::HLine );
    m_pLine->setFrameShadow( QFrame::Sunken );
    setLayout( m_pLayout );

    m_pLayout->addWidget( m_pName, m_row, 0, Qt::AlignLeft );
    m_pLayout->addWidget( m_pPrice, m_row++, 2, Qt::AlignRight );
    m_pLayout->addWidget( m_pLine, m_row++, 0, 1, 3 );

    if ( "Broń biała" == m_item.value("type").toString() )
        handWeaponLayout();

    m_pLayout->addWidget( m_pBuyBtn, m_row, 2 );
}

void NPCShopItem::handWeaponLayout()
{
    if ( m_item.contains("bonus") ) {
        QStringList bonuses;
        for ( const QJsonValue bonus: m_item.value("bonus").toArray() )
            bonuses << bonus.toString();
        QLabel *pBonus = new QLabel("Bonus do Zręczności: "+bonuses.join(", "), this);
        m_pLayout->addWidget( pBonus, m_row++, 0, Qt::AlignLeft );
    }

    if ( m_item.contains("requirements") )
        addRequirements();

    addDamage();

    if ( m_item.contains("penetration") )
        addPenetration();

    if ( m_item.contains("special") )
        addSpecial();

    addDescription();
    addAvailability();
}

void NPCShopItem::addRequirements()
{
    QLabel *pRequirements = new QLabel("Wymagania: "+m_item.value("requirements").toString(), this);
    m_pLayout->addWidget( pRequirements, m_row++, 0, 1, 3 );
}

void NPCShopItem::addDamage()
{
    QLabel *pDamage = new QLabel( "Obrażenia: ", this );
    if ( m_item.value("damage").isArray() ) {
        m_pLayout->addWidget( pDamage, m_row, 0 );
        QHBoxLayout *pRow{nullptr};
        QJsonArray damage = m_item.value("damage").toArray();
        for ( int i{0}; i<damage.size(); ++i ) {
            if ( 0 == i%2 )
                pRow = new QHBoxLayout;
            pRow->addWidget( new QLabel(damage.at(i).toString(), this) );
            if ( 1 == i%2 || i == damage.size()-1 )
                m_pLayout->addLayout( pRow, m_row++, 1, 1, 2, Qt::AlignLeft );
        }
    }
    else {
        m_pLayout->addWidget( pDamage, m_row++, 0, 1, 3 );
        pDamage->setText( pDamage->text()+m_item.value("damage").toString() );
    }
}

void NPCShopItem::addPenetration()
{
    QLabel *pPenetration = new QLabel("Punkty przebicia: "
                                      +QString::number(m_item.value("penetration").toInt()), this);
    m_pLayout->addWidget( pPenetration, m_row++, 0, 1, 3 );
}

void NPCShopItem::addSpecial()
{
    m_pLayout->addWidget( new QLabel("Reguly specjalne: ", this), m_row, 0 );
    QHBoxLayout *pRow{nullptr};
    QJsonArray special = m_item.value("special").toArray();
    for ( int i{0}; i<special.size(); ++i ) {
        if ( 0 == i%2 )
            pRow = new QHBoxLayout;
        pRow->addWidget( new QLabel(special.at(i).toString(), this), 0, Qt::AlignLeft );
        if ( 1 == i%2 || i == special.size()-1 )
            m_pLayout->addLayout( pRow, m_row++, 1, 1, 2, Qt::AlignLeft );
    }
}

void NPCShopItem::addDescription()
{
    QLabel *pDescription = new QLabel("Komentarz handlarza");
    pDescription->setToolTip(
                    "<div style=\"width: 200px; word-wrap: break-word;\" align=\"justify\">"
                    + m_item.value("description").toString()+
                    "</div>"
                );
    pDescription->setToolTipDuration( 0 );
    m_pLayout->addWidget( pDescription, m_row, 0);
}

void NPCShopItem::addAvailability()
{
    QLabel *pAvailability = new QLabel("Dostępność: "
                                       +QString::number(m_item.value("availability").toInt()), this);
    m_pLayout->addWidget( pAvailability,
                        m_row++, 2, Qt::AlignRight );
}
