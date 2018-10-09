#include "NPCTricksWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

NPCTricksWidget::NPCTricksWidget(QWidget *parent)
    : QWidget(parent),
      m_pTricks(new QListWidget(this)),
      m_pBuyBtn(new QPushButton("Kup", this)),
      m_pSellBtn(new QPushButton("Sprzedaj", this))
{
    QHBoxLayout *pButtonsL = new QHBoxLayout;
    pButtonsL->addWidget( m_pBuyBtn );
    pButtonsL->addWidget( m_pSellBtn );

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( m_pTricks );
    pLayout->addLayout( pButtonsL );
    pLayout->setMargin( 0 );
    pLayout->setSpacing( 0 );
    setLayout( pLayout );
}
