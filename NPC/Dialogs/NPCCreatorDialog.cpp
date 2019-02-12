#include "NPCCreatorDialog.h"
#include "../Utils/DataLoader.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

NPCCreatorDialog::NPCCreatorDialog(QWidget *parent)
    : QDialog (parent),
      m_pTabWidget(new QTabWidget(this)),
      m_pAccept(new QPushButton("Akceptuj", this)),
      m_pClose(new QPushButton("Anuluj", this)),
      m_pAttributeManager(new NPCAttributeManagerWidget(this)),
      m_pSpecializationManager(new NPCSpecializationManagerWidget(this))
{
    setAttribute( Qt::WA_DeleteOnClose );
    setMinimumSize( 500, 500 );

    m_pTabWidget->addTab( m_pAttributeManager, "Atrybuty" );
    m_pTabWidget->addTab( m_pSpecializationManager, "Specjalizacja" );
    m_pTabWidget->addTab( new QWidget(), "Pochodzenie" );
    m_pTabWidget->addTab( new QWidget(), "Profesja" );
    m_pTabWidget->addTab( new QWidget(), "Choroba" );
    m_pTabWidget->addTab( new QWidget(), "Umiejętności" );
    m_pTabWidget->addTab( new QWidget(), "Sztuczki" );

    m_pCard = new NPCCardTab();

    m_pClose->setFixedWidth( 80 );
    m_pAccept->setFixedWidth( 80 );

    connect( m_pClose, &QPushButton::clicked,
             this, &NPCCreatorDialog::close );

    QHBoxLayout *pButtonsL = new QHBoxLayout;
    pButtonsL->addWidget( m_pClose );
    pButtonsL->addWidget( m_pAccept );

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( m_pTabWidget );
    pLayout->addLayout( pButtonsL );

    setLayout( pLayout );
}
