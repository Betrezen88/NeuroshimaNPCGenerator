#include "NPCOriginManagerWidget.h"
#include "../Utils/DataLoader.h"

#include <QJsonValue>
#include <QRadioButton>

#include <QDebug>

NPCOriginManagerWidget::NPCOriginManagerWidget(const NPCSpecializationManagerWidget *pSpecManager,
                                               QWidget *parent)
    : QWidget(parent),
      m_pSpecManager(pSpecManager),
      m_pOrigin(new QComboBox(this)),
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
            emit originNameChanged( originName );
            attributeBonus( tOrigin.value("attribute").toObject() );
            m_pOriginDescription->setText( tOrigin.value("description").toString() );
            m_features = tOrigin.value("features").toArray();
            featuresBox();
            return;
        }
    }
}

void NPCOriginManagerWidget::setFeature(const QJsonObject &feature)
{
    emit originFeatureChanged( feature.value("name").toString(),
                               feature.value("description").toString() );
    m_pFeatureDescription->setText( feature.value("description").toString() );
    setBonus( feature.value("bonus").toObject() );
}

void NPCOriginManagerWidget::setBonus(const QJsonObject &bonus)
{
    bonusBox();

    if ( !m_bonus.isEmpty() )
        removeBonus( m_bonus );
    m_bonus = bonus;

    m_pBonusDescription->setText( m_bonus.value("text").toString() );

    if ( m_bonus.contains("select") )
        bonusLogic( m_bonus );
    const QString &type = m_bonus.value("type").toString();
    if ( "trick" == type )
        emit addBonusTrick( m_bonus.value("name").toString() );
    else if ( "specialization" == type ) {
        qDebug() << "specialization bonus";
        setSpecBonusLogic( m_pSpecManager->specialization() );
        connect( m_pSpecManager, &NPCSpecializationManagerWidget::specializationChanged,
                 this, &NPCOriginManagerWidget::setSpecBonusLogic );
    }
}

void NPCOriginManagerWidget::setSpecBonusLogic(const QString &name)
{
    if ( name == m_bonus.value("name").toString() )
        emit bonusSkillpointsChanged( m_bonus.value("value").toInt() );
    else
        emit bonusSkillpointsChanged( -m_bonus.value("value").toInt() );
}

QGroupBox *NPCOriginManagerWidget::originDescriptionBox()
{
    QGroupBox *pOriginDescriptionBox = new QGroupBox( "Opis pochodzenia", this );
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( m_pOriginDescription );
    pOriginDescriptionBox->setLayout( pLayout );
    return pOriginDescriptionBox;
}

void NPCOriginManagerWidget::attributeBonus(const QJsonObject &attribute)
{
    if ( nullptr != m_pAttributeBonus ) {
        m_pLayout->removeWidget( m_pAttributeBonus );
        delete m_pAttributeBonus;
    }

    QStringList names;
    if ( attribute.value("name").isArray() )
        for ( const QJsonValue name: attribute.value("name").toArray() )
            names << name.toString();
    else
        names << attribute.value("name").toString();
    const int &value = attribute.value("value").toInt();

    m_pAttributeBonus = new QWidget( this );
    QHBoxLayout *pAttributeBonusLayout = new QHBoxLayout;
    pAttributeBonusLayout->addWidget( new QLabel("Bonus do atrybutu: +"
                                                +QString::number(value)) );
    if ( 1 == names.count() ) {
        pAttributeBonusLayout->addWidget( new QLabel(names.first()) );
        emit attributeBonusChanged( names.first(), value );
    }
    else {
        QComboBox *pSelect = new QComboBox( m_pAttributeBonus );
        connect( pSelect, &QComboBox::currentTextChanged,
                 [this](const QString &name){
                    emit this->attributeBonusChanged( name, 1 );
        } );
        pSelect->insertItems( 0, names );
        pAttributeBonusLayout->addWidget( pSelect );
    }
    m_pAttributeBonus->setLayout( pAttributeBonusLayout );
    m_pLayout->addWidget( m_pAttributeBonus, 1, 0, 1, 2 );
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

        pRadioBtn->setChecked( m_features.first() == tFeature );
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

void NPCOriginManagerWidget::bonusBox()
{
    if ( nullptr != m_pBonusBox ) {
        m_pLayout->removeWidget( m_pBonusBox );
        delete m_pBonusBox;
    }

    m_pBonusBox = new QGroupBox( "Bonus", this );
    m_pBonusDescription = new QLabel( m_pBonusBox );
    m_pBonusDescription->setWordWrap( true );
    m_pBonusLayout = new QVBoxLayout;
    m_pBonusLayout->addWidget( m_pBonusDescription );

    m_pBonusBox->setLayout( m_pBonusLayout );
    m_pLayout->addWidget( m_pBonusBox, 3, 1 );
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

void NPCOriginManagerWidget::bonusLogic(QJsonObject &bonus)
{
    m_pSelect = new QComboBox(m_pBonusBox);
    connect( m_pSelect, &QComboBox::currentTextChanged,
             [this](const QString &name) {
                const int &value = m_bonus.value("value").toInt();
                if ( m_bonus.contains("name") ) {
                    emit bonusSkillChanged( QStringList(m_bonus.take("name").toString()),
                                            -value );
                }
                m_bonus.insert( "name", name );
                emit bonusSkillChanged( QStringList(name), value );
            }
    );
    m_pSelect->insertItems( 0, selectData(bonus.value("type").toString(),
                                          bonus.value("select").toArray()) );
    m_pBonusLayout->addWidget( m_pSelect );
}

void NPCOriginManagerWidget::removeBonus(const QJsonObject &bonus)
{
    const QString &type = bonus.value("type").toString();
    if ( "skillpack" == type ) {
        emit bonusSkillChanged( QStringList(bonus.value("name").toString()),
                                -bonus.value("value").toInt() );
    }
    else if ( "trick" == type ) {
        emit removeBonusTrick( bonus.value("name").toString() );
    }
    else if ( "specialization" == type ) {
        disconnect( m_pSpecManager, &NPCSpecializationManagerWidget::specializationChanged,
                    this, &NPCOriginManagerWidget::setSpecBonusLogic );
        emit bonusSkillpointsChanged( -m_bonus.value("value").toInt() );
    }
}
