#include "NPCFriendManager.h"
#include "NPCFriendCreator.h"

#include <QDialog>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

NPCFriendManager::NPCFriendManager(QWidget *parent)
    : QWidget (parent),
      m_pFriends( new QListWidget(this) ),
      m_pAddBtn( new QPushButton("Stwórz", this) ),
      m_pEditBtn( new QPushButton("Edytuj", this) ),
      m_pRemoveBtn( new QPushButton("Usuń", this) ),
      m_pCash(new QLabel("50"))
{
    connect( m_pAddBtn, &QPushButton::clicked,
             this, &NPCFriendManager::showCreatorDialog );

    QHBoxLayout *pCashL = new QHBoxLayout;
    pCashL->addWidget( new QLabel("Kasa na znajomości:") );
    pCashL->addWidget( m_pCash, 1, Qt::AlignLeft );

    QHBoxLayout *pButtons = new QHBoxLayout;
    pButtons->addWidget( m_pAddBtn );
    pButtons->addWidget( m_pEditBtn );
    pButtons->addWidget( m_pRemoveBtn );

    QVBoxLayout *pLayout = new QVBoxLayout;
    setLayout( pLayout );
    pLayout->addLayout( pCashL );
    pLayout->addLayout( pButtons );
    pLayout->addWidget( m_pFriends );
}

void NPCFriendManager::showCreatorDialog() const
{
    QDialog *pDialog = new QDialog();
    pDialog->setModal( true );
    pDialog->setAttribute( Qt::WA_DeleteOnClose );
    pDialog->setWindowTitle( "Tworzenie znajomości" );
    pDialog->setMinimumWidth( 400 );

    QPushButton *pOkBtn = new QPushButton("Dodaj");
    QPushButton *pCloseBtn = new QPushButton("Anuluj");

    connect( pCloseBtn, &QPushButton::clicked,
             pDialog, &QDialog::close );
    connect( pOkBtn, &QPushButton::clicked,
             pDialog, &QDialog::close );

    QHBoxLayout *pButtonL = new QHBoxLayout;
    pButtonL->addSpacerItem( new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding) );
    pButtonL->addWidget( pOkBtn );
    pButtonL->addWidget( pCloseBtn );

    NPCFriendCreator *pCreator = new NPCFriendCreator( m_pCash->text() );

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( pCreator );
    pLayout->addLayout( pButtonL );

    pDialog->setLayout( pLayout );

    pDialog->show();
}
