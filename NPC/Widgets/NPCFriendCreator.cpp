#include "NPCFriendCreator.h"

#include "NPCFeatureWidget.h"
#include "../Utils/DataLoader.h"

#include <QComboBox>
#include <QDialog>
#include <QGridLayout>
#include <QJsonValue>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QSpinBox>

#include <QDebug>

NPCFriendCreator::NPCFriendCreator(QString cash, QWidget *parent)
    : QWidget( parent ),
      m_cash( QString::number(cash.toInt()*2) ),
      m_pAvailableCash( new QLabel(this) ),
      m_pSpendedCash( new QLabel(this) ),
      m_pTabWidget( new QTabWidget ),
      m_pName( new QLineEdit ),
      m_pSurname( new QLineEdit ),
      m_pNickname( new QLineEdit ),
      m_pProfession( new QLineEdit ),
      m_pConnection( new QComboBox ),
      m_pAvailableFeatures( new QListWidget ),
      m_pFeatures( new QListWidget ),
      m_pRemoveBtn( new QPushButton("Usuń") ),
      m_pAddBtn( new QPushButton("Dodaj") )
{
    connect( m_pConnection, QOverload<int>::of(&QComboBox::currentIndexChanged),
             this, &NPCFriendCreator::onConnectionValueChange );
    connect( m_pAvailableFeatures, &QListWidget::itemClicked,
             this, &NPCFriendCreator::onAvailableFeatureClick );
    connect( m_pFeatures, &QListWidget::itemClicked,
             this, &NPCFriendCreator::onBougthFeatureClick );
    connect( m_pAddBtn, &QPushButton::clicked,
             this, &NPCFriendCreator::onAddBtnClick );
    connect( m_pRemoveBtn, &QPushButton::clicked,
             this, &NPCFriendCreator::onRemoveBtnClick );
    connect( this, &NPCFriendCreator::connectionCostChanged,
             [this](const int &value){ this->setCost("connection", value); } );
    connect( this, &NPCFriendCreator::featureCostChanged,
             [this](const int &value){ this->setCost("feature", value); } );
    connect( this, &NPCFriendCreator::profitCostChanged,
             [this](const int &value){ this->setCost("profit", value); } );

    init();

    QHBoxLayout *pCashRow = new QHBoxLayout;
    pCashRow->addWidget( new QLabel("Dostępne gamble:") );
    pCashRow->addWidget( m_pAvailableCash );
    pCashRow->addSpacerItem( new QSpacerItem(5, 1) );
    pCashRow->addWidget( new QLabel("Wydane gamble:") );
    pCashRow->addWidget( m_pSpendedCash );
    pCashRow->addSpacerItem( new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding) );

    QScrollArea *pScrollArea = new QScrollArea;
    pScrollArea->setWidgetResizable( true );
    pScrollArea->setWidget( createPersonalTab() );

    m_pTabWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    m_pTabWidget->addTab( pScrollArea, "Dane personalne" );
    m_pTabWidget->addTab( createFeaturesTab(), "Cechy" );
    m_pTabWidget->addTab( new QWidget(), "Ekwipunek" );

    QVBoxLayout *pLayout = new QVBoxLayout;
    setLayout( pLayout );
    pLayout->setSpacing( 1 );
    pLayout->addLayout( pCashRow );
    pLayout->addWidget( m_pTabWidget );
}

void NPCFriendCreator::onConnectionValueChange(const int &value)
{
    m_pConnection->setToolTip( "<div style=\"width: 200px; word-wrap: break-word;\" align=\"justify\">"
                               + m_connections.at(value).toObject().value("description").toString() +
                               "</div>" );
    emit connectionCostChanged( 5*(value+1) );
}

void NPCFriendCreator::onAvailableFeatureClick(QListWidgetItem *pItem)
{
    if ( pItem->flags() == QFlags<Qt::ItemFlag>(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled
                                                |Qt::ItemIsUserCheckable|Qt::ItemIsEnabled) )
        m_pAddBtn->setEnabled( true );
}

void NPCFriendCreator::onBougthFeatureClick(QListWidgetItem *pItem)
{
    Q_UNUSED(pItem);
    m_pRemoveBtn->setEnabled( true );
}

void NPCFriendCreator::onAddBtnClick()
{
    m_pAddBtn->setDisabled( true );

    QJsonObject feature = m_pAvailableFeatures->currentItem()->data(0x100).toJsonObject();

    if ( "other" != feature.value("type").toString() ) {
        QDialog *pDialog = createDialog( feature );
        pDialog->show();
    }
    else {
        m_pAvailableFeatures->currentItem()->setData( 0x101, true );
        buyFeature( feature );
    }
    m_pAvailableFeatures->clearSelection();
}

void NPCFriendCreator::onRemoveBtnClick()
{
    m_pRemoveBtn->setDisabled( true );

    const int &row = m_pFeatures->row( m_pFeatures->currentItem() );
    QListWidgetItem *pItem = m_pFeatures->takeItem( row );
    const QJsonObject feature = pItem->data(0x100).toJsonObject();

    int price = feature.value("price").toInt();
    if ( 0 < price )
        emit featureCostChanged( -price );
    else
        emit profitCostChanged( -price );
    delete pItem;

}

void NPCFriendCreator::buyFeature(const QJsonObject &feature)
{
    int price = feature.value("price").toInt();

    QListWidgetItem *pFeature = new QListWidgetItem( m_pFeatures );
    pFeature->setText( feature.value("name").toString() );
    pFeature->setData( 0x100, feature );
    m_pFeatures->addItem( pFeature );

    if ( 0 < price )
        emit featureCostChanged( price );
    else
        emit profitCostChanged( price );
}

void NPCFriendCreator::setCost(const QString &type, const int &value)
{
    if ( "connection" == type )
        m_connectionCost = value;
    else if ( "feature" == type )
        m_featuresCost += value;
    else if ( "profit" == type )
        m_profit += value;

    m_pSpendedCash->setNum( m_connectionCost + m_featuresCost + m_profit );
    m_pAvailableCash->setNum( m_cash.toInt() - m_pSpendedCash->text().toInt() );

    checkFeatureAvailability();
}

void NPCFriendCreator::checkFeatureAvailability()
{
    for ( int i{0}; i<m_pAvailableFeatures->count(); ++i ) {
        QListWidgetItem *pItem = m_pAvailableFeatures->item(i);
        const QJsonObject feature = pItem->data(0x100).toJsonObject();
        const int &price = feature.value("price").toInt();
        const QString &type = feature.value("type").toString();

        if ( (price > 0 && m_pAvailableCash->text().toInt() < price)
             || (price < 0 && (m_featuresCost+m_connectionCost < 70) && m_profit < 30 )
             || (feature.contains("connection") && feature.value("connection").toInt() < m_pConnection->currentIndex()+1)
             || (("other" == type || "debt" == type )
                 && (pItem->data(0x101).toBool() || feature.value("price").toInt()+m_profit < -30)) ) {
            pItem->setFlags( pItem->flags() & ~Qt::ItemIsEnabled );
        }
        else {
            pItem->setFlags( pItem->flags() | Qt::ItemIsEnabled );
        }
    }
}

void NPCFriendCreator::init()
{
    m_pAddBtn->setDisabled( true );
    m_pRemoveBtn->setDisabled( true );

    m_connections = jsonData( "connection" );

    for ( const QJsonValue connection: m_connections ) {
        const QJsonObject &tConnection = connection.toObject();
        m_pConnection->addItem( QString::number(tConnection.value("value").toInt()) + ". "
                                + tConnection.value("name").toString() );
    }

    QJsonArray features = jsonData( "features" );
    for ( const QJsonValue feature: features ) {
        const QJsonObject tFeature = feature.toObject();
        QListWidgetItem *pFeature = new QListWidgetItem(m_pAvailableFeatures);
        pFeature->setText( tFeature.value("name").toString() +
                           " (" + QString::number(tFeature.value("price").toInt()) +"g)" );
        pFeature->setData( 0x100, feature );
        if ( "other" == tFeature.value("type").toString() )
            pFeature->setData( 0x101, false );
        pFeature->setToolTip( "<div style=\"width: 200px; word-wrap: break-word;\" align=\"justify\">"
                              + tFeature.value("description").toString()
                              + "</div>" );
        m_pAvailableFeatures->addItem( pFeature );
    }
    checkFeatureAvailability();
}

QWidget *NPCFriendCreator::createPersonalTab()
{
    QWidget *pWidget = new QWidget();

    m_pName = new QLineEdit( pWidget );
    m_pSurname = new QLineEdit( pWidget );
    m_pNickname = new QLineEdit( pWidget );
    m_pProfession = new QLineEdit( pWidget );

    QGridLayout *pLayout = new QGridLayout;
    pWidget->setLayout( pLayout );
    pLayout->setSpacing( 2 );
    pLayout->addWidget( new QLabel("Imię"), 0, 0 );
    pLayout->addWidget( m_pName, 0, 1 );
    pLayout->addWidget( new QLabel("Nazwisko"), 0, 2 );
    pLayout->addWidget( m_pSurname, 0, 3 );
    pLayout->addWidget( new QLabel("Ksywa"), 1, 0 );
    pLayout->addWidget( m_pNickname, 1, 1 );
    pLayout->addWidget( new QLabel("Profesja"), 1, 2 );
    pLayout->addWidget( m_pProfession, 1, 3 );

    QJsonArray questions = jsonData( "questions" );

    int row{2};
    for ( const QJsonValue question: questions ) {
        QLabel *pLabel = new QLabel(question.toObject().value("question").toString());
        pLabel->setWordWrap( true );
        pLayout->addWidget( pLabel, row, 0, 1, 4 );
        QPlainTextEdit *pAnswer = new QPlainTextEdit( this );
        pAnswer->setMaximumHeight( 80 );
        m_answers.push_back( pAnswer );
        pLayout->addWidget( pAnswer, ++row, 0, 1, 4 );
        ++row;
    }

    return pWidget;
}

QWidget *NPCFriendCreator::createFeaturesTab()
{
    QWidget *pWidget = new QWidget;

    QLabel *pAvailableL = new QLabel( "Dostępne" );
    pAvailableL->setAlignment( Qt::AlignCenter );
    QLabel *pBougthL = new QLabel( "Kupione" );
    pBougthL->setAlignment( Qt::AlignCenter );

    QGridLayout *pLayout = new QGridLayout;
    pWidget->setLayout( pLayout );
    pLayout->setSpacing( 1 );
    pLayout->addWidget( new QLabel("Poziom powiązania (koszt: 5 gambli x poziom)"), 0, 0 );
    pLayout->addWidget( m_pConnection, 0, 1 );
    pLayout->addWidget( pAvailableL, 1, 0 );
    pLayout->addWidget( m_pAvailableFeatures, 2, 0, 4, 1 );
    pLayout->addWidget( pBougthL, 1, 1 );
    pLayout->addWidget( m_pFeatures, 2, 1, 4, 1 );
    pLayout->addWidget( m_pAddBtn, 6, 0 );
    pLayout->addWidget( m_pRemoveBtn, 6, 1 );

    return pWidget;
}

QJsonArray NPCFriendCreator::jsonData(const QString &category)
{
    QJsonArray array;
    QJsonArray data = DataLoader::loadJson( ":/data/json/Friend.json" );
    for (const QJsonValue value: data ) {
        const QJsonObject &tValue = value.toObject();
        if ( category == tValue.value("name").toString() ) {
            array = tValue.value("data").toArray();
            break;
        }
    }
    return array;
}

QDialog *NPCFriendCreator::createDialog(const QJsonObject &feature)
{
    QDialog *pDialog = new QDialog();
    pDialog->setWindowTitle( feature.value("name").toString() );
    pDialog->setAttribute( Qt::WA_DeleteOnClose );
    pDialog->setModal( true );

    m_pOkBtn = new QPushButton( "Akceptuj" );
    m_pCloseBtn = new QPushButton( "Anuluj" );

    NPCFeatureWidget *pWidget = createWidget( feature );

    QHBoxLayout *pButtonsL = new QHBoxLayout;
    pButtonsL->addWidget( m_pOkBtn );
    pButtonsL->addWidget( m_pCloseBtn );

    connect( m_pCloseBtn, &QPushButton::clicked,
             pDialog, &QDialog::close );
    connect( m_pOkBtn, &QPushButton::clicked,
             pWidget, &NPCFeatureWidget::createFeature );
    connect( pWidget, &NPCFeatureWidget::sendFeature,
             this, &NPCFriendCreator::buyFeature );
    connect( m_pOkBtn, &QPushButton::clicked,
             pDialog, &QDialog::close );

    QVBoxLayout *pLayout = new QVBoxLayout;
    pDialog->setLayout( pLayout );
    pLayout->addWidget( pWidget );
    pLayout->addLayout( pButtonsL );

    return pDialog;
}

NPCFeatureWidget *NPCFriendCreator::createWidget(const QJsonObject &feature)
{
    NPCFeatureWidget *pWidget{nullptr};
    if ( "skill" == feature.value("type").toString() )
        pWidget = new SkillSelector( feature );
    else if ( "skills" == feature.value("type").toString() )
        pWidget = new SkillsSelector( feature );
    else if ( "attribute" == feature.value("type").toString() )
        pWidget = new AttributeSelector( feature );
    else if ( "reputation" == feature.value("type").toString() )
        pWidget = new ReputationSelector( feature );
    else if ( "fame" == feature.value("type").toString() )
        pWidget = new ReputationSelector( feature );
    else if ( "debt" == feature.value("type").toString() )
        pWidget = new DebtSelector( feature );
    return pWidget;
}
