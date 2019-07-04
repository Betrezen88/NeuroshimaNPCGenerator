#include "NPCWeaponView.h"

#include <QLabel>
#include <QListWidget>

#include <QVBoxLayout>

NPCWeaponView::NPCWeaponView(QWidget *parent)
    : NPCCustomWidget(parent),
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

}
