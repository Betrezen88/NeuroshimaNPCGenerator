#include "NPCWeaponView.h"
#include "NPCWeaponItem.h"

#include <QLabel>
#include <QListWidget>

#include <QVBoxLayout>

NPCWeaponView::NPCWeaponView(int *body, QWidget *parent)
    : NPCCustomWidget(parent),
      m_pBody( body ),
      m_pTitle( new QLabel("Broń", this) ),
      m_pWeapons( new QListWidget(this) )
{
    m_pTitle->setObjectName( "Title" );
    m_pTitle->setStyleSheet( m_titleStyle );
    m_pTitle->setAlignment( Qt::AlignCenter );

    QVBoxLayout *pLayout = new QVBoxLayout;
    setLayout( pLayout );
    pLayout->addWidget( m_pTitle );
    pLayout->addWidget( m_pWeapons );
}

void NPCWeaponView::addWeapon(const QJsonObject &item)
{
    QListWidgetItem *pItem = new QListWidgetItem( m_pWeapons );
    NPCWeaponItem *pWeapon = new NPCWeaponItem(item, *m_pBody, this);
    m_pWeapons->addItem( pItem );
    m_pWeapons->setItemWidget( pItem, pWeapon );
    pItem->setSizeHint( pWeapon->sizeHint() );

    connect( pWeapon, &NPCWeaponItem::unequip,
             this, &NPCWeaponView::unequip );
    connect( pWeapon, &NPCWeaponItem::destroyView,
             this, &NPCWeaponView::removeWeapon );
}

void NPCWeaponView::removeWeapon(NPCWeaponItem *weapon)
{
    for ( int i{0}; i<m_pWeapons->count(); ++i )
        if ( weapon == m_pWeapons->itemWidget(m_pWeapons->item(i)) ) {
            delete weapon;
            delete m_pWeapons->takeItem(i);
            break;
        }
}
