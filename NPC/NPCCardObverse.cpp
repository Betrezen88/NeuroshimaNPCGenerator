#include "NPCCardObverse.h"

#include "Utils/DataLoader.h"

#include <QHBoxLayout>
#include <QDebug>

NPCCardObverse::NPCCardObverse(QWidget *parent)
    : QWidget(parent),
      m_pPortrait(new QLabel(this)),
      m_pName(new QLineEdit(this)),
      m_pOrigin(new QComboBox(this)),
      m_pProfession(new QComboBox(this)),
      m_pSpecialization(new QComboBox(this)),
      m_pSickness(new QLineEdit(this)),
      m_pFeature1(new QComboBox(this)),
      m_pFeature2(new QComboBox(this)),
      m_pReputation(new QSpinBox(this)),
      m_pFame(new QSpinBox(this)),
      m_pTricks(new QListWidget(this)),
      m_pProgressWidget(new NPCProgressWidget(m_pSpecialization, this))
{
    m_pSickness->setReadOnly( true );

    setAttributes( DataLoader::loadJson(":/data/json/Attributes.json") );
    setSpecializations( DataLoader::loadJson(":/data/json/Specializations.json") );
    setOrigins( DataLoader::loadJson(":/data/json/Origins.json") );
    setProfessions( DataLoader::loadJson(":/data/json/Professions.json") );
    m_attributesMods = DataLoader::loadJson( ":/data/json/DifficultyLevel.json" );

    m_pAttributesModsInfo = new NPCAttributesModsInfoWidget( m_attributesMods, this );

    connect( m_pName, &QLineEdit::textChanged,
             this, &NPCCardObverse::heroNameChanged );
    connect( m_pOrigin, &QComboBox::currentTextChanged,
             this, &NPCCardObverse::onOriginChange );
    connect( m_pProfession,  &QComboBox::currentTextChanged,
             this, &NPCCardObverse::onProfessionChanged );
    connect( m_pFeature1, &QComboBox::currentTextChanged, [this](){
        onFeatureChanged( m_pFeature1, m_originFeatures );
    } );
    connect( m_pFeature2, &QComboBox::currentTextChanged, [this](){
        onFeatureChanged( m_pFeature2, m_professionFeatures );
    } );

    QHBoxLayout *pLayout = new QHBoxLayout;
    setLayout( pLayout );
    pLayout->addLayout( column1() );
    pLayout->addLayout( column2() );
    pLayout->addLayout( column3() );
    pLayout->addLayout( column4() );
}

void NPCCardObverse::onOriginChange(const QString &name)
{
    const QJsonObject origin = m_origins.value( name );
    const QJsonObject attribute = origin.value( "attribute" ).toObject();
    QStringList featureNames;
    m_originFeatures = origin.value("features").toArray();

    for ( NPCAttributeWidget *attribute: m_attributes )
        if ( 1 == *attribute->modValue() )
            attribute->setModValue( 0 );
    if ( m_attributes.keys().contains(attribute.value("name").toString()) )
        m_attributes[attribute.value("name").toString()]->setModValue( attribute.value("value").toInt() );

    for ( const QJsonValueRef tOrigin: m_originFeatures )
        featureNames << tOrigin.toObject().value("name").toString();

    m_pFeature1->clear();
    m_pFeature1->insertItems( 0, featureNames );
    onFeatureChanged( m_pFeature1, m_originFeatures );
}

void NPCCardObverse::onProfessionChanged(const QString &name)
{
    const QJsonObject profession = m_professions.value( name );
    QStringList featureNames;
    m_professionFeatures = profession.value("features").toArray();

    for (const QJsonValueRef tProfession: m_professionFeatures)
        featureNames << tProfession.toObject().value("name").toString();

    m_pFeature2->clear();
    m_pFeature2->insertItems( 0, featureNames );
    onFeatureChanged( m_pFeature2, m_professionFeatures );
}

void NPCCardObverse::onFeatureChanged(QComboBox *pFeature, const QJsonArray &features)
{
    for ( const QJsonValue &tFeature: features ) {
        const QJsonObject &feature = tFeature.toObject();
        if ( feature.value("name").toString() == pFeature->currentText() ) {
            pFeature->setToolTip( feature.value("description").toString() );
            break;
        }
    }
}

void NPCCardObverse::onAttributeChanged(QVector<int> attributes)
{
    QStringList names = m_attributes.keys();

    int index = 0;
    for ( const QString &tName: names ) {
        m_attributes.value( tName )->setValue( attributes.at(index) );
        ++index;
    }
}

void NPCCardObverse::addBougthTricks(QVector<QListWidgetItem *> tricks)
{
    for ( QListWidgetItem *trick: tricks ) {
        trick->setFlags( Qt::ItemIsEnabled );
        m_pTricks->addItem( trick );
    }
}

QLabel *NPCCardObverse::createSpecialLabel(
        const QString &text,
        const QString &objName,
        const QString &style,
        const int &width,
        const int &heigth,
        Qt::Alignment aligment)
{
    QLabel *pLabel = new QLabel(text);
    pLabel->setAlignment( aligment );
    if ( "" != objName ) pLabel->setObjectName( objName );
    if ( "" != style ) pLabel->setStyleSheet( style );
    if ( 0 != width ) pLabel->setFixedWidth( width );
    if ( 0 != heigth ) pLabel->setFixedHeight( heigth );
    return pLabel;
}

QWidget *NPCCardObverse::createPersonalSection()
{
    QWidget *pWidget = new QWidget(this);
    QVBoxLayout *pLayout = new QVBoxLayout;

    pWidget->setMinimumWidth( 200 );

    QVBoxLayout *pTitleL = new QVBoxLayout;
    pTitleL->addWidget( m_pPortrait );
    pTitleL->addWidget( createSpecialLabel("Postać",
                                           "Title",
                                           m_titleStyle,
                                           0, 40) );

    QHBoxLayout *pNameL = new QHBoxLayout;
    pNameL->addWidget( new QLabel("Imię", pWidget) );
    pNameL->addWidget( m_pName );

    QVBoxLayout *pOriginL = new QVBoxLayout;
    pOriginL->addWidget( new QLabel("Pochodzenie", pWidget) );
    pOriginL->addWidget( m_pOrigin );

    QVBoxLayout *pProfessionL = new QVBoxLayout;
    pProfessionL->addWidget( new QLabel("Profesja", pWidget) );
    pProfessionL->addWidget( m_pProfession );

    QVBoxLayout *pSpecializationL = new QVBoxLayout;
    pSpecializationL->addWidget( new QLabel("Specjalizacja", pWidget) );
    pSpecializationL->addWidget( m_pSpecialization );

//    m_pRandomSicknessBtn = new QPushButton( "Losuj", pWidget );
//    m_pSickness->setReadOnly( true );

    QHBoxLayout *pSicknessHL = new QHBoxLayout;
    pSicknessHL->addWidget( new QLabel("Choroba", pWidget) );
//    pSicknessHL->addWidget( m_pRandomSicknessBtn );

    QVBoxLayout *pSicknessL = new QVBoxLayout;
    pSicknessL->addLayout( pSicknessHL );
    pSicknessL->addWidget( m_pSickness );

    QVBoxLayout *pFeature1L = new QVBoxLayout;
    pFeature1L->addWidget( new QLabel("Cecha z pochodzenia", pWidget) );
    pFeature1L->addWidget( m_pFeature1 );

    QVBoxLayout *pFeatrue2L = new QVBoxLayout;
    pFeatrue2L->addWidget( new QLabel("Cecha z profesji", pWidget) );
    pFeatrue2L->addWidget( m_pFeature2 );

    QHBoxLayout *pReputationL = new QHBoxLayout;
    pReputationL->addWidget( new QLabel("Reputacja", pWidget) );
    pReputationL->addWidget( m_pReputation );

    QHBoxLayout *pFameL = new QHBoxLayout;
    pFameL->addWidget( new QLabel("Sława", pWidget) );
    pFameL->addWidget( m_pFame );

    pLayout->addLayout( pTitleL );
    pLayout->addLayout( pNameL );
    pLayout->addLayout( pOriginL );
    pLayout->addLayout( pProfessionL );
    pLayout->addLayout( pSpecializationL );
    pLayout->addLayout( pSicknessL );
    pLayout->addLayout( pFeature1L );
    pLayout->addLayout( pFeatrue2L );
    pLayout->addLayout( pReputationL );
    pLayout->addLayout( pFameL );
    pLayout->setSpacing( 2 );
    pLayout->setMargin( 0 );

    pWidget->setLayout( pLayout );
    return pWidget;
}

NPCSkillPackWidget *NPCCardObverse::createSkillPack(const QJsonObject &skillPack)
{
    NPCSkillPackWidget* pSkillPack = new NPCSkillPackWidget( skillPack.value("name").toString() );

    connect( pSkillPack, &NPCSkillPackWidget::bougth,
             m_pProgressWidget, &NPCProgressWidget::onSkillPackBougth );
    connect( m_pProgressWidget, &NPCProgressWidget::availableSkillPointsChanged,
             pSkillPack, &NPCSkillPackWidget::onAvailableSkillPointsChanged);
    connect( pSkillPack, &NPCSkillPackWidget::refundPoints,
             m_pProgressWidget, &NPCProgressWidget::onRefundPoints );

    const QJsonArray &specs = skillPack.value("Specialization").toArray();
    for ( const QJsonValue &tSpec: specs )
        pSkillPack->addSpecialization( tSpec.toString() );

    const QJsonArray &skills = skillPack.value("skills").toArray();
    for ( const QJsonValue &tSkill: skills ) {
        SkillSpinBox *pSkillBox = new SkillSpinBox( pSkillPack );
        connect( pSkillBox, &SkillSpinBox::skillValueChanged,
                 [this, pSkillPack](const int &value, const bool &increase){
            emit m_pProgressWidget->onSkillBougth(value, pSkillPack->specializations(), increase);
        } );
        connect( m_pProgressWidget, &NPCProgressWidget::availableSkillPointsChanged,
                 pSkillBox, &SkillSpinBox::onAvailableSkillPointsChanged );
        pSkillPack->addSkill(tSkill.toString(), pSkillBox );
    }

    return pSkillPack;
}

const QHash<QString, NPCAttributeWidget *> *NPCCardObverse::attributes() const
{
    return &m_attributes;
}

const NPCProgressWidget *NPCCardObverse::progressWidget() const
{
    return m_pProgressWidget;
}

const QString NPCCardObverse::heroName() const
{
    return m_pName->text();
}

void NPCCardObverse::setAttributes(const QJsonArray &attributes)
{
    for ( const QJsonValue &tAttribute: attributes ) {
        const QJsonObject &object = tAttribute.toObject();
        const QString &name = object.value("name").toString();

        NPCAttributeWidget *pAttribute = new NPCAttributeWidget(name, m_mods, this);
        pAttribute->setValue( 0 );

        const QJsonArray &skillPacks = object.value("skillPacks").toArray();
        for ( const QJsonValue &tSkillPack: skillPacks ) {
            const QJsonObject &skillPack = tSkillPack.toObject();
            NPCSkillPackWidget *pSkillPack = createSkillPack( skillPack );
            pAttribute->addSkillPack( skillPack.value("name").toString(), pSkillPack );
        }

        m_attributes.insert( name, pAttribute );
    }
}

void NPCCardObverse::setSpecializations(const QJsonArray &specializations)
{
    int index = -1;
    for ( const QJsonValue &spec: specializations )
        m_pSpecialization->insertItem( ++index, spec.toString() );
}

void NPCCardObverse::setOrigins(const QJsonArray &origins)
{
    QStringList list;
    for ( const QJsonValue &tOrigin: origins ) {
        QString name = tOrigin.toObject().value("name").toString();
        m_origins.insert( name, tOrigin.toObject() );
        list << name;
    }

    m_pOrigin->insertItems( 0, list );
    onOriginChange( m_pOrigin->currentText() );
}

void NPCCardObverse::setProfessions(const QJsonArray &professions)
{
    QStringList list;
    for ( const QJsonValue &tProfession: professions) {
        QString name = tProfession.toObject().value("name").toString();
        m_professions.insert( name, tProfession.toObject() );
        list << name;
    }

    m_pProfession->insertItems( 0, list );
    onProfessionChanged( m_pProfession->currentText() );
}

QVBoxLayout *NPCCardObverse::column1()
{
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( createPersonalSection() );
    pLayout->addWidget( createSpecialLabel("Sztuczki",
                                           "Title",
                                           m_titleStyle,
                                           0, 40) );
    pLayout->addWidget( m_pTricks );
    pLayout->setMargin( 0 );
    pLayout->setSpacing( 1 );
    return pLayout;
}

QVBoxLayout *NPCCardObverse::column2()
{
    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addWidget( createSpecialLabel(
                            "Współczynniki i umiejętności",
                            "Title",
                            m_titleStyle,
                            0, 40) );
    pLayout->addWidget( m_attributes.value("Budowa") );
    pLayout->addWidget( m_attributes.value("Zręczność") );
    pLayout->addWidget( m_pAttributesModsInfo );

    pLayout->setMargin( 0 );
    pLayout->setSpacing( 1 );
    return pLayout;
}

QVBoxLayout *NPCCardObverse::column3()
{
    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addWidget( m_attributes.value("Charakter") );
    pLayout->addWidget( m_attributes.value("Percepcja") );
    pLayout->addWidget( createSpecialLabel("Punkty Rozwoju", "Title", m_titleStyle, 0, 40) );
    pLayout->addWidget( m_pProgressWidget );

    pLayout->setMargin( 0 );
    pLayout->setSpacing( 1 );
    return pLayout;
}

QVBoxLayout *NPCCardObverse::column4()
{
    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addWidget( m_attributes.value("Spryt") );
    pLayout->addWidget( createSpecialLabel("Rany", "Title", m_titleStyle, 0, 40) );
//    pLayout->addWidget( createWoundsSection() );
    pLayout->addWidget( createSpecialLabel("Modyfikatory", "Title", m_titleStyle, 0, 40) );
//    pLayout->addWidget( createWoundsModificatorsSection() );
//    pLayout->addWidget( createSpecialLabel("Punkty Rozwoju", "Title", m_titleStyle, 0, 40) );
//    pLayout->addWidget( m_pProgressWidget );

    pLayout->setMargin( 0 );
    pLayout->setSpacing( 1 );
    return pLayout;
}
