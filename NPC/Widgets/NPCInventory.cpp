#include "NPCInventory.h"
#include "NPCItem.h"

#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>

NPCInventory::NPCInventory(QWidget *parent)
    : NPCCustomWidget(parent),
      m_pTitle(new QLabel("Ekwipunek", this)),
      m_pInventory(new QListWidget(this))
{
    m_pTitle->setAlignment( Qt::AlignCenter );
    m_pTitle->setObjectName( "Title" );
    m_pTitle->setStyleSheet( m_titleStyle );

    QVBoxLayout *pLayout = new QVBoxLayout;
    setLayout( pLayout );
    pLayout->addWidget( m_pTitle );
    pLayout->addWidget( m_pInventory );
}

QVector<NPCItem *> NPCInventory::items() const
{
    QVector<NPCItem*> tItems;
    for ( int i{0}; i<m_pInventory->count(); ++i )
        tItems.push_back( qobject_cast<NPCItem*>(m_pInventory->itemWidget(m_pInventory->item(i))) );
    return tItems;
}

void NPCInventory::addItem(NPCItem *item)
{
    NPCItem *pTmpItem = findItemByName(item->name());
    if ( pTmpItem ) {
        pTmpItem->increaseQuantity();
        delete item;
    }
    else {
        QListWidgetItem *pItem = new QListWidgetItem( m_pInventory );
        m_pInventory->addItem( pItem );
        m_pInventory->setItemWidget( pItem, item );
        pItem->setSizeHint( item->sizeHint() );

        if ( NPCItem::Type::SHOP == item->type() ) {
            connect( item, &NPCItem::returnItem,
                     this, &NPCInventory::returnItemToShop );
        }

        connect( item, &NPCItem::destroyItem,
                 this, &NPCInventory::destroyItem );
    }
}

void NPCInventory::destroyItem(NPCItem *item)
{
    for ( int i{0}; i<m_pInventory->count(); ++i )
        if ( item == m_pInventory->itemWidget(m_pInventory->item(i)) ) {
            delete item;
            delete m_pInventory->takeItem(i);
            break;
        }
}

NPCItem *NPCInventory::findItemByName(const QString &name)
{
    NPCItem *pItem{nullptr};
    for ( int i{0}; i<m_pInventory->count(); ++i ) {
        NPCItem *pTmpItem = qobject_cast<NPCItem*>( m_pInventory->itemWidget(m_pInventory->item(i)) );
        if ( pTmpItem && (name == pTmpItem->name()) ) {
            pItem = pTmpItem;
            break;
        }
    }
    return pItem;
}
