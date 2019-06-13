#include "NPCShop.h"

#include "../Utils/DataLoader.h"
#include "NPCShopItem.h"
#include "NPCItem.h"

#include <QComboBox>
#include <QGridLayout>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QListWidget>

#include <QDebug>

NPCShop::NPCShop(QWidget *parent)
    : QWidget(parent),
      m_pCategory(new QComboBox(this)),
      m_pSubcategory(new QComboBox(this)),
      m_pInventory(new QListWidget(this)),
      m_pShop(new QListWidget(this)),
      m_pMoney(new QLabel("100", this))
{
    connect( m_pCategory, &QComboBox::currentTextChanged,
             this, &NPCShop::setSubcategory );
    connect( m_pSubcategory, &QComboBox::currentTextChanged,
             this, &NPCShop::showItems );

    QGridLayout *pLayout = new QGridLayout;
    pLayout->setSpacing( 1 );
    setLayout( pLayout );

    addItemsToShop();

    pLayout->addWidget( new QLabel("Ekwipunek"), 0, 0, 1, 2 );
    pLayout->addWidget( new QLabel("Gamble"), 1, 0, 1, 1 );
    pLayout->addWidget( m_pMoney, 1, 1, 1, 1 );
    pLayout->addWidget( m_pInventory, 2, 0, 6, 2 );
    pLayout->addWidget( new QLabel("Sklep"), 0, 2, 1, 2 );
    pLayout->addWidget( new QLabel("Kategoria"), 1, 2, 1, 1 );
    pLayout->addWidget( m_pCategory, 1, 3, 1, 1 );
    pLayout->addWidget( new QLabel("Podkategoria"), 2, 2, 1, 1 );
    pLayout->addWidget( m_pSubcategory, 2, 3, 1, 1 );
    pLayout->addWidget( m_pShop, 3, 2, 5, 2 );
}

void NPCShop::addItemToInventory(const QJsonObject &item)
{
    NPCItem *pInvItem = findItemInInventoryByName( item.value("name").toString() );

    if ( pInvItem ) {
        pInvItem->increaseQuantity();
    }
    else {
        QListWidgetItem *pItem = new QListWidgetItem( m_pInventory );
        pInvItem = new NPCItem( item, this );
        m_pInventory->addItem( pItem );
        m_pInventory->setItemWidget( pItem, pInvItem );
        pItem->setSizeHint( pInvItem->sizeHint() );
    }
    m_pMoney->setText( QString::number(m_pMoney->text().toInt() - item.value("price").toInt()) );
}

void NPCShop::setSubcategory(const QString &categoryName)
{
    m_pSubcategory->clear();
    m_pSubcategory->addItems( m_items.value(categoryName).keys() );
}

void NPCShop::showItems(const QString &subcategoryName)
{
    if ( !subcategoryName.isEmpty() ) {
        hideItems();

        for ( QListWidgetItem *pItem: m_items.value(m_pCategory->currentText()).value(subcategoryName) )
            pItem->setHidden( false );
    }
}

void NPCShop::checkItemAvailability(const int &availability, NPCShopItem *item)
{
    Dice k100{100};
    bool available = availability >= static_cast<int>(k100.throwValue());
    item->setAvailable( available );
    if ( available ) {
        disconnect( item, &NPCShopItem::checkAvailability,
                    this, &NPCShop::checkItemAvailability );
    }
}

void NPCShop::checkItemQuantity(const QString &type, NPCShopItem *item)
{
    Dice k10{10};
    int quantity{0};
    if ( "ammo" == type ) {
        quantity = static_cast<int>( k10.throwValue() + k10.throwValue() + k10.throwValue() );
    }
    else if ( "Broń biała" == type || "Broń miotana" == type
              || "Pistolet" == type || "Rewolwer" == type
              || "Pistolet maszynowy" == type || "Karabin powtarzalny" == type
              || "Karabin samopowtarzalny" == type || "Karabin automatyczny" == type
              || "Strzelba" == type || "Karabin maszynowy" == type
              || "Karabin snajperski" == type || "Granatnik" == type
              || "Broń przeciwpancerna" == type || "Miotacz ognia" == type )
        quantity = ( 7>=static_cast<int>(k10.throwValue()) ) ? 1: 2;
    else if ( "Zbroja" == type || "Kamizelka" == type
              || "Hełm" == type )
        quantity = ( 6>static_cast<int>(k10.throwValue()) ) ? 1: 2;
    else if ( "Używka" == type )
        quantity = static_cast<int>( k10.throwValue() + k10.throwValue() + k10.throwValue() );
    item->setQuantity( quantity );
}

void NPCShop::addItemsToShop()
{
    QJsonArray categories = DataLoader::loadJson( ":/data/json/Items.json" );

    for ( const QJsonValue category: categories ) {
        const QJsonObject tCategory = category.toObject();
        QHash<QString, QVector<QListWidgetItem*>> subcategories;

        for ( const QJsonValue subcategory: tCategory.value("subcategories").toArray() ) {
            const QJsonObject tSubcategory = subcategory.toObject();

            QVector<QListWidgetItem*> items;
            for ( const QJsonValue item: tSubcategory.value("items").toArray() ) {
                QListWidgetItem *pItem = new QListWidgetItem(m_pShop);
                NPCShopItem *pShopItem = new NPCShopItem(item.toObject(), this);
                m_pShop->addItem( pItem );
                m_pShop->setItemWidget( pItem, pShopItem );
                pItem->setSizeHint( pShopItem->sizeHint() );
                items.push_back( pItem );
            }
            subcategories.insert( tSubcategory.value("name").toString(), items );
        }
        m_items.insert( tCategory.value("category").toString(), subcategories );
    }

    m_pCategory->addItems( m_items.keys() );
}

void NPCShop::hideItems()
{
    for ( int i{0}; i<m_pShop->count(); ++i )
        m_pShop->item(i)->setHidden( true );
}

NPCItem *NPCShop::findItemInInventoryByName(const QString &name)
{
    NPCItem *pItem{nullptr};
    for ( int i{0}; i<m_pInventory->count(); ++i ) {
        NPCItem *pTmpItem = static_cast<NPCItem*>( m_pInventory->itemWidget(m_pInventory->item(i)) );
        if ( name == pTmpItem->name() )
            pItem = pTmpItem;
    }
    return pItem;
}
