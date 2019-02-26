#include "NPCCardObverse.h"

#include "Utils/DataLoader.h"

#include <QHBoxLayout>
#include <QDebug>

NPCCardObverse::NPCCardObverse(QWidget *parent)
    : QWidget(parent),
      m_pPortrait(new QLabel(this)),
      m_pName(new QLineEdit(this)),
      m_pOrigin(new QLineEdit(this)),
      m_pProfession(new QLineEdit(this)),
      m_pSpecialization(new QLineEdit(this)),
      m_pSickness(new QLineEdit(this)),
      m_pOriginFeature(new QLineEdit(this)),
      m_pFeature2(new QComboBox(this)),
      m_pReputation(new QSpinBox(this)),
      m_pFame(new QSpinBox(this)),
      m_pTricks(new QListWidget(this))
{
    setAttributes( DataLoader::loadJson(":/data/json/Attributes.json") );
    m_attributesMods = DataLoader::loadJson( ":/data/json/DifficultyLevel.json" );
    m_pOrigin->setReadOnly( true );
    m_pProfession->setReadOnly( true );
    m_pSpecialization->setReadOnly( true );
    m_pSickness->setReadOnly( true );
    m_pOriginFeature->setReadOnly( true );
    m_pAttributesModsInfo = new NPCAttributesModsInfoWidget( m_attributesMods, this );

    connect( m_pName, &QLineEdit::textChanged,
             this, &NPCCardObverse::heroNameChanged );
    connect( m_pFeature2, &QComboBox::currentTextChanged, [this](){
        onFeatureChanged( m_pFeature2, m_professionFeatures );
    } );

    QHBoxLayout *pLayout = new QHBoxLayout;
    setLayout( pLayout );
    pLayout->addLayout( column1() );
    pLayout->addLayout( column2() );
    pLayout->addLayout( column3() );
    pLayout->addLayout( column4() );

    m_pName->setText( "Nowa Postać" );
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
            pFeature->setToolTip( "<div style=\"word-wrap: break-word;\" align=\"justify\">"
                                  +feature.value("description").toString()+"</div>");
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

void NPCCardObverse::setAttribute(const QString &name, const int &value)
{
    m_attributes[name]->setValue( value );
}

void NPCCardObverse::addBougthTricks(QVector<QListWidgetItem *> tricks)
{
    for ( QListWidgetItem *trick: tricks ) {
        trick->setFlags( Qt::ItemIsEnabled );
        m_pTricks->addItem( trick );
    }
}

void NPCCardObverse::setSickness(const QJsonObject &sickness)
{
    m_sickness = sickness;
    m_pSickness->setText( m_sickness.value("name").toString() );
    setSicknessTooltip( m_sickness );
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

    QHBoxLayout *pSicknessHL = new QHBoxLayout;
    pSicknessHL->addWidget( new QLabel("Choroba", pWidget) );

    QVBoxLayout *pSicknessL = new QVBoxLayout;
    pSicknessL->addLayout( pSicknessHL );
    pSicknessL->addWidget( m_pSickness );

    QVBoxLayout *pOriginFeatureL = new QVBoxLayout;
    pOriginFeatureL->addWidget( new QLabel("Cecha z pochodzenia", pWidget) );
    pOriginFeatureL->addWidget( m_pOriginFeature );

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
    pLayout->addLayout( pOriginFeatureL );
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

    const QJsonArray &specs = skillPack.value("Specialization").toArray();
    for ( const QJsonValue &tSpec: specs )
        pSkillPack->addSpecialization( tSpec.toString() );

    const QJsonArray &skills = skillPack.value("skills").toArray();
    for ( const QJsonValue &tSkill: skills ) {
        SkillSpinBox *pSkillBox = new SkillSpinBox( pSkillPack );
        pSkillPack->addSkill(tSkill.toString(), pSkillBox );
    }

    return pSkillPack;
}

const QHash<QString, NPCAttributeWidget *> *NPCCardObverse::attributes() const
{
    return &m_attributes;
}

const QString NPCCardObverse::heroName() const
{
    return m_pName->text();
}

const QString NPCCardObverse::origin() const
{
    return m_pOrigin->text();
}

const QString NPCCardObverse::profession() const
{
    return m_pProfession->text();
}

const QString NPCCardObverse::specialization() const
{
    return m_pSpecialization->text();
}

const QString NPCCardObverse::sickness() const
{
    return m_pSickness->text();
}

const QString NPCCardObverse::originFeature() const
{
    return m_pOriginFeature->text();
}

const QString NPCCardObverse::professionFeature() const
{
    return m_pFeature2->currentText();
}

int NPCCardObverse::reputation() const
{
    return m_pReputation->value();
}

int NPCCardObverse::fame() const
{
    return m_pFame->value();
}

const QListWidget *NPCCardObverse::tricks() const
{
    return m_pTricks;
}

void NPCCardObverse::setOrigin(const QJsonObject &origin)
{
    if ( !m_origin.isEmpty() ) {
        const QJsonObject &attribute = m_origin.value("attribute").toObject();
        setAttributeMod( attribute.value("name").toString(), 0 );
        undoBonus( m_origin.value("feature").toObject().value("bonus").toObject() );
    }

    m_origin = origin;
    const QJsonObject &attribute = m_origin.value("attribute").toObject();

    m_pOrigin->setText( m_origin.value("name").toString() );
    setAttributeMod( attribute.value("name").toString(), attribute.value("value").toInt() );
    setOriginFeature( origin.value("feature").toObject() );
}

void NPCCardObverse::setProfession(const QJsonObject &profession)
{
    m_profession = profession;

    m_pProfession->setText( profession.value("name").toString() );
}

void NPCCardObverse::setSpecialization(const QString &spec)
{
    if ( spec != m_pSpecialization->text() )
        m_pSpecialization->setText( spec );
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

void NPCCardObverse::setSicknessTooltip(const QJsonObject &sickness)
{
    QString tooltip;
    const QJsonObject &symptoms = sickness.value("symptoms").toObject();

    tooltip = "<b>Nazwa:</b> " + sickness.value("name").toString()
            + "<br><br><b>Opis:</b> " + sickness.value("description").toString()
            + "<br><br><b>Lekarstwo:</b> " + sickness.value("cure").toString()
            + "<br><br><b>Symptomy</b>:"
            + "<ul>"
            + "<li><b>Pierwsze symptomy:</b> " + symptoms.value("1").toObject().value("description").toString()
            + "<br><b>Kary:</b>" + symptoms.value("1").toObject().value("penalties").toString() + "</li>"
            + "<li><b>Stan ostry:</b> " + symptoms.value("2").toObject().value("description").toString()
            + "<br><b>Kary:</b>" + symptoms.value("2").toObject().value("penalties").toString() + "</li>"
            + "<li><b>Stan krytyczny:</b> " + symptoms.value("3").toObject().value("description").toString()
            + "<br><b>Kary:</b>" + symptoms.value("3").toObject().value("penalties").toString() + "</li>"
            + "<li><b>Stan terminalny:</b> " + symptoms.value("4").toObject().value("description").toString()
            + "<br><b>Kary:</b>" + symptoms.value("4").toObject().value("penalties").toString() + "</li>"
            + "</ul>";

    m_pSickness->setToolTip( tooltip );
    m_pSickness->setToolTipDuration( 5 * 60 * 100 );
}

void NPCCardObverse::setAttributeMod(const QString &name, const int &value)
{
    m_attributes[name]->setModValue( value );
}

void NPCCardObverse::setOriginFeature(const QJsonObject &feature)
{
    m_pOriginFeature->setText( feature.value("name").toString() );
    m_pOriginFeature->setToolTip( feature.value("description").toString() );
    applyBonus( feature.value("bonus").toObject() );
}

void NPCCardObverse::applyBonus(const QJsonObject &bonus)
{
    const QJsonObject &object = bonus.value("object").toObject();
    if ( "skillpack" == object.value("type").toString() ) {
        const QString &skillpackName = object.value("name").toString();
        for ( NPCAttributeWidget *attribute: m_attributes ) {
            if ( attribute->skillPacks()->contains(skillpackName) ) {
                NPCSkillPackWidget *skillpack = attribute->skillPacks()->value(skillpackName);
                for ( QPair<const QLabel*, SkillSpinBox*> skill: skillpack->skills() ) {
                    skill.second->setValue( object.value("value").toInt() );
                    skill.second->setMinimum( object.value("value").toInt() );
                }
            }
        }
    }
}

void NPCCardObverse::undoBonus(const QJsonObject &bonus)
{
    const QJsonObject &object = bonus.value("object").toObject();
    if ( "skillpack" == object.value("type").toString() ) {
        const QString &skillpackName = object.value("name").toString();
        for ( NPCAttributeWidget *attribute: m_attributes ) {
            if ( attribute->skillPacks()->contains(skillpackName) ) {
                NPCSkillPackWidget *skillpack = attribute->skillPacks()->value(skillpackName);
                for ( QPair<const QLabel*, SkillSpinBox*> skill: skillpack->skills() ) {
                    skill.second->setMinimum( 0 );
                    skill.second->setValue( 0 );
                }
            }
        }
    }
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
