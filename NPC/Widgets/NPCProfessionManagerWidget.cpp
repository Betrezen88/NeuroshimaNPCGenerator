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
            emit professionNameChanged( professionName );
            m_pProfessionDescription->setText( tProfession.value("description").toString() );
            m_features = tProfession.value("features").toArray();
            featureBox();
            break;
        }
    }
}

void NPCProfessionManagerWidget::setFeature(const QJsonObject &feature)
{
    emit professionFeatureChanged( feature.value("name").toString(),
                                   feature.value("description").toString() );
    m_pFeatureDescription->setText( feature.value("description").toString() );
    setBonus( feature.value("bonus").toObject() );
}

void NPCProfessionManagerWidget::setBonus(const QJsonObject &bonus)
{
    if ( nullptr != m_pBonusBox ) {
        m_pLayout->removeWidget( m_pBonusBox );
        delete m_pBonusBox;
    }

    if ( !m_bonus.isEmpty() )
        removeBonus( m_bonus );

    m_pBonusBox = new QGroupBox( "Bonus", this );
    m_pBonusDescription = new QLabel( bonus.value("text").toString(), m_pBonusBox );
    m_pBonusDescription->setWordWrap( true );
    m_pBonusLayout = new QVBoxLayout;
    m_pBonusLayout->addWidget( m_pBonusDescription );

    m_bonus = bonus;
    bonusLogic( m_bonus );

    m_pBonusBox->setLayout( m_pBonusLayout );
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

void NPCProfessionManagerWidget::bonusLogic(QJsonObject &bonus)
{
    const QString &type = bonus.value("type").toString();
    if ( "skillpack" == type ) {
        QStringList names;
        for ( const QJsonValue name: bonus.value("name").toArray() )
            names << name.toString();
        emit bonusSkillChanged( names, m_bonus.value("value").toInt() );
    }
}

void NPCProfessionManagerWidget::removeBonus(const QJsonObject &bonus)
{
    const QString &type = bonus.value("type").toString();
    if ( "skillpack" == type ) {
        QStringList names;
        for ( const QJsonValue name: bonus.value("name").toArray() )
            names << name.toString();
        emit bonusSkillChanged( names, -m_bonus.value("value").toInt() );
    }
}
