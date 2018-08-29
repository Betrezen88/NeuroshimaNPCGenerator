#include "MainWindow.h"

#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_pTabWidget(new QTabWidget(this))
{
    NPCCardEditor* pEditor = new NPCCardEditor(this);

    createActions();
    createMenus();

    m_pTabWidget->addTab( pEditor, "Editor" );

    setCentralWidget( m_pTabWidget );
}

MainWindow::~MainWindow()
{

}

void MainWindow::showAttributeDialog()
{

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
