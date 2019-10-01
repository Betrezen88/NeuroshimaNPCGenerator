#include "NPCFriendCreator.h"

#include "NPCFeatureWidget.h"
#include "NPCFriendEquipment.h"
#include "NPCFriendStats.h"
#include "NPCAbstractSkillpackView.h"
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

NPCFriendCreator::NPCFriendCreator(QString cash, QWidget *parent)
    : QWidget( parent ),
      m_cash( cash ),
      m_pAvailableCash( new QLabel(this) ),
      m_pSpendedCash( new QLabel(this) ),
      m_pTabWidget( new QTabWidget ),
      m_pAvailableFeatures( new QListWidget ),
      m_pFeatures( new QListWidget ),
      m_pRemoveBtn( new QPushButton("Usuń") ),
      m_pAddBtn( new QPushButton("Dodaj") ),
      m_pEquipment( new NPCFriendEquipment(this) ),
      m_pStats( new NPCFriendStats(this) ),
      m_pRandomBtn( new QPushButton( QIcon(":/images/icons/Dice_icon.png"), "Losuj", this ) )
{
    connect( m_pAvailableFeatures, &QListWidget::itemClicked,
             this, &NPCFriendCreator::onAvailableFeatureClick );
    connect( m_pFeatures, &QListWidget::itemClicked,
             this, &NPCFriendCreator::onBougthFeatureClick );
    connect( m_pAddBtn, &QPushButton::clicked,
             this, &NPCFriendCreator::onAddBtnClick );
    connect( m_pRemoveBtn, &QPushButton::clicked,
             this, &NPCFriendCreator::onRemoveBtnClick );
    connect( m_pStats, &NPCFriendStats::connectionChanged,
             [this](const int &value){ this->setCost("connection", value); } );
    connect( this, &NPCFriendCreator::featureCostChanged,
             [this](const int &value){ this->setCost("feature", value); } );
    connect( this, &NPCFriendCreator::profitCostChanged,
             [this](const int &value){ this->setCost("profit", value); } );
    connect( m_pEquipment, &NPCFriendEquipment::itemBougth,
             [this](const int &value){ this->setCost("equipment", value); } );
    connect( m_pEquipment, &NPCFriendEquipment::itemSold,
             [this](const int &value){ this->setCost("equipment", value); } );
    connect( m_pRandomBtn, &QPushButton::clicked,
             m_pStats, &NPCFriendStats::random );

    init();
    setCost( "connection", 5 );

    QHBoxLayout *pCashRow = new QHBoxLayout;
    pCashRow->addWidget( new QLabel("Dostępne gamble:") );
    pCashRow->addWidget( m_pAvailableCash );
    pCashRow->addSpacerItem( new QSpacerItem(5, 1) );
    pCashRow->addWidget( new QLabel("Wydane gamble:") );
    pCashRow->addWidget( m_pSpendedCash );
    pCashRow->addSpacerItem( new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding) );
    pCashRow->addWidget( m_pRandomBtn );

    QScrollArea *pScrollArea = new QScrollArea;
    pScrollArea->setWidgetResizable( true );
    pScrollArea->setWidget( createPersonalTab() );

    QScrollArea *pStatScroll = new QScrollArea;
    pStatScroll->setWidgetResizable( true );
    pStatScroll->setWidget( m_pStats );

    m_pTabWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    m_pTabWidget->addTab( pStatScroll, "Statystyki" );
    m_pTabWidget->addTab( createFeaturesTab(), "Cechy" );
    m_pTabWidget->addTab( m_pEquipment, "Ekwipunek" );
    m_pTabWidget->addTab( pScrollArea, "Formularz" );

    QVBoxLayout *pLayout = new QVBoxLayout;
    setLayout( pLayout );
    pLayout->setSpacing( 1 );
    pLayout->addLayout( pCashRow );
    pLayout->addWidget( m_pTabWidget );
}

void NPCFriendCreator::onAvailableFeatureClick(QListWidgetItem *pItem)
{
    if ( pItem->flags() == QFlags<Qt::ItemFlag>(Qt::ItemIsSelectable|Qt::ItemIsDragEnabled
                                                |Qt::ItemIsUserCheckable|Qt::ItemIsEnabled) )
        m_pAddBtn->setEnabled( true );
}

void NPCFriendCreator::onBougthFeatureClick(QListWidgetItem *pItem)
{
    Q_UNUSED(pItem)
    m_pRemoveBtn->setEnabled( true );
}

void NPCFriendCreator::onAddBtnClick()
{
    m_pAddBtn->setDisabled( true );

    QJsonObject feature = m_pAvailableFeatures->currentItem()->data(0x100).toJsonObject();
    const QString &type = feature.value("type").toString();

    if ( "other" != type ) {
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
    else if ( "equipment" == type )
        m_equipmentCost += value;

    m_pSpendedCash->setNum( m_connectionCost + m_featuresCost + m_profit + m_equipmentCost );
    m_pAvailableCash->setNum( m_cash.toInt() - m_pSpendedCash->text().toInt() );

    m_pEquipment->setAvailableCash( m_pAvailableCash->text().toInt() );

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
             || (feature.contains("connection") && feature.value("connection").toInt() < (m_connectionCost/5))
             || (("other" == type || "debt" == type )
                 && (pItem->data(0x101).toBool() || feature.value("price").toInt()+m_profit < -30)) ) {
            pItem->setFlags( pItem->flags() & ~Qt::ItemIsEnabled );
        }
        else {
            pItem->setFlags( pItem->flags() | Qt::ItemIsEnabled );
        }
    }

    for ( int i{0}; i<m_pFeatures->count(); ++i ) {
        QListWidgetItem *pItem = m_pFeatures->item(i);
        if ( 0 > m_profit ) {
            const QJsonObject feature = pItem->data(0x100).toJsonObject();
            const int &price = feature.value("price").toInt();
            const QString &type = feature.value("type").toString();

            if ( ("other" != type || ("debt" != type ))
                 && (m_featuresCost+m_connectionCost-price < 70) ) {
                pItem->setFlags( pItem->flags() & ~Qt::ItemIsEnabled );
            }
        }
        else
            pItem->setFlags( pItem->flags() | Qt::ItemIsEnabled );
    }
}

void NPCFriendCreator::completeFriendCreation()
{
    QJsonObject personal;
    personal.insert( "name", m_pStats->name() );
    personal.insert( "surname", m_pStats->surname() );
    personal.insert( "nickname", m_pStats->nickname() );
    personal.insert( "profession", m_pStats->profession() );
    personal.insert( "sickness", m_pStats->sickness() );
    personal.insert( "connection", m_pStats->connection() );
    personal.insert( "origin", m_pStats->origin() );

    QJsonArray attributes;
    const QHash<QString, int> &attributesArray = m_pStats->attributes();
    for ( const QString &attribute: attributesArray.keys() ) {
        QJsonObject tAttribute;
        tAttribute.insert( "name", attribute );
        tAttribute.insert( "value", attributesArray.value(attribute) );
        attributes.push_back( tAttribute );
    }

    QJsonArray skillpacks;
    const QVector<NPCAbstractSkillpackView*> &skillpacksArray = m_pStats->skillpacks();
    for ( const NPCAbstractSkillpackView *skillpack: skillpacksArray ) {
        QJsonObject tSkillpack;
        tSkillpack.insert( "name", skillpack->name() );

        QJsonArray specs;
        for ( const QString &spec: skillpack->specializations() )
            specs.push_back( spec );
        tSkillpack.insert( "specializations", specs );

        QJsonArray tSkills;
        for ( const QPair<QString, int> &skill: skillpack->skills() ) {
            QJsonObject tSkill;
            tSkill.insert( "name", skill.first );
            tSkill.insert( "value", skill.second );
            tSkills.push_back( tSkill );
        }
        skillpacks.push_back( tSkillpack );
    }

    QJsonObject pal;
    pal.insert( "personal", personal );
    pal.insert( "attributes", attributes );
    pal.insert( "skillpacks", skillpacks );

    emit sendFriend( pal );
}

void NPCFriendCreator::init()
{
    m_pAddBtn->setDisabled( true );
    m_pRemoveBtn->setDisabled( true );

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

    QGridLayout *pLayout = new QGridLayout;
    pWidget->setLayout( pLayout );
    pLayout->setSpacing( 2 );

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
    pLayout->addWidget( pAvailableL, 0, 0 );
    pLayout->addWidget( m_pAvailableFeatures, 1, 0, 4, 1 );
    pLayout->addWidget( pBougthL, 0, 1 );
    pLayout->addWidget( m_pFeatures, 1, 1, 4, 1 );
    pLayout->addWidget( m_pAddBtn, 5, 0 );
    pLayout->addWidget( m_pRemoveBtn, 5, 1 );

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
    if ( "debt" == feature.value("type").toString() ) {
        connect( m_pOkBtn, &QPushButton::clicked,
                 [this](){
            this->m_pAvailableFeatures->currentItem()->setData(0x101, true);
            this->checkFeatureAvailability();
        });
    }

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
        pWidget = new DebtSelector( m_profit, feature );
    return pWidget;
}
