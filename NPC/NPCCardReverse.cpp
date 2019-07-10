#include "NPCCardReverse.h"

#include "Widgets/NPCArmor.h"
#include "Widgets/NPCWeaponView.h"
#include "Widgets/NPCInventory.h"

#include <QGridLayout>
#include <QListWidget>

#include <QJsonArray>
#include <QJsonObject>

NPCCardReverse::NPCCardReverse(QWidget *parent)
    : QWidget(parent),
      m_pArmor( new NPCArmor(this) ),
      m_pWeaponView( new NPCWeaponView(this) ),
      m_pInventory( new NPCInventory(this) )
{
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
