#include "MainWindow.h"

#include "NPCCard.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget( new NPCCard(this) );
}

MainWindow::~MainWindow()
{

}
