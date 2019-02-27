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
      m_pOriginManager(new NPCOriginManagerWidget(this)),
      m_pProfessionManager(new NPCProfessionManagerWidget(this)),
      m_pSicknessManager(new NPCSicknessManagerWidget(this)),
      m_pSkillsManager(new NPCSkillsManagerWidget(&m_attributes, this)),
      m_pTricksManager(new NPCTrickManagerWidget(m_pCard->obverse()->attributes(), this))
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

    m_pClose->setFixedWidth( 80 );
    m_pAccept->setFixedWidth( 80 );

    connect( m_pClose, &QPushButton::clicked,
             this, &NPCCreatorDialog::close );
    connect( m_pAccept, &QPushButton::clicked,
             [this](){ emit this->creationCompleted(m_pCard); this->close(); } );

    connect( m_pAttributeManager, &NPCAttributeManagerWidget::attributeChanged,
             m_pCard->obverse(), &NPCCardObverse::setAttribute );
    connect( m_pAttributeManager, &NPCAttributeManagerWidget::attributeChanged,
             m_pSkillsManager, &NPCSkillsManagerWidget::setAttributeValue );
    connect( m_pSpecializationManager, &NPCSpecializationManagerWidget::specializationChanged,
             m_pSkillsManager, &NPCSkillsManagerWidget::setSpecialization );
    connect( m_pSpecializationManager, &NPCSpecializationManagerWidget::specializationChanged,
             m_pCard->obverse(), &NPCCardObverse::setSpecialization );
    connect( m_pOriginManager, &NPCOriginManagerWidget::originNameChanged,
             m_pCard->obverse(), &NPCCardObverse::setOrigin );
    connect( m_pOriginManager, &NPCOriginManagerWidget::attributeBonusChanged,
             m_pCard->obverse(), &NPCCardObverse::setAttributeModValue );
    connect( m_pOriginManager, &NPCOriginManagerWidget::originFeatureChanged,
             m_pCard->obverse(), &NPCCardObverse::setOriginFeature );
    connect( m_pOriginManager, &NPCOriginManagerWidget::originBonusChanged,
             m_pSkillsManager, &NPCSkillsManagerWidget::setOriginBonus );

    m_pOriginManager->setOrigin( "Południowa Hegemonia" );
    m_pSpecializationManager->specializationChanged( "Technik" );

    QHBoxLayout *pButtonsL = new QHBoxLayout;
    pButtonsL->addWidget( m_pClose );
    pButtonsL->addWidget( m_pAccept );

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( m_pTabWidget );
    pLayout->addLayout( pButtonsL );

    setLayout( pLayout );
}
