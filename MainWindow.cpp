#include "MainWindow.h"

#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_pTabWidget(new QTabWidget(this))
{
    m_pCard = new NPCCard(this);

    createActions();
    createMenus();

    m_pTabWidget->addTab( m_pCard, "Postać" );

    connect( m_pCard, &NPCCard::heroNameChanged,
             this, &MainWindow::updateTabText );

    setCentralWidget( m_pTabWidget );
}

MainWindow::~MainWindow()
{

}

void MainWindow::showAttributeDialog()
{

}

void MainWindow::updateTabText(const QString &text)
{
    m_pTabWidget->setTabText( m_pTabWidget->currentIndex(), text );
}

void MainWindow::createActions()
{
    m_pAttributesAction = new QAction( "Atrybuty", this );
    connect( m_pAttributesAction, &QAction::triggered, this, &MainWindow::showAttributeDialog );
}

void MainWindow::createMenus()
{
    m_pHeroMenu = menuBar()->addMenu( "Postać..." );
    m_pHeroMenu->addAction( m_pAttributesAction );
}
