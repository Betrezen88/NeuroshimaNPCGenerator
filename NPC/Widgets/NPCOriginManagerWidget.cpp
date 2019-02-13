#include "NPCOriginManagerWidget.h"
#include "../Utils/DataLoader.h"

#include <QJsonValue>
#include <QRadioButton>

NPCOriginManagerWidget::NPCOriginManagerWidget(QWidget *parent)
    : QWidget(parent),
      m_pOrigin(new QComboBox(this)),
      m_pAttributeBonus(new QLabel("+0 Atrybut", this)),
      m_pOriginDescription(new QLabel(this)),
      m_pFeatureDescription(new QLabel(this)),
      m_pLayout(new QGridLayout())
{
    connect( m_pOrigin, &QComboBox::currentTextChanged,
             this, &NPCOriginManagerWidget::setOrigin );

    m_attributes = DataLoader::loadJson( ":/data/json/Attributes.json" );
    m_origins = DataLoader::loadJson( ":/data/json/Origins.json" );
    m_pOrigin->insertItems( 0, origins() );

    m_pOriginDescription->setWordWrap( true );
    m_pFeatureDescription->setWordWrap( true );

    m_pLayout->addWidget( new QLabel("Pochodzenie", this), 0, 0 );
    m_pLayout->addWidget( m_pOrigin, 0, 1 );
    m_pLayout->addWidget( new QLabel("Bonus do atrybutu", this), 1, 0 );
    m_pLayout->addWidget( m_pAttributeBonus, 1, 1 );
    m_pLayout->addWidget( originDescriptionBox(), 2, 0, 1, 2 );
    m_pLayout->addWidget( featureDescriptionBox(), 4, 0, 1, 2 );
    m_pLayout->setSpacing( 1 );

    setLayout( m_pLayout );
}

void NPCOriginManagerWidget::setOrigin(const QString &originName)
{
    for ( const QJsonValue origin: m_origins ) {
        const QJsonObject &tOrigin = origin.toObject();
        if ( originName == tOrigin.value("name").toString() ) {
            const QJsonObject &attribute = tOrigin.value("attribute").toObject();
            m_pOriginDescription->setText( tOrigin.value("description").toString() );
            m_pAttributeBonus->setText( "+"+QString::number(attribute.value("value").toInt())
                                        +" "+attribute.value("name").toString() );
            m_features = tOrigin.value("features").toArray();
            featuresBox();
            break;
        }
    }
}

void NPCOriginManagerWidget::setFeature(const QJsonObject &feature)
{
    m_pFeatureDescription->setText( feature.value("description").toString() );
    setBonus( feature.value("bonus").toObject() );
}

void NPCOriginManagerWidget::setBonus(const QJsonObject &bonus)
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
        m_bonus.insert( "value", object.value("value").toInt() );
        if ( bonus.contains("select") ) {
            m_pSelect = new QComboBox(m_pBonusBox);
            connect( m_pSelect, &QComboBox::currentTextChanged,
                     this, &NPCOriginManagerWidget::setBonusExtra );
            m_pSelect->insertItems( 0, selectData(object.value("type").toString(),
                                                  bonus.value("select").toArray()) );
            pLayout->addWidget( m_pSelect );
        }
    }

    m_pBonusBox->setLayout( pLayout );
    m_pLayout->addWidget( m_pBonusBox, 3, 1 );
}

void NPCOriginManagerWidget::setBonusExtra(const QString &extra)
{
    m_bonus.insert( "name", extra );
}

QGroupBox *NPCOriginManagerWidget::originDescriptionBox()
{
    QGroupBox *pOriginDescriptionBox = new QGroupBox( "Opis pochodzenia", this );
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( m_pOriginDescription );
    pOriginDescriptionBox->setLayout( pLayout );
    return pOriginDescriptionBox;
}

void NPCOriginManagerWidget::featuresBox()
{
    if ( nullptr != m_pFeatureBox ) {
        m_pLayout->removeWidget( m_pFeatureBox );
        delete m_pFeatureBox;
    }

    m_pFeatureBox = new QGroupBox( "Cechy z pochodzenia", this );
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
    m_pLayout->addWidget( m_pFeatureBox, 3, 0 );
}

QGroupBox *NPCOriginManagerWidget::featureDescriptionBox()
{
    QGroupBox *pFeatureDescriptionBox = new QGroupBox( "Opis cechy", this );
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( m_pFeatureDescription );
    pFeatureDescriptionBox->setLayout( pLayout );
    return pFeatureDescriptionBox;
}

QStringList NPCOriginManagerWidget::origins()
{
    QStringList originsList;
    for ( const QJsonValue origin: m_origins )
        originsList << origin.toObject().value("name").toString();
    return originsList;
}

QStringList NPCOriginManagerWidget::selectData(const QString &type, const QJsonArray &select)
{
    QStringList data;

    if ( "skillpack" == type ) {
        for ( const QJsonValue attribute: m_attributes ) {
            const QJsonArray &skillpacks = attribute.toObject().value("skillPacks").toArray();
            for ( const QJsonValue skillpack: skillpacks ) {
                if ( "specialization" == select.at(0).toString() ) {
                    const QJsonArray &specs = skillpack.toObject().value("Specialization").toArray();
                    if ( specs.contains( select.at(1).toString() ) )
                        data << skillpack.toObject().value("name").toString();
                }
                else if ( ("attribute" == select.at(0).toString()) &&
                          (attribute.toObject().value("name").toString() ==
                           select.at(1).toString()) ) {
                    data << skillpack.toObject().value("name").toString();
                }
            }
        }
    }

    return data;
}
