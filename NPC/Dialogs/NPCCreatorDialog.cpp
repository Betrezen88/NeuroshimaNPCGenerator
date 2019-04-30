#include "NPCCreatorDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QDebug>

NPCCreatorDialog::NPCCreatorDialog(QWidget *parent)
    : QDialog (parent),
      m_attributes(DataLoader::loadJson( ":/data/json/Attributes.json" )),
      m_pTabWidget(new QTabWidget(this)),
      m_pAccept(new QPushButton("Akceptuj", this)),
      m_pClose(new QPushButton("Anuluj", this)),
      m_pCard(new NPCCardTab()),
      m_pAttributeManager(new NPCAttributeManagerWidget(&m_attributes, this)),
      m_pSpecializationManager(new NPCSpecializationManagerWidget(this)),
      m_pOriginManager(new NPCOriginManagerWidget(m_pSpecializationManager, this)),
      m_pProfessionManager(new NPCProfessionManagerWidget(this)),
      m_pSicknessManager(new NPCSicknessManagerWidget(this)),
      m_pSkillsManager(new NPCSkillsManagerWidget(&m_attributes, this)),
      m_pTricksManager(new NPCTrickManagerWidget(m_pSkillsManager->attributes(), this)),
      m_pBioManager(new NPCBioManagerWidget(this)),
      m_pReputationManager(new NPCReputationManagerWidget(this))
{
    setAttribute( Qt::WA_DeleteOnClose );
    setMinimumSize( 500, 880 );

    m_pTabWidget->addTab( m_pAttributeManager, "Atrybuty" );
    m_pTabWidget->addTab( m_pSpecializationManager, "Specjalizacja" );
    m_pTabWidget->addTab( m_pOriginManager, "Pochodzenie" );
    m_pTabWidget->addTab( m_pProfessionManager, "Profesja" );
    m_pTabWidget->addTab( m_pSicknessManager, "Choroba" );
    m_pTabWidget->addTab( m_pSkillsManager, "Umiejętności" );
    m_pTabWidget->addTab( m_pTricksManager, "Sztuczki" );
    m_pTabWidget->addTab( m_pReputationManager, "Reputacja" );
    m_pTabWidget->addTab( m_pBioManager, "Personalia" );

    m_pClose->setFixedWidth( 80 );
    m_pAccept->setFixedWidth( 80 );

    connect( m_pTabWidget, &QTabWidget::tabBarClicked,
             [this](int index){
                if ( 6 == index )
                    m_pTricksManager->loadTricks();
    } );
    connect( m_pClose, &QPushButton::clicked,
             this, &NPCCreatorDialog::close );
    connect( m_pAccept, &QPushButton::clicked,
             [this](){
        for ( NPCAttributeWidget *tAttribute: *m_pSkillsManager->attributes() ) {
            const QString &attribute = m_pSkillsManager->attributes()->key( tAttribute );
            for ( NPCSkillPackWidget *tSkillpack: *tAttribute->skillPacks() ) {
                const QString &skillpack = tAttribute->skillPacks()->key( tSkillpack );
                for ( QPair<const QLabel*, SkillSpinBox*> tSkill: tSkillpack->skills() )
                    m_pCard->obverse()->setSkill( attribute,
                                                  skillpack,
                                                  tSkill.first->text(),
                                                  tSkill.second->value() );
            }
        }

        m_pCard->obverse()->setTricks( m_pTricksManager->tricks() );
        emit this->creationCompleted(m_pCard); this->close();
    } );

    connect( m_pAttributeManager, &NPCAttributeManagerWidget::attributeChanged,
             m_pSkillsManager, &NPCSkillsManagerWidget::setAttributeValue );
    connect( m_pSpecializationManager, &NPCSpecializationManagerWidget::specializationChanged,
             m_pSkillsManager, &NPCSkillsManagerWidget::setSpecialization );
    connect( m_pOriginManager, &NPCOriginManagerWidget::originNameChanged,
             m_pReputationManager, &NPCReputationManagerWidget::setPlaceReputation );
    connect( m_pOriginManager, &NPCOriginManagerWidget::attributeBonusChanged,
             m_pSkillsManager, &NPCSkillsManagerWidget::setAttributeModValue );
    connect( m_pOriginManager, &NPCOriginManagerWidget::bonusSkillChanged,
             m_pSkillsManager, &NPCSkillsManagerWidget::setBonusSkills );
    connect( m_pOriginManager, &NPCOriginManagerWidget::addBonusTrick,
             m_pTricksManager, &NPCTrickManagerWidget::setBonusTrick );
    connect( m_pOriginManager, &NPCOriginManagerWidget::removeBonusTrick,
             m_pTricksManager, &NPCTrickManagerWidget::removeBonusTrick );

    connect( m_pOriginManager, &NPCOriginManagerWidget::bonusSkillpointsChanged,
             m_pSkillsManager, &NPCSkillsManagerWidget::setBonusSpecPoints );

    connect( m_pProfessionManager, &NPCProfessionManagerWidget::bonusSkillChanged,
             m_pSkillsManager, &NPCSkillsManagerWidget::setBonusSkills );

    connect( m_pAttributeManager, &NPCAttributeManagerWidget::attributeChanged,
             m_pCard->obverse(), &NPCCardObverse::setAttribute );

    connect( m_pOriginManager, &NPCOriginManagerWidget::originNameChanged,
             m_pCard->obverse(), &NPCCardObverse::setOrigin );
    connect( m_pOriginManager, &NPCOriginManagerWidget::originFeatureChanged,
             m_pCard->obverse(), &NPCCardObverse::setOriginFeature );
    connect( m_pOriginManager, &NPCOriginManagerWidget::attributeBonusChanged,
             m_pCard->obverse(), &NPCCardObverse::setAttributeModValue );
    connect( m_pOriginManager, &NPCOriginManagerWidget::originBonusChanged,
             m_pCard->obverse(), &NPCCardObverse::setOriginBonus );

    connect( m_pProfessionManager, &NPCProfessionManagerWidget::professionNameChanged,
             m_pCard->obverse(), &NPCCardObverse::setProfession );
    connect( m_pProfessionManager, &NPCProfessionManagerWidget::professionFeatureChanged,
             m_pCard->obverse(), &NPCCardObverse::setProfessionFeature );
    connect( m_pProfessionManager, &NPCProfessionManagerWidget::professionBonusChanged,
             m_pCard->obverse(), &NPCCardObverse::setProfessionBonus );

    connect( m_pSpecializationManager, &NPCSpecializationManagerWidget::specializationChanged,
             m_pCard->obverse(), &NPCCardObverse::setSpecialization );

    connect( m_pBioManager, &NPCBioManagerWidget::nameChanged,
             m_pCard->obverse(), &NPCCardObverse::setName );
    connect( m_pBioManager, &NPCBioManagerWidget::portraitChanged,
             m_pCard->obverse(), &NPCCardObverse::setPortrait );

    connect( m_pSicknessManager, &NPCSicknessManagerWidget::sicknessChanged,
             m_pCard->obverse(), &NPCCardObverse::setSickness );

    m_pSpecializationManager->specializationChanged( "Technik" );
    m_pOriginManager->setOrigin( "Południowa Hegemonia" );
    m_pProfessionManager->setProfession( "Chemik" );

    QHBoxLayout *pButtonsL = new QHBoxLayout;
    pButtonsL->addWidget( m_pClose );
    pButtonsL->addWidget( m_pAccept );

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( m_pTabWidget );
    pLayout->addLayout( pButtonsL );

    setLayout( pLayout );
}
