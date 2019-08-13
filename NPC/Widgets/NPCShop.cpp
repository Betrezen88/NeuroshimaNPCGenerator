#include "NPCShop.h"

#include "../Utils/DataLoader.h"
#include "../Utils/Dice.h"
#include "NPCShopItem.h"
#include "NPCInventory.h"
#include "NPCItem.h"

#include <QComboBox>
#include <QGridLayout>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QListWidget>

NPCShop::NPCShop(QWidget *parent)
    : QWidget(parent),
      m_pCategory(new QComboBox(this)),
      m_pSubcategory(new QComboBox(this)),
      m_pInventory(new NPCInventory(this)),
      m_pShop(new QListWidget(this)),
      m_pMoney(new QLabel("100", this))
{
    connect( m_pCategory, &QComboBox::currentTextChanged,
             this, &NPCShop::setSubcategory );
    connect( m_pSubcategory, &QComboBox::currentTextChanged,
             this, &NPCShop::showItems );
    connect( m_pInventory, &NPCInventory::returnItemToShop,
             this, &NPCShop::returnItemToShop );

    addItemsToShop();

    QHBoxLayout *pGambleRow = new QHBoxLayout;
    QLabel *pGampleL = new QLabel( "Gamble:", this );
    pGampleL->setStyleSheet( "font: bold 10pt;" );
    pGambleRow->addSpacerItem( new QSpacerItem(10, 1) );
    pGambleRow->addWidget( pGampleL );
    pGambleRow->addWidget( m_pMoney );

    QVBoxLayout *pLeftColumn = new QVBoxLayout;
    pLeftColumn->addWidget( m_pInventory );
    pLeftColumn->addLayout( pGambleRow );

    QLabel *pShopL = new QLabel( "Sklep", this );
    pShopL->setAlignment( Qt::AlignHCenter );
    pShopL->setObjectName( "Title" );
    pShopL->setStyleSheet( m_titleStyle );
    QLabel *pCategoryL = new QLabel( "Kategoria:", this );
    pCategoryL->setStyleSheet( "font: bold 10pt;" );
    QLabel *pSubcategoryL = new QLabel( "Podkategoria:", this );
    pSubcategoryL->setStyleSheet( "font: bold 10pt;" );

    QHBoxLayout *pCategoryRow = new QHBoxLayout;
    pCategoryRow->addWidget( pCategoryL );
    pCategoryRow->addWidget( m_pCategory );

    QHBoxLayout *pSubcategoryRow = new QHBoxLayout;
    pSubcategoryRow->addWidget( pSubcategoryL );
    pSubcategoryRow->addWidget( m_pSubcategory );

    QVBoxLayout *pRightColumn = new QVBoxLayout;
    pRightColumn->addSpacerItem( new QSpacerItem(1, 11) );
    pRightColumn->addWidget( pShopL );
    pRightColumn->addLayout( pCategoryRow );
    pRightColumn->addLayout( pSubcategoryRow );
    pRightColumn->addWidget( m_pShop );

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addLayout( pLeftColumn );
    pLayout->addLayout( pRightColumn );

    setLayout( pLayout );
}

NPCInventory *NPCShop::inventory() const
{
    return m_pInventory;
}

void NPCShop::addItemToInventory(const QJsonObject &item)
{
    NPCItem *pInvItem = new NPCItem( item, NPCItem::Type::SHOP, this );
    m_pInventory->addItem( pInvItem );
    m_pMoney->setText( QString::number(m_pMoney->text().toInt() - item.value("price").toInt()) );
    emit moneyValueChanged( m_pMoney->text().toInt() );
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
        connect( this, &NPCShop::moneyValueChanged,
                 item, &NPCShopItem::checkPrice );
        item->checkPrice( m_pMoney->text().toInt() );
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

void NPCShop::returnItemToShop(const QString &name, const int &value)
{
    NPCShopItem *pItem = findItemInShopByName(name);
    if ( pItem ) {
        pItem->setQuantity( pItem->quantity()+1 );
        m_pMoney->setText( QString::number(m_pMoney->text().toInt()+value) );
        emit moneyValueChanged( m_pMoney->text().toInt() );
    }
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

                connect( pShopItem, &NPCShopItem::itemBougth,
                         this, &NPCShop::addItemToInventory );
                connect( pShopItem, &NPCShopItem::checkAvailability,
                         this, &NPCShop::checkItemAvailability );
                connect( pShopItem, &NPCShopItem::checkQuantity,
                         this, &NPCShop::checkItemQuantity );

                if ( 100 == item.toObject().value("availability").toInt() )
                    checkItemQuantity( item.toObject().value("type").toString(), pShopItem );

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

NPCShopItem *NPCShop::findItemInShopByName(const QString &name)
{
    NPCShopItem *pItem{nullptr};
    for ( int i{0}; i<m_pShop->count(); ++i ) {
        NPCShopItem *pTmpItem = qobject_cast<NPCShopItem*>( m_pShop->itemWidget(m_pShop->item(i)) );
        if ( pTmpItem && (name == pTmpItem->name()) ) {
            pItem = pTmpItem;
            break;
        }
    }
    return pItem;
}
