#include "NPCShopItem.h"
#include "../Utils/Dice.h"

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
      m_pQuantity(new QLabel(this)),
      m_pLine( new QFrame(this) ),
      m_pButton( new QPushButton(this) )
{
    m_pLayout->setSpacing( 1 );
    m_pName->setText( m_item.value("name").toString() );
    m_pName->setObjectName( "Name" );
    m_pName->setStyleSheet( m_nameStyle );
    m_pName->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );

    const int &price = m_item.value("price").toInt();
    m_pPrice->setText( QString::number(price)+" "+gambleText(price) );
    m_pPrice->setObjectName( "Name" );
    m_pPrice->setStyleSheet( m_nameStyle );

    m_pButton->setFixedSize( 70, 20 );

    m_pLine->setFrameShape( QFrame::HLine );
    m_pLine->setFrameShadow( QFrame::Sunken );
    setLayout( m_pLayout );

    QHBoxLayout *pHeaderL = new QHBoxLayout;
    pHeaderL->addWidget( m_pName, 1, Qt::AlignLeft );
    pHeaderL->addWidget( m_pQuantity, 0, Qt::AlignLeft );
    pHeaderL->addWidget( m_pPrice, 0, Qt::AlignRight );

    m_pLayout->addLayout( pHeaderL, m_row++, 0, 1, 3 );
    m_pLayout->addWidget( m_pLine, m_row++, 0, 1, 3 );

    if ( "Broń biała" == m_item.value("type").toString() )
        handWeaponLayout();
    else if ( "Broń miotana" == m_item.value("type").toString() )
        tossingWeaponLayout();
    else if ( "Pistolet" == m_item.value("type").toString()
              || "Rewolwer" == m_item.value("type").toString()
              || "Pistolet maszynowy" == m_item.value("type").toString()
              || "Karabin powtarzalny" == m_item.value("type").toString()
              || "Karabin samopowtarzalny" == m_item.value("type").toString()
              || "Karabin automatyczny" == m_item.value("type").toString()
              || "Strzelba" == m_item.value("type").toString()
              || "Karabin maszynowy" == m_item.value("type").toString()
              || "Karabin snajperski" == m_item.value("type").toString()
              || "Granatnik" == m_item.value("type").toString()
              || "Broń przeciwpancerna" == m_item.value("type").toString()
              || "Miotacz ognia" == m_item.value("type").toString() )
        gunWeaponLayout();
    else if ( "Zbroja" == m_item.value("type").toString()
              || "Kamizelka" == m_item.value("type").toString()
              || "Hełm" == m_item.value("type").toString() )
        armorLayout();

    if ( m_item.contains("description") )
        addDescription();
    addAvailability();

    addButton();
}

void NPCShopItem::setAvailable(const bool &available)
{
    setEnabled( available );
    if ( available ) {
        setBuyBtn();
        emit checkQuantity( m_item.value("type").toString(), this );
    }
    else
        m_pButton->setText( "Brak" );
}

void NPCShopItem::setQuantity(const int &value)
{
    m_quantity = value;
    m_pQuantity->setText( "(Ilość: "+QString::number(value)+")" );

}

{
}

void NPCShopItem::buyItem()
{
    setQuantity( --m_quantity );
    emit itemBougth( m_item );

void NPCShopItem::setBuyBtn()
{
    m_pButton->setText( "Kup" );
    connect( m_pButton, &QPushButton::clicked,
             this, &NPCShopItem::buyItem );
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

    if ( m_item.contains("durability") )
        addDurability();

    if ( m_item.contains("special") )
        addSpecial();
}

void NPCShopItem::tossingWeaponLayout()
{
    addPenetration();
    addDamage();
}

void NPCShopItem::gunWeaponLayout()
{
    if ( m_item.contains("damage") )
        addDamage();
    if ( m_item.contains("ammo") )
        addAmmo();
    if ( m_item.contains("magazine") )
        addMagazine();
    if ( m_item.contains("special") )
        addSpecial();
}

void NPCShopItem::armorLayout()
{
    addDurability();
    addDefence();
    addPenalty();
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
        QLabel *pSpecial = new QLabel(special.at(i).toString(), this);
        if ( i < special.size()-1 )
            pSpecial->setText( pSpecial->text() + "," );
        pRow->addWidget( pSpecial, 0, Qt::AlignLeft );
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

void NPCShopItem::addAmmo()
{
    QLabel *pAmmo = new QLabel("Nabój: "+m_item.value("ammo").toString(), this);
    m_pLayout->addWidget( pAmmo, m_row++, 0 );
}

void NPCShopItem::addMagazine()
{
    QLabel *pMagazine = new QLabel("Magazynek: "+m_item.value("magazine").toString(),
                                   this);
    m_pLayout->addWidget( pMagazine, m_row++, 0 );
}

void NPCShopItem::addDurability()
{
    QLabel *pDurability = new QLabel("Wytrzymałość: "
                                     +QString::number(
                                         m_item.value("durability").toInt()),
                                     this);
    m_pLayout->addWidget( pDurability, m_row++, 0 );
}

void NPCShopItem::addDefence()
{
    QLabel *pDefence = new QLabel("Redukcja:", this);
    m_pLayout->addWidget( pDefence, m_row++, 0 );
    QJsonArray defence = m_item.value("defence").toArray();
    for ( int i{0}; i<defence.size(); ++i ) {
        const QJsonObject tDefence = defence.at(i).toObject();
        QLabel *pLocation = new QLabel("\t- "+tDefence.value("name").toString()
                                       +" "+QString::number(tDefence.value("value").toInt()));
        if ( tDefence.contains("extra") )
            pLocation->setText( pLocation->text()
                                +" ("+tDefence.value("extra").toString()+")" );
        m_pLayout->addWidget( pLocation, m_row++, 0 );
    }
}

void NPCShopItem::addPenalty()
{
    QLabel *pPenalty = new QLabel("Kara: +"+
                                  QString::number(m_item.value("penalty").toInt())
                                  +"%", this);
    m_pLayout->addWidget( pPenalty, m_row++, 0 );
}

void NPCShopItem::addButton()
{
    if ( 100 > m_item.value("availability").toInt() ) {
    }
    else
}

QString NPCShopItem::gambleText(const int &price)
{
    if ( price == 1 )
        return "gambel";
    else if ( price < 5 )
        return "gamble";
    else
        return "gambli";
}
