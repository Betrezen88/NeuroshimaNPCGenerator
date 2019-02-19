#include "NPCProfessionManagerWidget.h"
#include "../Utils/DataLoader.h"

#include <QJsonValue>
#include <QGroupBox>
#include <QRadioButton>

NPCProfessionManagerWidget::NPCProfessionManagerWidget(QWidget *parent)
    : QWidget(parent),
      m_pProfession(new QComboBox(this)),
      m_pProfessionDescription(new QLabel()),
      m_pFeatureDescription(new QLabel()),
      m_pLayout(new QGridLayout(this))
{
    connect( m_pProfession, &QComboBox::currentTextChanged,
             this, &NPCProfessionManagerWidget::setProfession );

    m_professions = DataLoader::loadJson( ":/data/json/Professions.json" );
    m_pProfession->insertItems( 0, professions() );
    m_pProfessionDescription->setWordWrap( true );
    m_pFeatureDescription->setWordWrap( true );

    m_pLayout->addWidget( new QLabel("Profesja", this), 0, 0 );
    m_pLayout->addWidget( m_pProfession, 0, 1 );
    m_pLayout->addWidget( professionDescriptionBox(), 1, 0, 1, 2 );
    m_pLayout->addWidget( featureDescriptionBox(), 3, 0, 1, 2 );

    setLayout( m_pLayout );
}

void NPCProfessionManagerWidget::setProfession(const QString &professionName)
{
    for ( const QJsonValue profession: m_professions ) {
        const QJsonObject &tProfession = profession.toObject();
        if ( professionName == tProfession.value("name").toString() ) {
            m_pProfessionDescription->setText( tProfession.value("description").toString() );
            m_features = tProfession.value("features").toArray();
            featureBox();
            break;
        }
    }
}

void NPCProfessionManagerWidget::setFeature(const QJsonObject &feature)
{
    m_pFeatureDescription->setText( feature.value("description").toString() );
    setBonus( feature.value("bonus").toObject() );
}

void NPCProfessionManagerWidget::setBonus(const QJsonObject &bonus)
{
    if ( nullptr != m_pBonusBox ) {
        m_pLayout->removeWidget( m_pBonusBox );
        delete m_pBonusBox;
    }

    m_pBonusBox = new QGroupBox( "Bonus", this );
    QLabel *pBonusDescription = new QLabel( bonus.value("text").toString(), m_pBonusBox );
    pBonusDescription->setWordWrap( true );
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( pBonusDescription );

    m_bonus = QJsonObject();
    m_bonus.insert( "text", bonus.value("text").toString() );
    if ( bonus.contains("object") ) {
        const QJsonObject &object = bonus.value("object").toObject();
        m_bonus.insert( "type", object.value("type").toString() );
        m_bonus.insert( "value", object.value("value").toInt() );
        if ( bonus.contains("name") )
            m_bonus.insert( "name", object.value("name").toString() );
        if ( bonus.contains("price") )
            m_bonus.insert( "price", object.value("price").toInt() );
    }

    m_pBonusBox->setLayout( pLayout );
    m_pLayout->addWidget( m_pBonusBox, 2, 1 );
}

QStringList NPCProfessionManagerWidget::professions()
{
    QStringList professionList;

    for ( const QJsonValue profession: m_professions )
        professionList << profession.toObject().value("name").toString();

    return professionList;
}

QGroupBox *NPCProfessionManagerWidget::professionDescriptionBox()
{
    QGroupBox *pProfessionDescriptionBox = new QGroupBox( "Opis profesji", this );
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( m_pProfessionDescription );
    return pProfessionDescriptionBox;
}

void NPCProfessionManagerWidget::featureBox()
{
    if ( nullptr != m_pFeatureBox ) {
        m_pLayout->removeWidget( m_pFeatureBox );
        delete m_pFeatureBox;
    }

    m_pFeatureBox = new QGroupBox( "Cechy profesji", this );
    QVBoxLayout *pLayout = new QVBoxLayout;

    for ( const QJsonValue feature: m_features ) {
        const QJsonObject &tFeature = feature.toObject();
        QRadioButton *pRadioBtn = new QRadioButton( tFeature.value("name").toString(),
                                                    m_pFeatureBox );
        pLayout->addWidget( pRadioBtn );
        connect( pRadioBtn, &QRadioButton::toggled,
                 [this, tFeature](bool checked){
            if ( checked ) setFeature( tFeature );
        } );

        if ( m_features.first() == tFeature ) {
            pRadioBtn->setChecked( true );
            pRadioBtn->toggled( true );
        }
    }
    m_pFeatureBox->setLayout( pLayout );
    m_pLayout->addWidget( m_pFeatureBox, 2, 0 );
}

QGroupBox *NPCProfessionManagerWidget::featureDescriptionBox()
{
    QGroupBox *pFeatureDescriptionBox = new QGroupBox( "Opis cechy", this );
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( m_pFeatureDescription );
    pFeatureDescriptionBox->setLayout( pLayout );
    return pFeatureDescriptionBox;
}
