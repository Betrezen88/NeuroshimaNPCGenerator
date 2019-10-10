#include "NPCFriendManager.h"
#include "NPCFriendCreator.h"
#include "NPCFriendCard.h"
#include "NPCFriendObverse.h"

#include <QDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QTabWidget>

NPCFriendManager::NPCFriendManager(QWidget *parent)
    : QWidget (parent),
      m_pFriends( new QTabWidget(this) ),
      m_pAddBtn( new QPushButton("Stwórz", this) ),
      m_pEditBtn( new QPushButton("Edytuj", this) ),
      m_pRemoveBtn( new QPushButton("Usuń", this) ),
      m_pCash(new QLabel("50"))
{
    connect( m_pAddBtn, &QPushButton::clicked,
             this, &NPCFriendManager::showCreatorDialog );
    connect( m_pFriends, &QTabWidget::tabCloseRequested,
             this, &NPCFriendManager::removeFriend );

    m_pFriends->setTabsClosable( true );
    QScrollArea *pScrollArea = new QScrollArea( this );
    pScrollArea->setWidgetResizable( true );
    pScrollArea->setWidget( m_pFriends );

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
    pLayout->addWidget( pScrollArea );
}

void NPCFriendManager::showCreatorDialog() const
{
    QDialog *pDialog = new QDialog();
    pDialog->setModal( true );
    pDialog->setAttribute( Qt::WA_DeleteOnClose );
    pDialog->setWindowTitle( "Tworzenie znajomości" );
    pDialog->setMinimumWidth( 400 );

    NPCFriendCreator *pCreator = new NPCFriendCreator( m_pCash->text() );

    QPushButton *pOkBtn = new QPushButton("Dodaj");
    QPushButton *pCloseBtn = new QPushButton("Anuluj");

    connect( pCloseBtn, &QPushButton::clicked,
             pDialog, &QDialog::close );
    connect( pOkBtn, &QPushButton::clicked,
             pCreator, &NPCFriendCreator::completeFriendCreation );
    connect( pCreator, &NPCFriendCreator::sendFriend,
             this, &NPCFriendManager::addFriend );
    connect( pOkBtn, &QPushButton::clicked,
             pDialog, &QDialog::close );

    QHBoxLayout *pButtonL = new QHBoxLayout;
    pButtonL->addSpacerItem( new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding) );
    pButtonL->addWidget( pOkBtn );
    pButtonL->addWidget( pCloseBtn );

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( pCreator );
    pLayout->addLayout( pButtonL );

    pDialog->setLayout( pLayout );

    pDialog->show();
}

void NPCFriendManager::addFriend(const QJsonObject &pal, const int &cost)
{
    NPCFriendCard *pFriend = new NPCFriendCard(pal, this);
    const QString &name = pFriend->obverse()->fullname();
    m_friends.push_back( pFriend );
    m_pFriends->addTab( pFriend, name );

    m_prices.insert( name, cost );
    m_pCash->setNum( m_pCash->text().toInt() - cost );
}

void NPCFriendManager::removeFriend(const int &index)
{
    const QString &name = m_friends.at(index)->obverse()->fullname();
    QMessageBox::StandardButton btn =
            QMessageBox::question(this,
                                  "Usunąć przyjaciela",
                                  "Czy chcesz usunąć "+name+ " ?",
                                  QMessageBox::StandardButton::Yes
                                  | QMessageBox::StandardButton::No);

    if ( QMessageBox::StandardButton::No == btn )
        return;
    else {
        m_pCash->setNum( m_pCash->text().toInt() + m_prices.value(name) );
        m_pFriends->removeTab( index );
        m_friends.removeAt( index );
    }
}
