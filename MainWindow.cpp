#include "MainWindow.h"

#include <QMenuBar>
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_pTabWidget(new QTabWidget(this)),
      m_pAttributeDialog(new NPCAttributeManagerDialog(this))
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
    NPCCardObverse *pCardObverse = m_cards.at(m_pTabWidget->currentIndex())->obverse();

    connect( m_pAttributeDialog, &NPCAttributeManagerDialog::acceptAttributes,
             pCardObverse, &NPCCardObverse::onAttributeChanged );
    connect( m_pAttributeDialog, &NPCAttributeManagerDialog::hidden,
             this, &MainWindow::onAttributeDialogClose );

    m_pAttributeDialog->show();
}

void MainWindow::onAttributeDialogClose()
{
    m_pAttributeDialog->disconnect( m_cards.at(m_pTabWidget->currentIndex())->obverse() );
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
    m_pAttributesAction = new QAction( "Atrybuty", this );
    connect( m_pAttributesAction, &QAction::triggered,
             this, &MainWindow::showAttributeDialog );

    m_pNewCardAction = new QAction( "Nowa Postać", this );
    connect( m_pNewCardAction, &QAction::triggered,
             this, &MainWindow::createNewCard );
}

void MainWindow::createMenus()
{
    m_pHeroMenu = menuBar()->addMenu( "Postać..." );
    m_pHeroMenu->addAction( m_pNewCardAction );
    m_pHeroMenu->addAction( m_pAttributesAction );
}
