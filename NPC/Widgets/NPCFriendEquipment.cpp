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
#include <QtMath>

#include <QDebug>

NPCFriendEquipment::NPCFriendEquipment(QWidget *parent)
    : QWidget(parent),
      m_pCategory( new QComboBox(this) ),
      m_pEquipment( new QListWidget(this) ),
      m_pShop( new QListWidget(this) ),
      m_pRemoveBtn( new QPushButton("Zwróć", this) ),
      m_pAddBtn( new QPushButton("Kup", this) )
{
    connect( m_pCategory, &QComboBox::currentTextChanged,
             this, &NPCFriendEquipment::showCategoryItems );
    connect( m_pShop, &QListWidget::itemClicked,
             [this](QListWidgetItem *item){
        if ( item->flags() == QFlags<Qt::ItemFlag>(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled
                                                    |Qt::ItemIsUserCheckable|Qt::ItemIsEnabled) )
            this->m_pAddBtn->setEnabled( true );
    } );
    connect( m_pEquipment, &QListWidget::itemClicked,
             [this](QListWidgetItem *item){
        if ( item->flags() == QFlags<Qt::ItemFlag>(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled
                                                    |Qt::ItemIsUserCheckable|Qt::ItemIsEnabled) )
            this->m_pRemoveBtn->setEnabled( true ); }
    );
    connect( m_pAddBtn, &QPushButton::clicked,
             this, &NPCFriendEquipment::buyItem );
    connect( m_pRemoveBtn, &QPushButton::clicked,
             this, &NPCFriendEquipment::sellItem );

    addShopItems();

    m_pAddBtn->setDisabled( true );
    m_pRemoveBtn->setDisabled( true );

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

QJsonArray NPCFriendEquipment::equipment() const
{
    QJsonArray array;

    for ( int i{0}; i<m_pEquipment->count(); ++i )
        array.push_back( m_pEquipment->item(i)->data(0x100).toJsonObject() );

    return array;
}

void NPCFriendEquipment::setAvailableCash(const int &value)
{
    m_cash = value;
    checkItemsAvailability();
}

void NPCFriendEquipment::showCategoryItems(const QString &category)
{
    for ( int i{0}; i<m_pShop->count(); ++i ) {
        QListWidgetItem *pItem = m_pShop->item( i );
        const QJsonObject &tItem = pItem->data( 0x100 ).toJsonObject();
        bool show = (category == tItem.value("type").toString()
                     || (category == "Inne" && !tItem.contains("type")) ) ? false : true;
        pItem->setHidden( show );
    }
}

void NPCFriendEquipment::buyItem()
{
    m_pAddBtn->setDisabled( true );

    QListWidgetItem *pItem = new QListWidgetItem( *m_pShop->currentItem() );
    m_pEquipment->addItem( pItem );

    m_pShop->clearSelection();

    emit itemBougth( pItem->data( 0x101).toInt() );

    checkItemsAvailability();
}

void NPCFriendEquipment::sellItem()
{
    m_pRemoveBtn->setDisabled( true );

    emit itemSold( -m_pEquipment->currentItem()->data(0x101).toInt() );

    delete m_pEquipment->currentItem();
    m_pEquipment->clearSelection();
    checkItemsAvailability();
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
                if ( !categories.contains(tItem.value("type").toString()) ) {
                    if ( tItem.contains("type") )
                        categories << tItem.value("type").toString();
                    else {
                        if ( !categories.contains("Inne") )
                            categories << "Inne";
                    }
                }

                int price = qFloor(tItem.value("price").toInt()/2);
                price = (price>0) ? price : 1;

                QListWidgetItem *pItem = new QListWidgetItem( tItem.value("name").toString()
                                                              +" ("+QString::number(price)+"g)",
                                                              m_pShop );
                m_pShop->addItem( pItem );
                pItem->setData( 0x100, tItem );
                pItem->setData( 0x101, price );

                pItem->setToolTip( "<div style=\"\">"
                                   +itemTooltip(tItem)+
                                   "</div>" );
            }
        }
    }
    m_pCategory->addItems( categories );
    checkItemsAvailability();
}

void NPCFriendEquipment::checkItemsAvailability()
{
    for ( int i{0}; i<m_pShop->count(); ++i ) {
        QListWidgetItem *pItem = m_pShop->item(i);

        if ( m_cash < pItem->data(0x101).toInt() ) {
            pItem->setFlags( pItem->flags() & ~Qt::ItemIsEnabled );
        }
        else {
            pItem->setFlags( pItem->flags() | Qt::ItemIsEnabled );
        }
    }
}

QString NPCFriendEquipment::itemTooltip(const QJsonObject &item)
{
    QString tooltip;

    if ( item.contains("bonus") ) {
        tooltip += "<b>Bonus: </b>";
        for ( const QJsonValue bonus: item.value("bonus").toArray() ) {
            tooltip += bonus.toString();
            if ( bonus != item.value("bonus").toArray().last() )\
                tooltip += ", ";
        }
        tooltip += "<br>";
    }

    if ( item.contains("damage") ) {
        tooltip += "<b>Obrażenia: </b>";
        if ( item.value("damage").isArray() ) {
            for ( const QJsonValue damage: item.value("damage").toArray() ) {
                tooltip += damage.toString();
                if ( damage != item.value("damage").toArray().last() )
                    tooltip += ",<br> ";
            }
        }
        else
            tooltip += item.value("damage").toString();
    }

    if ( item.contains("penetration") ) {
        tooltip += "<br><b>Przebicie: </b>"
                +QString::number(item.value("penetration").toInt());
    }

    if ( item.contains("requirements") ) {
        tooltip += "<br><b>Wymagania: </b>"
                + item.value("requirements").toString();
    }

    if ( item.contains("special") ) {
        tooltip += "<br><b>Reguły specjalne: </b>";
        for ( const QJsonValue special: item.value("special").toArray() ) {
            tooltip += special.toString();
            if ( special != item.value("special").toArray().last() )
                tooltip += ", ";
        }
    }

    if ( item.contains("durability") ) {
        tooltip += "<br><b>Wytrzymałość: </b>"
                +QString::number(item.value("durability").toInt());
    }

    if ( item.contains("ammo") ) {
        tooltip += "<br><b>Amunicja: </b>"
                + item.value("ammo").toString();
    }

    if ( item.contains("magazine") ) {
        tooltip += "<br><b>Magazynek: </b>"
                + item.value("magazine").toString();
    }

    if ( item.contains("firerate") ) {
        tooltip += "<br><b>Szybkostrzelność: </b>"
                + QString::number(item.value("firerate").toInt());
    }

    return tooltip;
}

bool NPCFriendEquipment::isInEquipment(const QString &text)
{
    for ( int i{0}; i<m_pEquipment->count(); ++i ) {
        QListWidgetItem *pItem = m_pEquipment->item(i);
        if ( pItem->text().contains(text) )
            return true;
    }
    return false;
}
