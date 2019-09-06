#include "NPCFriendEquipment.h"

#include "../Utils/DataLoader.h"

#include <QComboBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QListWidget>
#include <QGridLayout>
#include <QPushButton>

NPCFriendEquipment::NPCFriendEquipment(QWidget *parent)
    : QWidget(parent),
      m_pCategory( new QComboBox(this) ),
      m_pEquipment( new QListWidget(this) ),
      m_pShop( new QListWidget(this) ),
      m_pRemoveBtn( new QPushButton("Zwróć", this) ),
      m_pAddBtn( new QPushButton("Kup", this) )
{
    addShopItems();

    QGridLayout *pLayout = new QGridLayout;
    setLayout( pLayout );
    pLayout->addWidget( new QLabel("Ekwipunek"), 0, 0 );
    pLayout->addWidget( m_pEquipment, 1, 0, 4, 1 );
    pLayout->addWidget( new QLabel("Sklep"), 0, 1 );
    pLayout->addWidget( m_pCategory, 1, 1 );
    pLayout->addWidget( m_pShop, 2, 1, 3, 1 );
    pLayout->addWidget( m_pRemoveBtn, 5, 0 );
    pLayout->addWidget( m_pAddBtn, 5, 1 );
}

void NPCFriendEquipment::addShopItems()
{
    QJsonArray array = DataLoader::loadJson( ":/data/json/Items.json" );
    QStringList categories;

    for ( const QJsonValue category: array ) {
        const QJsonObject &tCategory = category.toObject();
        for ( const QJsonValue subcategory: tCategory.value("subcategories").toArray() ) {
            const QJsonObject &tSubcategory = subcategory.toObject();
            for ( const QJsonValue item: tSubcategory.value("items").toArray() ) {
                const QJsonObject &tItem = item.toObject();
                if ( !categories.contains(tItem.value("type").toString()) )
                    categories << tItem.value("type").toString();

                QListWidgetItem *pItem = new QListWidgetItem( tItem.value("name").toString(), m_pShop );
                m_pShop->addItem( pItem );
            }
        }
    }
    m_pCategory->addItems( categories );
}
