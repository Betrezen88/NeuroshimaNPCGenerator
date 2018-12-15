#include "MainWindow.h"

#include <QMenuBar>
#include <QScrollArea>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_pTabWidget(new QTabWidget(this))
{
    createActions();
    createMenus();

    setCentralWidget( m_pTabWidget );
}

MainWindow::~MainWindow()
{

}

void MainWindow::showAttributeDialog()
{
    if ( m_cards.isEmpty() ) {
        QMessageBox::warning(this, "Błąd losowania atrybutów",
                             "Brak istniejących postaci do losowania atrybutów!",
                             QMessageBox::Ok);
        return;
    }

    NPCCardObverse *pCardObverse = m_cards.at(m_pTabWidget->currentIndex())->obverse();
    m_pAttributeDialog = new NPCAttributeManagerDialog( this );

    connect( m_pAttributeDialog, &NPCAttributeManagerDialog::acceptAttributes,
             pCardObverse, &NPCCardObverse::onAttributeChanged );

    m_pAttributeDialog->show();
}

void MainWindow::showTricksDialog()
{
    if ( m_cards.isEmpty() ) {
        QMessageBox::warning(this, "Błąd sztuczek",
                             "Brak istniejących postaci do nabycia sztuczek!",
                             QMessageBox::Ok);
        return;
    }
    NPCCardObverse *pCardObverse = m_cards.at(m_pTabWidget->currentIndex())->obverse();

    m_pTricksDialog = new NPCTrickManagerDialog( pCardObverse->attributes(), this );

    connect( m_pTricksDialog, &NPCTrickManagerDialog::buyTrick,
             pCardObverse->progressWidget(), &NPCProgressWidget::onTrickBougth );
    connect( m_pTricksDialog, &NPCTrickManagerDialog::resignTrick,
             pCardObverse->progressWidget(), &NPCProgressWidget::onTrickResign );
    connect( pCardObverse->progressWidget(), &NPCProgressWidget::addTrick,
             m_pTricksDialog, &NPCTrickManagerDialog::trickBougth );
    connect( pCardObverse->progressWidget(), &NPCProgressWidget::removeTrick,
             m_pTricksDialog, &NPCTrickManagerDialog::trickRemove );

    m_pTricksDialog->show();
}

void MainWindow::createNewCard()
{
    NPCCardTab *pCard = new NPCCardTab( this );
    m_cards.push_back( pCard );

    connect( pCard->obverse(), &NPCCardObverse::heroNameChanged,
             this, &MainWindow::updateTabText );

    m_pTabWidget->addTab( pCard, "Nowa Postać" );
}

void MainWindow::updateTabText(const QString &text)
{
    m_pTabWidget->setTabText( m_pTabWidget->currentIndex(), text );
}

void MainWindow::createActions()
{
    m_pNewCardAction = new QAction( "Stwórz", this );
    connect( m_pNewCardAction, &QAction::triggered,
             this, &MainWindow::createNewCard );

    m_pSaveCardAction = new QAction( "Zapisz", this );

    m_pLoadCardAction = new QAction( "Wczytaj", this );

    m_pRandomAttributesAction = new QAction( "Atrybuty" );
    connect( m_pRandomAttributesAction, &QAction::triggered,
             this, &MainWindow::showAttributeDialog );

    m_pRandomSicknessAction = new QAction( "Choroba", this );

    m_pTrickAction = new QAction( "Sztuczka", this );
    connect( m_pTrickAction, &QAction::triggered,
             this, &MainWindow::showTricksDialog );

    m_pExperienceAction = new QAction( "Doświadczenie", this );
}

void MainWindow::createMenus()
{
    m_pRandomMenu = new QMenu( "Losuj" );
    m_pRandomMenu->addAction( m_pRandomAttributesAction );
    m_pRandomMenu->addAction( m_pRandomSicknessAction );

    m_pAddMenu = new QMenu( "Dodaj" );
    m_pAddMenu->addAction( m_pTrickAction );

    m_pGameMasterMenu = new QMenu( "MG" );
    m_pGameMasterMenu->addAction( m_pExperienceAction );

    m_pHeroMenu = menuBar()->addMenu( "Postać" );
    m_pHeroMenu->addAction( m_pNewCardAction );
    m_pHeroMenu->addAction( m_pSaveCardAction );
    m_pHeroMenu->addAction( m_pLoadCardAction );
    m_pHeroMenu->addSeparator();
    m_pHeroMenu->addMenu( m_pRandomMenu );
    m_pHeroMenu->addMenu( m_pAddMenu );
    m_pHeroMenu->addMenu( m_pGameMasterMenu );
}
