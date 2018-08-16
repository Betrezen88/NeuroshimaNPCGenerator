#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_pTabWidget(new QTabWidget(this))
{
    NPCCardEditor* pEditor = new NPCCardEditor(this);

    m_pTabWidget->addTab( pEditor, "Editor" );

    setCentralWidget( m_pTabWidget );
}

MainWindow::~MainWindow()
{

}
