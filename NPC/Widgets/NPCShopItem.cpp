#include "NPCShopItem.h"
#include "../Utils/Dice.h"
#include "../Utils/DataLoader.h"

#include <QFrame>
#include <QGridLayout>
#include <QJsonArray>
#include <QJsonValue>
#include <QLabel>
#include <QPushButton>

#include <QDebug>

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
    connect( this, &NPCShopItem::quantityChanged,
             this, &NPCShopItem::setSoldout );

    m_pLayout->setSpacing( 1 );
    m_pName->setText( m_item.value("name").toString() );
    m_pName->setStyleSheet( m_nameStyle );
    m_pName->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );

    const int &price = m_item.value("price").toInt();
    m_pPrice->setText( QString::number(price)+" "+gambleText(price) );
    m_pPrice->setStyleSheet( m_nameStyle );

    m_pButton->setFixedSize( 80, 20 );

    m_pLine->setFrameShape( QFrame::HLine );
    m_pLine->setFrameShadow( QFrame::Sunken );
    setLayout( m_pLayout );

    QHBoxLayout *pHeaderL = new QHBoxLayout;
    pHeaderL->addWidget( m_pName, 1, Qt::AlignLeft );
    pHeaderL->addWidget( m_pQuantity, 0, Qt::AlignLeft );
    pHeaderL->addWidget( m_pPrice, 0, Qt::AlignRight );

    m_pLayout->addLayout( pHeaderL, m_row++, 0, 1, 4 );
    m_pLayout->addWidget( m_pLine, m_row++, 0, 1, 4 );

    if ( m_item.contains("requirements") )
        m_pLayout->addLayout( addRequirements(), m_row++, 0, 1, 4 );

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

QString NPCShopItem::name() const
{
    return m_item.value("name").toString();
}

int NPCShopItem::quantity() const
{
    return m_quantity;
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
    emit quantityChanged( m_quantity );
}

void NPCShopItem::setSoldout(const int &quantity)
{
    if ( isEnabled() && quantity == 0 ) {
        setEnabled( false );
        m_pButton->setText( "Wyprzedany" );
    }
    else if ( !isEnabled() && quantity > 0) {
        setEnabled( true );
        m_pButton->setText( "Kup" );
    }
}

void NPCShopItem::checkPrice(const int &money)
{
    if ( isEnabled() && money < m_item.value("price").toInt() ) {
        m_pButton->setDisabled( true );
        m_pButton->setText( "Nie stać Cię!" );
    }
    else {
        m_pButton->setEnabled( true );
        m_pButton->setText( "Kup" );
    }
}

void NPCShopItem::buyItem()
{
    setQuantity( --m_quantity );
    emit quantityChanged( m_quantity );
    emit itemBougth( m_item );
}

void NPCShopItem::setBuyBtn()
{
    m_pButton->setText( "Kup" );
    connect( m_pButton, &QPushButton::clicked,
             this, &NPCShopItem::buyItem );
}

void NPCShopItem::handWeaponLayout()
{
    if ( m_item.contains("bonus") )
        m_pLayout->addLayout( addBonus(), m_row++, 0, Qt::AlignLeft );

    m_pLayout->addLayout( addDamage(), m_row++, 0, 1, 4 );

    if ( m_item.contains("penetration") )
        m_pLayout->addLayout( addPenetration(), m_row++, 0, 1, 4 );
    else
        ++m_row;

    if ( m_item.contains("durability") )
        m_pLayout->addLayout( addDurability(), m_row++, 0, 1, 4 );

    if ( m_item.contains("special") )
        m_pLayout->addLayout( addSpecial(), m_row++, 0, 1, 4 );
}

void NPCShopItem::tossingWeaponLayout()
{
    m_pLayout->addLayout( addDamage(), m_row++, 0, 1, 4 );
    m_pLayout->addLayout( addPenetration(), m_row++, 0, 1, 4 );
}

void NPCShopItem::gunWeaponLayout()
{
    if ( m_item.contains("damage") )
        m_pLayout->addLayout( addDamage(), m_row, 0, 1, 2 );
    if ( m_item.contains("penetration") )
        m_pLayout->addLayout( addPenetration(), m_row, 2, 1, 2 );
    m_row++;
    if ( m_item.contains("ammo") )
        m_pLayout->addLayout( addAmmo(), m_row, 0, 1, 2 );
    if ( m_item.contains("magazine") )
         m_pLayout->addLayout( addMagazine(), m_row, 2, 1, 2 );
    m_row++;
    if ( m_item.contains("special") )
        m_pLayout->addLayout( addSpecial(), m_row++, 0, 1, 4 );
}

void NPCShopItem::armorLayout()
{
    m_pLayout->addLayout( addDurability(), m_row++, 0 );
    addDefence();
    m_pLayout->addLayout( addPenalty(), m_row++, 0 );
}

QHBoxLayout *NPCShopItem::addBonus()
{
    QHBoxLayout *pBonusRow = new QHBoxLayout;
    pBonusRow->setSpacing( 5 );
    QLabel *pBonusL = new QLabel("Bonus do Zręczności:", this);
    pBonusL->setStyleSheet( m_infoStyle );
    QStringList bonuses;
    for ( const QJsonValue bonus: m_item.value("bonus").toArray() )
        bonuses << bonus.toString();
    QLabel *pBonusV = new QLabel(bonuses.join(", "), this);
    pBonusV->setStyleSheet( m_valueStyle );
    pBonusRow->addWidget( pBonusL );
    pBonusRow->addWidget( pBonusV );
    return pBonusRow;
}

QHBoxLayout *NPCShopItem::addRequirements()
{
    QHBoxLayout *pRequirementsRow = new QHBoxLayout;
    pRequirementsRow->setSpacing( 5 );
    QLabel *pRequirementL = new QLabel("Wymagania:", this);
    pRequirementL->setStyleSheet( m_infoStyle );
    QLabel *pRequirementV = new QLabel(m_item.value("requirements").toString(), this);
    pRequirementV->setStyleSheet( m_valueStyle );
    pRequirementsRow->addWidget( pRequirementL, 0 );
    pRequirementsRow->addWidget( pRequirementV, 1 );
    return pRequirementsRow;
}

QHBoxLayout *NPCShopItem::addDamage()
{
    QHBoxLayout *pDamageRow = new QHBoxLayout;
    pDamageRow->setSpacing( 5 );
    QLabel *pDamageL = new QLabel( "Obrażenia:", this );
    pDamageL->setStyleSheet( m_infoStyle );
    QLabel *pDamageV = new QLabel( damageText(m_item.value("damage")) );
    pDamageV->setStyleSheet( m_valueStyle );
    pDamageRow->addWidget( pDamageL, 0 );
    pDamageRow->addWidget( pDamageV, 1 );
    return pDamageRow;
}

QHBoxLayout *NPCShopItem::addPenetration()
{
    QHBoxLayout *pPenetrationRow = new QHBoxLayout;
    pPenetrationRow->setSpacing( 5 );
    QLabel *pPenetrationL = new QLabel( "Punkty przebicia:", this );
    pPenetrationL->setStyleSheet( m_infoStyle );
    QLabel *pPenetrationV = new QLabel( QString::number(m_item.value("penetration").toInt()), this );
    pPenetrationV->setStyleSheet( m_valueStyle );
    pPenetrationRow->addWidget( pPenetrationL, 0 );
    pPenetrationRow->addWidget( pPenetrationV, 1 );
    return pPenetrationRow;
}

QHBoxLayout *NPCShopItem::addSpecial()
{
    QHBoxLayout *pSpecialRow = new QHBoxLayout;
    pSpecialRow->setSpacing( 5 );
    QLabel *pSpecialL = new QLabel( "Reguły specjalne:", this );
    pSpecialL->setStyleSheet( m_infoStyle );
    QLabel *pSpecialV = new QLabel(this);
    pSpecialV->setWordWrap( true );
    pSpecialV->setStyleSheet( m_valueStyle );
    pSpecialRow->addWidget( pSpecialL, 0 );
    pSpecialRow->addWidget( pSpecialV, 1 );

    QJsonArray specials = DataLoader::loadJson(":/data/json/WeaponSpecials.json");
    QString specialText, tooltip = "<div style=\"width: 450px;\">";
    for ( const QJsonValue special: m_item.value("special").toArray() ) {
        specialText += special.toString();
        for ( const QJsonValue s: specials ) {
            const QJsonObject tSpecial = s.toObject();
            if ( tSpecial.value("name").toString() == special.toString() ) {
                tooltip += "<b>"+special.toString()+"</b> - "
                        + tSpecial.value("description").toString();
                break;
            }
        }
        if ( special != m_item.value("special").toArray().last() ) {
            specialText += ", ";
            tooltip += "<br><br>";
        }
    }
    tooltip += "</div>";
    pSpecialV->setText( specialText );
    pSpecialV->setToolTip( tooltip );
    pSpecialV->setToolTipDuration( 5*60*100 );
    return pSpecialRow;
}

void NPCShopItem::addDescription()
{
    QLabel *pDescription = new QLabel("Komentarz handlarza");
    pDescription->setStyleSheet( m_infoStyle );
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
                          m_row++, 3, Qt::AlignRight );
}

QHBoxLayout *NPCShopItem::addAmmo()
{
    QHBoxLayout *pAmmoRow = new QHBoxLayout;
    pAmmoRow->setSpacing( 5 );
    QLabel *pAmmoL = new QLabel("Nabój:", this);
    pAmmoL->setStyleSheet( m_infoStyle );
    QLabel *pAmmoV = new QLabel(m_item.value("ammo").toString(), this);
    pAmmoV->setStyleSheet( m_valueStyle );
    pAmmoRow->addWidget( pAmmoL, 0 );
    pAmmoRow->addWidget( pAmmoV, 1 );
    return pAmmoRow;
}

QHBoxLayout *NPCShopItem::addMagazine()
{
    QHBoxLayout *pMagazineRow = new QHBoxLayout;
    pMagazineRow->setSpacing( 5 );
    QLabel *pMagazineL = new QLabel("Magazynek:", this);
    pMagazineL->setStyleSheet( m_infoStyle );
    QLabel *pMagazineV = new QLabel(m_item.value("magazine").toString(), this);
    pMagazineV->setStyleSheet ( m_valueStyle );
    pMagazineRow->addWidget( pMagazineL, 0 );
    pMagazineRow->addWidget( pMagazineV, 1 );
    return pMagazineRow;
}

QHBoxLayout *NPCShopItem::addDurability()
{
    QHBoxLayout *pDurabilityRow = new QHBoxLayout;
    pDurabilityRow->setSpacing( 5 );
    QLabel *pDurabilityL = new QLabel("Wytrzymałość:", this);
    pDurabilityL->setStyleSheet( m_infoStyle );
    QLabel *pDurabilityV = new QLabel(QString::number(m_item.value("durability").toInt()));
    pDurabilityV->setStyleSheet( m_valueStyle );
    pDurabilityRow->addWidget( pDurabilityL, 0 );
    pDurabilityRow->addWidget( pDurabilityV, 1 );
    return pDurabilityRow;
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

QHBoxLayout *NPCShopItem::addPenalty()
{
    QHBoxLayout *pPenaltyRow = new QHBoxLayout;
    pPenaltyRow->setSpacing( 5 );
    QLabel *pPenaltyL = new QLabel("Kara:", this);
    pPenaltyL->setStyleSheet( m_infoStyle );
    QLabel *pPenaltyV = new QLabel("+"+QString::number(m_item.value("penalty").toInt())+"%");
    pPenaltyV->setStyleSheet( m_valueStyle );
    pPenaltyRow->addWidget( pPenaltyL, 0 );
    pPenaltyRow->addWidget( pPenaltyV, 1 );
    return pPenaltyRow;
}

void NPCShopItem::addButton()
{
    if ( 100 > m_item.value("availability").toInt() ) {
        m_pButton->setText( "Sprawdź" );
        connect( m_pButton, &QPushButton::clicked,
                 [this](){
            emit this->checkAvailability(m_item.value("availability").toInt(), this);
        } );
    }
    else {
        setBuyBtn();
    }
    m_pLayout->addWidget( m_pButton, m_row, 3, Qt::AlignRight );
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

QString NPCShopItem::damageText(const QJsonValue &value)
{
    QString result;
    if ( value.isArray() ) {
        for ( const QJsonValue v: value.toArray() ) {
            result += v.toString();
            if ( v != value.toArray().last() )
                result += "\n";
        }
    }
    else {
        result = value.toString();
    }
    return result;
}
