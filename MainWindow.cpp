#include "MainWindow.h"
#include "NPC/Utils/CardConverter.h"
#include "NPC/Utils/DataLoader.h"
#include "NPC/Dialogs/NPCSkicnessDialog.h"
#include "NPC/Dialogs/NPCOriginManagerDialog.h"

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
    connect( m_pTricksDialog, &NPCTrickManagerDialog::acceptTricks,
             pCardObverse, &NPCCardObverse::addBougthTricks );

    m_pTricksDialog->show();
}

void MainWindow::showSicknessDialog()
{
    if ( m_cards.isEmpty() ) {
        QMessageBox::warning(this, "Brak postaci",
                             "Brak istniejących postaci do nabycia choroby!",
                             QMessageBox::Ok);
        return;
    }
    NPCCardObverse *pCardObverse = m_cards.at(m_pTabWidget->currentIndex())->obverse();

    NPCSkicnessDialog *pSicknessDialog = new NPCSkicnessDialog(this);

    connect( pSicknessDialog, &NPCSkicnessDialog::acceptSickness,
             pCardObverse, &NPCCardObverse::setSickness );

    pSicknessDialog->show();
}

void MainWindow::showFeatureDialog()
{
    NPCCardObverse *pCard = m_cards.at(m_pTabWidget->currentIndex())->obverse();
    NPCOriginManagerDialog *pBonus = new NPCOriginManagerDialog( pCard, this );

    connect( pBonus, &NPCOriginManagerDialog::acceptOrigin,
             pCard, &NPCCardObverse::setOrigin );

    pBonus->show();
}

void MainWindow::createNewCard()
{
    NPCCardTab *pCard = new NPCCardTab( this );
    m_cards.push_back( pCard );

    connect( pCard->obverse(), &NPCCardObverse::heroNameChanged,
             this, &MainWindow::updateTabText );

    QScrollArea *pScrollArea = new QScrollArea();
    pScrollArea->setWidget( pCard );

    m_pTabWidget->addTab( pScrollArea, "Nowa Postać" );
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
    connect( m_pRandomSicknessAction, &QAction::triggered,
             this, &MainWindow::showSicknessDialog );

    m_pTrickAction = new QAction( "Sztuczka", this );
    connect( m_pTrickAction, &QAction::triggered,
             this, &MainWindow::showTricksDialog );

    m_pExperienceAction = new QAction( "Doświadczenie", this );

    m_pChooseOriginAction = new QAction( "Pochodzenie", this );
    connect( m_pChooseOriginAction, &QAction::triggered,
             this, &MainWindow::showFeatureDialog );

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
