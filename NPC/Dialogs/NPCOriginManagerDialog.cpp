#include "NPCOriginManagerDialog.h"
#include "../Utils/DataLoader.h"

#include <QJsonValue>
#include <QRadioButton>

#include <QDebug>

NPCOriginManagerDialog::NPCOriginManagerDialog(const NPCCardObverse *card,
                                               QWidget *parent)
    : QDialog (parent),
      m_pCard(card),
      m_pOrigin(new QComboBox(this)),
      m_pCancelBtn(new QPushButton("Zamknij", this)),
      m_pAcceptBtn(new QPushButton("Akceptuj", this)),
      m_pAttributeBonus(new QLabel(this)),
      m_pLayout(new QGridLayout(this))
{
    setAttribute( Qt::WA_DeleteOnClose );
    setWindowTitle( "Wybierz pochodzenie" );

    m_pCancelBtn->setMaximumWidth( 100 );
    m_pAcceptBtn->setMaximumWidth( 100 );

    connect( m_pCancelBtn, &QPushButton::clicked,
             this, &NPCOriginManagerDialog::close );
    connect( m_pAcceptBtn, &QPushButton::clicked,
             this, &NPCOriginManagerDialog::onAcceptBtnClicked );

    connect( m_pOrigin, &QComboBox::currentTextChanged,
             this, &NPCOriginManagerDialog::onOriginChanged );

    setOrigins( DataLoader::loadJson(":/data/json/Origins.json") );

    m_pLayout->addWidget( new QLabel("Pochodzenie", this), 0, 0 );
    m_pLayout->addWidget( m_pOrigin, 0, 1 );
    m_pLayout->addWidget( new QLabel("Bonus do atrybutu"), 1, 0 );
    m_pLayout->addWidget( m_pAttributeBonus, 1, 1 );
    m_pLayout->addWidget( m_pCancelBtn, 5, 0 );
    m_pLayout->addWidget( m_pAcceptBtn, 5, 1, Qt::AlignRight );

    setLayout( m_pLayout );
}

void NPCOriginManagerDialog::onOriginChanged(const QString &origin)
{
    if ( nullptr != m_pFeatureBox ) {
        m_pLayout->removeWidget( m_pFeatureBox );
        delete m_pFeatureBox;
    }

    const QJsonArray &features = m_origins[origin].value("features").toArray();
    const QJsonObject &attribute = m_origins[origin].value("attribute").toObject();
    m_pAttributeBonus->setText( "+"+QString::number(attribute.value("value").toInt())+" "
                                +attribute.value("name").toString());
    m_pFeatureBox = new QGroupBox("Cechy z pochodzenia");
    m_pFeatureBox->setMinimumHeight( 120 );
    QVBoxLayout *pLayout = new QVBoxLayout;
    bool checked = true;
    for ( const QJsonValue &feature: features ) {
        const QJsonObject &featureObj = feature.toObject();
        QRadioButton *pRadioBtn = new QRadioButton(featureObj.value("name").toString(), this);
        pLayout->addWidget( pRadioBtn );

        connect( pRadioBtn, &QRadioButton::toggled,
                 [this, featureObj](const bool checked){
            if ( checked ) {
                m_feature = featureObj;
                onFeatureChanged(featureObj);
            }
        } );

        pRadioBtn->toggled(checked);
        pRadioBtn->setChecked(checked);

        if ( checked ) checked = false;
    }
    m_pFeatureBox->setLayout( pLayout );
    m_pLayout->addWidget( m_pFeatureBox, 3, 0 );
    setOriginDescription( m_origins[origin].value("description").toString() );
}

void NPCOriginManagerDialog::onFeatureChanged(const QJsonObject &feature)
{
    if ( nullptr != m_pFeatureDescriptionBox ) {
        m_pLayout->removeWidget( m_pFeatureDescriptionBox );
        delete m_pFeatureDescriptionBox;
    }

    m_feature = feature;
    m_pFeatureDescriptionBox = new QGroupBox( "Opis cechy", this );
    m_pFeatureDescriptionBox->setMinimumHeight( 120 );
    QLabel *pLabel = new QLabel( feature.value("description").toString() );
    pLabel->setWordWrap( true );
    pLabel->setAlignment( Qt::AlignJustify );
    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget( pLabel );
    m_pFeatureDescriptionBox->setLayout( pLayout );
    m_pLayout->addWidget( m_pFeatureDescriptionBox, 4, 0, 1, 2 );

    setBonus( feature.value("bonus").toObject() );
}

void NPCOriginManagerDialog::onAcceptBtnClicked()
{
    emit acceptOrigin( originJson() );
    close();
}

void NPCOriginManagerDialog::setOrigins(const QJsonArray &origins)
{
    QStringList list;
    for (const QJsonValue origin: origins) {
        QString name = origin.toObject().value("name").toString();
        m_origins.insert( name, origin.toObject() );
        list << name;
    }
    m_pOrigin->insertItems( 0, list );
}

void NPCOriginManagerDialog::setBonus(const QJsonObject &bonus)
{
    if ( nullptr != m_pBonusBox ) {
        m_pLayout->removeWidget( m_pBonusBox );
        delete m_pBonusBox;
    }

    m_pBonusBox = new QGroupBox( "Bonus", this );
    m_pBonusBox->setMinimumHeight( 120 );
    QVBoxLayout *pLayout = new QVBoxLayout;
    m_pBonusBox->setLayout( pLayout );

    if ( "Info" == bonus.value("type").toString() )
            m_pBonusBox->layout()->addWidget( createBonusLabel(bonus.value("text").toString()) );
    else if ( "Complex" == bonus.value("type").toString() )
        createComplexBonus( bonus );
    else
        createSimpleBonus( bonus );

    m_pLayout->addWidget( m_pBonusBox, 3, 1 );
}

void NPCOriginManagerDialog::setOriginDescription(const QString &description)
{
    if ( nullptr != m_pDescriptionBox ) {
        m_pLayout->removeWidget( m_pDescriptionBox );
        delete m_pDescriptionBox;
    }

    m_pDescriptionBox = new QGroupBox( "Opis pochodzenia", this );
    m_pDescriptionBox->setMinimumHeight( 100 );
    QLabel *pLabel = new QLabel( description );
    pLabel->setAlignment( Qt::AlignJustify );
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( pLabel );
    m_pDescriptionBox->setLayout( pLayout );
    m_pLayout->addWidget( m_pDescriptionBox, 2, 0, 1, 2 );
}

QJsonObject NPCOriginManagerDialog::originJson()
{
    QJsonObject origin;
    const QJsonObject &tOrigin = m_origins[m_pOrigin->currentText()];
    const QJsonObject &tBonus = m_feature.value("bonus").toObject();

    origin.insert( "name", tOrigin.value("name") );
    origin.insert( "attribute", tOrigin.value("attribute") );

    QJsonObject feature;
    feature.insert( "name", m_feature.value("name").toString() );
    feature.insert( "description", m_feature.value("description").toString() );

    QJsonObject bonus;
    bonus.insert( "text", tBonus.value("text").toString() );

    if ( "Info" != tBonus.value("type").toString() ) {
        const QJsonObject &tObject = tBonus.value("object").toObject();
        QJsonObject object;
        object.insert( "type", tObject.value("type").toString() );
        object.insert( "value", tObject.value("value").toInt() );
        if ( "Complex" == tBonus.value("type").toString() )
            object.insert( "name", m_pComplex->currentText() );
        else
            object.insert( "name", tObject.value("name").toString() );
        bonus.insert( "object", object );
    }

    feature.insert( "bonus", bonus);
    origin.insert( "feature", feature );

    return origin;
}

void NPCOriginManagerDialog::createSimpleBonus(const QJsonObject &bonus)
{
    m_pBonusBox->layout()->addWidget( createBonusLabel(bonus.value("text").toString()) );
}

void NPCOriginManagerDialog::createComplexBonus(const QJsonObject &bonus)
{
    m_pComplex = new QComboBox();
    m_pComplex->insertItems( 0, getData(bonus) );

    m_pBonusBox->layout()->addWidget( createBonusLabel(bonus.value("text").toString()) );
    m_pBonusBox->layout()->addWidget( m_pComplex );
}

QLabel *NPCOriginManagerDialog::createBonusLabel(const QString &text)
{
    QLabel *pLabel = new QLabel( text );
    pLabel->setWordWrap( true );
    pLabel->setAlignment( Qt::AlignJustify );
    return pLabel;
}

QStringList NPCOriginManagerDialog::getData(const QJsonObject &bonus)
{
    QStringList data;

    const QJsonObject object = bonus.value("object").toObject();

    if ( "skillpack" == object.value("type").toString() ) {
        const QJsonArray select = bonus.value("select").toArray();
        if ( "specialization" == select.at(0).toString() )
            data << skillpacksBySpec( select.at(1).toString() );
        else if ( "attribute" == select.at(0).toString() )
            data << skillpacksByAttribute( select.at(1).toString() );
    }

    return data;
}

QStringList NPCOriginManagerDialog::skillpacksBySpec(const QString &spec)
{
    QStringList list;

    for ( const NPCAttributeWidget *attribute: *m_pCard->attributes() ) {
        for ( NPCSkillPackWidget *skillpack: *attribute->skillPacks() ) {
            if ( skillpack->specializations().contains(spec) )
                list << attribute->skillPacks()->key(skillpack);
        }
    }

    return list;
}

QStringList NPCOriginManagerDialog::skillpacksByAttribute(const QString &attribute)
{
    QStringList list;

    const NPCAttributeWidget *tAttribute = m_pCard->attributes()->value(attribute);
    for ( NPCSkillPackWidget *skillpack: *tAttribute->skillPacks() )
        list << tAttribute->skillPacks()->key( skillpack );

    return list;
}
