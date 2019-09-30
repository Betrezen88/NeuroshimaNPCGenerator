#include "NPCCardObverse.h"

#include "Utils/DataLoader.h"

#include "Widgets/NPCAttributeView.h"
#include "Widgets/NPCAttributeWidget.h"
#include "Widgets/NPCSkillpackView.h"
#include "Widgets/NPCOtherSkillsView.h"
#include "Widgets/NPCReputationView.h"

#include <QComboBox>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
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
      m_pProfessionFeature(new QLineEdit(this)),
      m_pReputation(new QPushButton("Reputacja", this)),
      m_pFame(new QLabel("0",this)),
      m_pBonus(new QLabel(this)),
      m_pTricks(new QListWidget(this)),
      m_pOtherSkills(new NPCOtherSkillsView("Inne Umiejętności", this)),
      m_pReputationView(new NPCReputationView(this)),
      m_pReputationDialog(createReputationDialog())
{
    connect( m_pReputation, &QPushButton::clicked,
             m_pReputationDialog, &QDialog::show );
    connect( m_pReputationView, &NPCReputationView::fameChanged,
             m_pFame, QOverload<int>::of(&QLabel::setNum) );

    setAttributes( DataLoader::loadJson(":/data/json/Attributes.json") );
    m_attributesMods = DataLoader::loadJson( ":/data/json/DifficultyLevel.json" );
    m_pPortrait->setFixedSize( 140, 200 );
    m_pPortrait->setScaledContents( true );

    m_pOrigin->setReadOnly( true );
    m_pProfession->setReadOnly( true );
    m_pSpecialization->setReadOnly( true );
    m_pSickness->setReadOnly( true );
    m_pOriginFeature->setReadOnly( true );
    m_pBonus->setWordWrap( true );
    m_pBonus->setObjectName( "Bonus" );
    m_pBonus->setStyleSheet( m_bonusLabel );

    QHBoxLayout *pLayout = new QHBoxLayout;
    setLayout( pLayout );
    pLayout->addLayout( column1() );
    pLayout->addLayout( column2() );
    pLayout->addLayout( column3() );
    pLayout->addLayout( column4() );
}

void NPCCardObverse::setAttribute(const QString &name, const int &value)
{
    m_attributes[name]->setValue( value );
}

void NPCCardObverse::setTricks(QVector<QListWidgetItem *> tricks)
{
    for ( QListWidgetItem *trick: tricks ) {
        trick->setFlags( Qt::ItemIsEnabled );
        m_pTricks->addItem( trick );
    }
}

void NPCCardObverse::setSickness(const QString &name, const QString &description)
{
    m_pSickness->setText( name );
    m_pSickness->setToolTip( description );
    m_pSickness->setToolTipDuration( 5 * 60 * 100 );
}

void NPCCardObverse::setSkill(const QString &attribute,
                              const QString &skillpack,
                              const QString &skill,
                              const int &value)
{
    m_attributes.value(attribute)->setSkillValue(skillpack, skill, value);
}

void NPCCardObverse::setOtherSkill(const QString &name, const QString &attribute, const int &value)
{
    m_pOtherSkills->addSkill( name, attribute, value );
}

void NPCCardObverse::setOriginBonus(const QString &bonus)
{
    m_originBonus = bonus;
    updateBonusLabel();
}

void NPCCardObverse::setProfessionBonus(const QString &bonus)
{
    m_professionBonus = bonus;
    updateBonusLabel();
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
    pFeatrue2L->addWidget( m_pProfessionFeature );

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
    pLayout->addWidget( m_pReputation );
    pLayout->addLayout( pFameL );
    pLayout->setSpacing( 2 );
    pLayout->setMargin( 0 );

    pWidget->setLayout( pLayout );
    return pWidget;
}

NPCSkillpackView *NPCCardObverse::createSkillPack(const QJsonObject &skillPack)
{
    QStringList specs, skills;
    const QJsonArray &tSpecs = skillPack.value("Specialization").toArray();
    for ( const QJsonValue tSpec: tSpecs )
        specs << tSpec.toString();

    const QJsonArray &tSkills = skillPack.value("skills").toArray();
    for ( const QJsonValue tSkill: tSkills )
        skills << tSkill.toString();

    return new NPCSkillpackView( skillPack.value("name").toString(), specs, skills, this );
}

const QHash<QString, NPCAttributeView *> *NPCCardObverse::attributes() const
{
    return &m_attributes;
}

QPixmap NPCCardObverse::portrait() const
{
    return *m_pPortrait->pixmap();
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
    return m_pProfessionFeature->text();
}

const QString NPCCardObverse::originFeatureDescription() const
{
    return m_originFeatureDescription;
}

const QString NPCCardObverse::professionFeatureDescription() const
{
    return m_professionFeaturesDescription;
}

NPCReputationView *NPCCardObverse::reputation() const
{
    return m_pReputationView;
}

const QListWidget *NPCCardObverse::tricks() const
{
    return m_pTricks;
}

NPCOtherSkillsView *NPCCardObverse::otherSkills()
{
    return m_pOtherSkills;
}

void NPCCardObverse::setName(const QString &name)
{
    m_pName->setText( name );
}

void NPCCardObverse::setPortrait(const QPixmap portrait)
{
    portrait.scaled( 140, 200, Qt::KeepAspectRatio );
    m_pPortrait->setPixmap( portrait );
}

void NPCCardObverse::setOrigin(const QString &name)
{
    m_pOrigin->setText( name );
}

void NPCCardObverse::setOriginFeature(const QString &name, const QString &description)
{
    m_originFeatureDescription = description;
    m_pOriginFeature->setText( name );
    m_pOriginFeature->setToolTip( "<div style=\"width: 250px;\">"
                                  +description+"</div>" );
}

void NPCCardObverse::setProfession(const QString &name)
{
    m_pProfession->setText( name );
}

void NPCCardObverse::setProfessionFeature(const QString &name, const QString &description)
{
    m_professionFeaturesDescription = description;
    m_pProfessionFeature->setText( name );
    m_pProfessionFeature->setToolTip( "<div style=\"width: 250px;\">"
                                      +description+"</div>" );
}

void NPCCardObverse::setSpecialization(const QString &spec)
{
    if ( spec != m_pSpecialization->text() )
        m_pSpecialization->setText( spec );
}

void NPCCardObverse::setAttributeModValue(const QString &name, const int &value)
{
    for ( NPCAttributeView *tAttribute: m_attributes )
        if ( tAttribute->modValue() ) {
            tAttribute->setModValue( 0 );
            break;
        }
    m_attributes.value(name)->setModValue( value );
}

void NPCCardObverse::setAttributes(const QJsonArray &attributes)
{
    for ( const QJsonValue &tAttribute: attributes ) {
        const QJsonObject &object = tAttribute.toObject();
        const QString &name = object.value("name").toString();

        NPCAttributeView *pAttribute = new NPCAttributeView(name, 0, 0);
        pAttribute->setValue( 0 );

        const QJsonArray &skillPacks = object.value("skillPacks").toArray();
        for ( const QJsonValue &tSkillPack: skillPacks ) {
            QStringList specs, skills;
            const QJsonObject &skillPack = tSkillPack.toObject();
            const QJsonArray &tSpecs = skillPack.value("Specialization").toArray();
            for ( const QJsonValue tSpec: tSpecs )
                specs << tSpec.toString();
            const QJsonArray &tSkills = skillPack.value("skills").toArray();
            for ( const QJsonValue tSkill: tSkills )
                skills << tSkill.toString();

            pAttribute->addSkillpack( new NPCSkillpackView( skillPack.value("name").toString(), specs, skills, this ) );
        }
        m_attributes.insert( name, pAttribute );
    }
}

void NPCCardObverse::updateBonusLabel()
{
    m_pBonus->setText( m_originBonus + "\n\n" + m_professionBonus );
}

QDialog *NPCCardObverse::createReputationDialog()
{
    QDialog *pDialog = new QDialog();
    pDialog->setWindowTitle( "Reputacja postaci" );
    pDialog->setMinimumWidth( 450 );

    QPushButton *pOkBtn = new QPushButton( "Zamknij" );
    connect( pOkBtn, &QPushButton::clicked,
             pDialog, &QDialog::close );

    QHBoxLayout *pButtonRow = new QHBoxLayout;
    pButtonRow->addSpacerItem( new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum) );
    pButtonRow->addWidget( pOkBtn );
    pButtonRow->addSpacerItem( new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum) );

    QVBoxLayout *pDialogL = new QVBoxLayout;
    pDialogL->addWidget( m_pReputationView );
    pDialogL->addLayout( pButtonRow );

    pDialog->setLayout( pDialogL );

    return  pDialog;
}

QVBoxLayout *NPCCardObverse::column1()
{
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( createPersonalSection() );
    pLayout->addWidget( createSpecialLabel("Bonusy",
                                           "Title",
                                           m_titleStyle,
                                           0, 40) );
    pLayout->addWidget( m_pBonus );
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

    pLayout->addWidget( m_attributes.value("Budowa") );
    pLayout->addWidget( m_attributes.value("Zręczność") );

    pLayout->addStretch();
    pLayout->setMargin( 0 );
    pLayout->setSpacing( 1 );
    return pLayout;
}

QVBoxLayout *NPCCardObverse::column3()
{
    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addWidget( m_attributes.value("Charakter") );
    pLayout->addWidget( m_attributes.value("Percepcja") );

    pLayout->addStretch();
    pLayout->setMargin( 0 );
    pLayout->setSpacing( 1 );
    return pLayout;
}

QVBoxLayout *NPCCardObverse::column4()
{
    QHBoxLayout *pOtherLayout = new QHBoxLayout;
    pOtherLayout->addWidget( m_pOtherSkills, 0, Qt::AlignRight );
    pOtherLayout->addSpacing( 9 );

    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addWidget( m_attributes.value("Spryt") );
    pLayout->addLayout( pOtherLayout );

    pLayout->addStretch();
    pLayout->setMargin( 0 );
    pLayout->setSpacing( 1 );
    return pLayout;
}
