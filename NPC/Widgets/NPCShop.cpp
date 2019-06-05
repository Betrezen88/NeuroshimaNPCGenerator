#include "NPCShop.h"

#include "../Utils/DataLoader.h"
#include "NPCShopItem.h"

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
