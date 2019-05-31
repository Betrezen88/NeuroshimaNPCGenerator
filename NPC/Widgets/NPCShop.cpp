#include "NPCShop.h"

#include "../Utils/DataLoader.h"
#include "NPCShopItem.h"

#include <QGridLayout>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QListWidget>

NPCShop::NPCShop(QWidget *parent)
    : QWidget(parent),
      m_pInventory(new QListWidget(this)),
      m_pShop(new QListWidget(this))
{
    QGridLayout *pLayout = new QGridLayout;
    setLayout( pLayout );

    addItemsToShop();

    pLayout->addWidget( new QLabel("Ekwipunek"), 0, 0 );
    pLayout->addWidget( m_pInventory, 1, 0 );
    pLayout->addWidget( new QLabel("Sklep"), 0, 1 );
    pLayout->addWidget( m_pShop, 1, 1 );
}

void NPCShop::addItemsToShop()
{
    QJsonArray categories = DataLoader::loadJson( ":/data/json/Items.json" );

    for ( const QJsonValue category: categories ) {
        const QJsonObject tCategory = category.toObject();
        for ( const QJsonValue subcategory: tCategory.value("subcategories").toArray() ) {
            const QJsonObject tSubcategory = subcategory.toObject();
            for ( const QJsonValue item: tSubcategory.value("items").toArray() ) {
                QListWidgetItem *pItem = new QListWidgetItem( m_pShop );
                NPCShopItem *pShopItem = new NPCShopItem(item.toObject(), this);
                pItem->setSizeHint( pShopItem->sizeHint() );
                m_pShop->addItem( pItem );
                m_pShop->setItemWidget( pItem, pShopItem );
            }
        }
    }
}
