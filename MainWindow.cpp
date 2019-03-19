#include "MainWindow.h"
#include "NPC/Utils/CardConverter.h"
#include "NPC/Utils/DataLoader.h"
#include "NPC/Dialogs/NPCSkicnessDialog.h"
#include "NPC/Dialogs/NPCOriginManagerDialog.h"
#include "NPC/Dialogs/NPCCreatorDialog.h"

#include <QMenuBar>
#include <QScrollArea>
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_pTabWidget(new QTabWidget(this))
{
    setMinimumSize( 300, 200 );
    createActions();
    createMenus();

    m_pTabWidget->setTabsClosable( true );

    connect( m_pTabWidget, &QTabWidget::tabCloseRequested,
             this, &MainWindow::onCardClose );

    setCentralWidget( m_pTabWidget );
}

MainWindow::~MainWindow()
{

}

void MainWindow::createNewCard()
{
    NPCCreatorDialog *pDialog = new NPCCreatorDialog(this);

    connect( pDialog, &NPCCreatorDialog::creationCompleted,
             this, &MainWindow::addCard );

    pDialog->show();
}

void MainWindow::updateTabText(const QString &text)
{
    m_pTabWidget->setTabText( m_pTabWidget->currentIndex(), text );
}

void MainWindow::onCardClose(const int &index)
{
    QMessageBox::StandardButton btn =
            QMessageBox::question(this,
                                  "Zapisz postać",
                                  "Czy chcesz zapisać postać ?",
                                  QMessageBox::StandardButton::Yes
                                  | QMessageBox::StandardButton::No
                                  | QMessageBox::StandardButton::Cancel );

    if ( QMessageBox::StandardButton::Cancel == btn )
        return;

    if ( QMessageBox::StandardButton::Yes == btn )
        saveCard();

    m_pTabWidget->removeTab( index );
    m_cards.remove( index );
}

void MainWindow::saveCard()
{
    NPCCardTab *pCard = m_cards.at( m_pTabWidget->currentIndex() );
    QString filePath = QFileDialog::getSaveFileName( this,
                                                     "Zapisz postać",
                                                     QDir::homePath() + "/"
                                                     + pCard->obverse()->heroName() + ".json",
                                                     "(*.json)" );
    if ( filePath.isEmpty() )
        return;

    QFile file( filePath );
    if ( !file.open(QIODevice::WriteOnly) ) {
        qDebug() << "File open failed !";
        return;
    }

    CardConverter converter;
    QJsonDocument json;
    json.setObject( converter.toJson(pCard) );
    file.write( json.toJson() );
}

void MainWindow::addCard(NPCCardTab *card)
{
    QScrollArea *pScrollArea = new QScrollArea();
    pScrollArea->setWidget( card );

    m_pTabWidget->addTab( pScrollArea, "Nowa Postać" );
}

void MainWindow::createActions()
{
    m_pNewCardAction = new QAction( "Stwórz", this );
    connect( m_pNewCardAction, &QAction::triggered,
             this, &MainWindow::createNewCard );

    m_pSaveCardAction = new QAction( "Zapisz", this );

    m_pLoadCardAction = new QAction( "Wczytaj", this );

    m_pRandomAttributesAction = new QAction( "Atrybuty" );

    m_pRandomSicknessAction = new QAction( "Choroba", this );

    m_pTrickAction = new QAction( "Sztuczka", this );

    m_pExperienceAction = new QAction( "Doświadczenie", this );

    m_pChooseOriginAction = new QAction( "Pochodzenie", this );

    m_pChooseProfessionAction = new QAction( "Profesje", this );
}

void MainWindow::createMenus()
{
    m_pRandomMenu = new QMenu( "Losuj" );
    m_pRandomMenu->addAction( m_pRandomAttributesAction );
    m_pRandomMenu->addAction( m_pRandomSicknessAction );

    m_pChooseMenu = new QMenu( "Wybierz" );
    m_pChooseMenu->addAction( m_pChooseOriginAction );
    m_pChooseMenu->addAction( m_pChooseProfessionAction );

    m_pHeroMenu = menuBar()->addMenu( "Postać" );
    m_pHeroMenu->addAction( m_pNewCardAction );
    m_pHeroMenu->addAction( m_pSaveCardAction );
    m_pHeroMenu->addAction( m_pLoadCardAction );

    m_pCreationMenu = menuBar()->addMenu( "Tworzenie" );
    m_pCreationMenu->addMenu( m_pChooseMenu );
    m_pCreationMenu->addMenu( m_pRandomMenu );
    m_pCreationMenu->addAction( m_pTrickAction );

    m_pGameMasterMenu = menuBar()->addMenu( "Mistrz Gry" );
    m_pGameMasterMenu->addAction( m_pExperienceAction );
}
