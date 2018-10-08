#include "NPCCard.h"

#include <QCheckBox>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QFile>
#include <QJsonDocument>

#include <QDebug>

NPCCard::NPCCard(QWidget *parent)
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
      m_pProgressWidget(new NPCProgressWidget( m_pSpecialization, this)),
      m_dice(Dice(20))
{
    initCardData();

    QHBoxLayout *pAll = new QHBoxLayout;
    setLayout( pAll );

    pAll->addLayout( column1() );
    pAll->addLayout( column2() );
    pAll->addLayout( column3() );
    pAll->addLayout( column4() );

}

void NPCCard::onOriginChange(const QString &name)
{
    const QJsonObject origin = m_origins.value( name );
    QStringList featureNames;
    m_originFeatures = origin.value("features").toArray();

    for ( const QJsonValue &tOrigin: m_originFeatures )
        featureNames << tOrigin.toObject().value("name").toString();

    m_pFeature1->clear();
    m_pFeature1->insertItems( 0, featureNames );
    onFeatureChanged( m_pFeature1, m_originFeatures );
}

void NPCCard::onProfessionChanged(const QString &name)
{
    const QJsonObject profession = m_professions.value( name );
    QStringList featureNames;
    m_professionFeatures = profession.value("features").toArray();

    for (const QJsonValue &tProfession: m_professionFeatures)
        featureNames << tProfession.toObject().value("name").toString();

    m_pFeature2->clear();
    m_pFeature2->insertItems( 0, featureNames );
    onFeatureChanged( m_pFeature2, m_professionFeatures );
}

void NPCCard::onFeatureChanged(QComboBox *pFeature, const QJsonArray &features)
{
    for ( const QJsonValue &tFeature: features ) {
        const QJsonObject &feature = tFeature.toObject();
        if ( feature.value("name").toString() == pFeature->currentText() ) {
            pFeature->setToolTip( feature.value("description").toString() );
            break;
        }
    }
}

void NPCCard::initCardData()
{
    QJsonArray attributes = loadJson( ":/Attributes.json" );
    QJsonArray specializations = loadJson( ":/Specializations.json" );
    QJsonArray origins = loadJson( ":/Origins.json" );
    QJsonArray professions = loadJson( ":/Professions.json" );
    m_sickness = loadJson( ":/Sickness.json" );

    fillSpecializations( specializations );
    fillAttributes( attributes );
    fillOrigins( origins );
    fillProfessions( professions );
}

QWidget *NPCCard::createPersonalSection()
{
    QWidget *pWidget = new QWidget(this);
    QVBoxLayout *pLayout = new QVBoxLayout;

    QVBoxLayout *pTitleL = new QVBoxLayout;
    pTitleL->addWidget( m_pPortrait );
    pTitleL->addWidget( createLabel("Postać", "Title", m_titleStyle, 0, 40) );

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

    m_pSickness->setReadOnly( true );
    QHBoxLayout *pSicknessHL = new QHBoxLayout;
    pSicknessHL->addWidget( new QLabel("Choroba", pWidget) );
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

    connect( m_pOrigin, &QComboBox::currentTextChanged, this, &NPCCard::onOriginChange );
    connect( m_pProfession,  &QComboBox::currentTextChanged, this, &NPCCard::onProfessionChanged );
    connect( m_pFeature1, &QComboBox::currentTextChanged, [this](){
        onFeatureChanged( m_pFeature1, m_originFeatures );
    } );
    connect( m_pFeature2, &QComboBox::currentTextChanged, [this](){
        onFeatureChanged( m_pFeature2, m_professionFeatures );
    } );

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

QWidget *NPCCard::createTricksSection()
{
    QWidget *pWidget = new QWidget(this);
    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addWidget( createLabel("Sztuczki", "Title", m_titleStyle, 0, 40) );
    pLayout->addWidget( m_pTricks );
    pLayout->setSpacing( 1 );
    pLayout->setMargin( 0 );

    pWidget->setLayout( pLayout );
    return pWidget;
}

QWidget *NPCCard::createModificatorSection()
{
    QWidget *pWidget = new QWidget(this);
    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addWidget( createLabel("Modyfikatory do cech z poziomów trudności",
                                    "ModTitle",
                                    m_modTitle,
                                    0, 25) );
    pLayout->addWidget( createModificatorWidget() );
    pLayout->setSpacing( 0 );
    pLayout->setMargin( 0 );
    pWidget->setLayout( pLayout );
    return pWidget;
}

QWidget *NPCCard::createModificatorWidget()
{
    QWidget *pWidget = new QWidget();
    pWidget->setObjectName( "CellWidget" );
    pWidget->setStyleSheet( m_cellWidget );
    QHBoxLayout *pLayout = new QHBoxLayout;

    for ( int i=0; i<m_shortModsList.size(); ++i ) {
        QVBoxLayout *pCellL = new QVBoxLayout;
        pCellL->addWidget( createLabel(m_shortModsList.at(i),
                                       "CellTitle",
                                       m_cellTitle) );
        pCellL->addWidget( createLabel(QString::number(m_modsVals.at(i)),
                                       "CellValue",
                                       m_cellValue) );
        pLayout->addLayout( pCellL );
    }

    pLayout->setSpacing( 0 );
    pWidget->setLayout( pLayout );
    return pWidget;
}

QWidget *NPCCard::createWoundsSection()
{
    QWidget *pWidget = new QWidget(this);
    pWidget->setObjectName( "WoundsWidget" );
    pWidget->setStyleSheet( m_woundsWidget );
    QVBoxLayout *pLayout = new QVBoxLayout;

    for ( const QString &tName: m_woundsList ) {
        QHBoxLayout *pRowL = new QHBoxLayout;
        pRowL->addWidget( new QLabel(tName, pWidget) );
        pRowL->addWidget( new QSpinBox(pWidget) );
        pLayout->addLayout( pRowL );
    }
    pLayout->setSpacing( 1 );
    pWidget->setLayout( pLayout );
    return pWidget;
}

QWidget *NPCCard::createWoundsModificatorsSection()
{
    QTextEdit *pTextEdit = new QTextEdit(this);
    pTextEdit->setObjectName( "WoundsModificator" );
    pTextEdit->setStyleSheet( m_woundsModificatorsWidget );
    pTextEdit->setFixedHeight( 80 );
    pTextEdit->setWordWrapMode( QTextOption::WordWrap );
    return pTextEdit;
}

NPCSkillPackWidget *NPCCard::createSkillPack(const QJsonObject &skillPack)
{
    NPCSkillPackWidget* pSkillPack = new NPCSkillPackWidget( skillPack.value("name").toString() );

    connect( pSkillPack, &NPCSkillPackWidget::bougth,
             m_pProgressWidget, &NPCProgressWidget::onSkillPackBougth );
    connect( m_pProgressWidget, &NPCProgressWidget::availableSkillPointsChanged,
             pSkillPack, &NPCSkillPackWidget::onAvailableSkillPointsChanged);
    connect( pSkillPack, &NPCSkillPackWidget::refundPoints,
             m_pProgressWidget, &NPCProgressWidget::onRefundPoints );

    const QJsonArray &specs = skillPack.value("Specializations").toArray();
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

void NPCCard::fillAttributes(const QJsonArray &attributes)
{
    for ( const QJsonValue &tAttribute: attributes ) {
        const QJsonObject &object = tAttribute.toObject();
        const QString &name = object.value("name").toString();

        NPCAttributeWidget *pAttribute = new NPCAttributeWidget(name, m_mods, this);

        const QJsonArray &skillPacks = object.value("skillPacks").toArray();
        for ( const QJsonValue &tSkillPack: skillPacks ) {
            const QJsonObject &skillPack = tSkillPack.toObject();
            NPCSkillPackWidget *pSkillPack = createSkillPack( skillPack );
            pAttribute->addSkillPack( skillPack.value("name").toString(), pSkillPack );
        }

        m_attributes.insert( name, pAttribute );
    }
}

void NPCCard::fillSpecializations(const QJsonArray &specializations)
{
    int index = -1;
    for ( const QJsonValue &spec: specializations )
        m_pSpecialization->insertItem( ++index, spec.toString() );
}

void NPCCard::fillOrigins(const QJsonArray &origins)
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

void NPCCard::fillProfessions(const QJsonArray &professions)
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

QLabel *NPCCard::createLabel(const QString &text,
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

QVBoxLayout *NPCCard::column1()
{
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( createPersonalSection() );
    pLayout->addWidget( createTricksSection() );
    pLayout->setMargin( 0 );
    pLayout->setSpacing( 1 );
    return pLayout;
}

QVBoxLayout *NPCCard::column2()
{
    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addWidget( createLabel("Współczynniki i umiejętności", "Title", m_titleStyle, 0, 40) );
    pLayout->addWidget( m_attributes.value("Budowa") );
    pLayout->addWidget( m_attributes.value("Zręczność") );
    pLayout->addWidget( createModificatorSection() );

    pLayout->setMargin( 0 );
    pLayout->setSpacing( 1 );
    return pLayout;
}

QVBoxLayout *NPCCard::column3()
{
    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addWidget( m_attributes.value("Charakter") );
    pLayout->addWidget( m_attributes.value("Percepcja") );

    pLayout->setMargin( 0 );
    pLayout->setSpacing( 1 );
    return pLayout;
}

QVBoxLayout *NPCCard::column4()
{
    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addWidget( m_attributes.value("Spryt") );
    pLayout->addWidget( createLabel("Rany", "Title", m_titleStyle, 0, 40) );
    pLayout->addWidget( createWoundsSection() );
    pLayout->addWidget( createLabel("Modyfikatory", "Title", m_titleStyle, 0, 40) );
    pLayout->addWidget( createWoundsModificatorsSection() );
    pLayout->addWidget( createLabel("Punkty Rozwoju", "Title", m_titleStyle, 0, 40) );
    pLayout->addWidget( m_pProgressWidget );

    pLayout->setMargin( 0 );
    pLayout->setSpacing( 1 );
    return pLayout;
}

QJsonArray NPCCard::loadJson(const QString &fileName)
{
    QJsonArray array;
    QFile file( fileName );
    file.open( QIODevice::ReadOnly );

    if ( file.isOpen() ) {
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson( file.readAll(), &error );
        if ( doc.isEmpty() )
            qDebug() << "Parsing error in file " << fileName << ": " << error.errorString();
        array = doc.array();
    }
    else {
        qDebug() << "File " << fileName << " not open !";
    }
    return array;
}
