#include "NPCCardReverse.h"

#include "Widgets/NPCArmor.h"
#include "Widgets/NPCWeaponView.h"
#include "Widgets/NPCInventory.h"
#include "Widgets/NPCItem.h"

#include <QGridLayout>
#include <QListWidget>

#include <QJsonArray>
#include <QJsonObject>

NPCCardReverse::NPCCardReverse(int *body, QWidget *parent)
    : QWidget(parent),
      m_pArmor( new NPCArmor(this) ),
      m_pWeaponView( new NPCWeaponView(body, this) ),
      m_pInventory( new NPCInventory(this) )
{
    connect( m_pInventory, &NPCInventory::equipWeapon,
             m_pWeaponView, &NPCWeaponView::addWeapon );
    connect( m_pInventory, &NPCInventory::equipArmor,
             m_pArmor, &NPCArmor::addArmor );
    connect( m_pWeaponView, &NPCWeaponView::unequip,
             [this](const QJsonObject &item){
        NPCItem *pItem = new NPCItem(item, NPCItem::Type::INVENTORY, m_pInventory);
        m_pInventory->addItem( pItem );
    });
    connect( m_pArmor, &NPCArmor::unequip,
             [this](const QJsonObject &item){
        NPCItem *pItem = new NPCItem(item, NPCItem::Type::INVENTORY, m_pInventory);
        m_pInventory->addItem( pItem );
    });

    QGridLayout *pLayout = new QGridLayout;
    setLayout( pLayout );
    pLayout->addWidget( m_pArmor, 0, 0, 1, 1 );
    pLayout->addWidget( m_pWeaponView, 1, 0, 3, 1 );
    pLayout->addWidget( m_pInventory, 0, 1, 4, 1 );
}

NPCInventory *NPCCardReverse::inventory() const
{
    return m_pInventory;
}
