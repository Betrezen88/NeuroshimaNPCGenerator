#include "NPCCreatorDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

NPCCreatorDialog::NPCCreatorDialog(QWidget *parent)
    : QDialog (parent),
      m_pTabWidget(new QTabWidget(this)),
      m_pAccept(new QPushButton("Akceptuj", this)),
      m_pClose(new QPushButton("Anuluj", this))
{
    setAttribute( Qt::WA_DeleteOnClose );
    m_pTabWidget->addTab( new QWidget(), "Atrybuty" );
    m_pTabWidget->addTab( new QWidget(), "Specjalizacja" );
    m_pTabWidget->addTab( new QWidget(), "Pochodzenie" );
    m_pTabWidget->addTab( new QWidget(), "Profesja" );
    m_pTabWidget->addTab( new QWidget(), "Choroba" );
    m_pTabWidget->addTab( new QWidget(), "Umiejętności" );
    m_pTabWidget->addTab( new QWidget(), "Sztuczki" );

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
