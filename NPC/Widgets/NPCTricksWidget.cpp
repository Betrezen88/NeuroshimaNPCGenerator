#include "NPCTricksWidget.h"

#include "../Utils/DataLoader.h"

#include <QJsonValue>
#include <QJsonObject>
#include <QVBoxLayout>
#include <QHBoxLayout>

NPCTricksWidget::NPCTricksWidget(QWidget *parent)
    : QWidget(parent),
      m_pTricks(new QListWidget(this)),
      m_pManageBtn(new QPushButton("Dodaj/Usuń", this))
{
    connect( m_pManageBtn, &QPushButton::clicked, this, &NPCTricksWidget::clicked );

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( m_pTricks );
    pLayout->addWidget( m_pManageBtn );
    pLayout->setMargin( 0 );
    pLayout->setSpacing( 1 );
    setLayout( pLayout );
}

bool NPCTricksWidget::fitRequirements(const QJsonArray &attributes, const QJsonArray &skills) const
{
    bool passRequirements{ false };
    if ( !attributes.empty() ) {
        passRequirements = true;
    }
    if ( !skills.empty() ) {
        passRequirements = true;
    }
    return passRequirements;
}
