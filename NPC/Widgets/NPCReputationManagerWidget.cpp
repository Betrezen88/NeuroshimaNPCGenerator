#include "NPCReputationManagerWidget.h"

#include "../Utils/DataLoader.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#include <QDebug>

NPCReputationManagerWidget::NPCReputationManagerWidget(QWidget *parent)
    : QWidget(parent),
      m_pLayout(new QGridLayout),
      m_pFame(new QLabel("0", this)),
      m_pPoints(new QLabel("0", this))
{
    setLayout( m_pLayout );
    m_pLayout->addWidget( reputationBox(), 0, 0, 10, 2 );
    m_pLayout->addWidget( fameBox(), 0, 2 );
    m_pLayout->addWidget( pointsBox(), 0, 3 );
    m_pLayout->addWidget( descriptionBox(), 1, 2, 9, 2 );
}

void NPCReputationManagerWidget::setPlaceReputation(const QString &name)
{
    for ( ReputationValueBox *pReputation: m_reputation )
        if ( pReputation->value() > 0 ) {
            pReputation->setValue( 0 );
            break;
        }

    if ( m_reputation.contains(name) ) {
        m_reputation.value(name)->setValue( 1 );
        m_reputation.value(name)->setMinimumValue( 1 );
    }
    else
        m_pPoints->setNum( 1 );

    emit freeReputationPointsChanged( m_pPoints->text().toInt() );
}

QGroupBox *NPCReputationManagerWidget::reputationBox()
{
    QGroupBox *pReputationBox = new QGroupBox( "Reputacja w miejscu", this );
    QGridLayout *pLayout = new QGridLayout;
    QJsonArray places = DataLoader::loadJson( ":/data/json/Reputation.json" );

    for ( const QJsonValue place: places ) {
        int row = (m_reputation.count()%2) ? m_reputation.count() : m_reputation.count()+1 ;
        int column = (m_reputation.count()%2) ? 3 : 1;
        QLabel *pPlace = new QLabel( place.toString(), this );
        ReputationValueBox *pReputation = new ReputationValueBox( this );
        pReputation->setMaximumValue( 20 );

        connect( this, &NPCReputationManagerWidget::freeReputationPointsChanged,
                 pReputation, &ReputationValueBox::onFreePointsChange );
        connect( pReputation, &ReputationValueBox::valueUp,
                 [this](){
            this->m_pPoints->setNum( this->m_pPoints->text().toInt()-1 );
            emit this->freeReputationPointsChanged( this->m_pPoints->text().toInt() );
        });
        connect( pReputation, &ReputationValueBox::valueDown,
                 [this](){
            this->m_pPoints->setNum( this->m_pPoints->text().toInt()+1 );
            emit this->freeReputationPointsChanged( this->m_pPoints->text().toInt() );
        });

        pLayout->addWidget( pPlace, row, column );
        pLayout->addWidget( pReputation, row, column+1 );

        m_reputation.insert( pPlace->text(), pReputation );
    }
    pReputationBox->setLayout( pLayout );

    return pReputationBox;
}

QGroupBox *NPCReputationManagerWidget::fameBox()
{
    QGroupBox *pFameBox = new QGroupBox( "Sława", this );
    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addWidget( m_pFame );

    pFameBox->setLayout( pLayout );
    return pFameBox;
}

QGroupBox *NPCReputationManagerWidget::pointsBox()
{
    QGroupBox *pPointsBox = new QGroupBox( "Wolne punkty reputacji", this );
    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addWidget( m_pPoints );

    pPointsBox->setLayout( pLayout );
    return pPointsBox;
}

QGroupBox *NPCReputationManagerWidget::descriptionBox()
{
    QGroupBox *pDescriptionBox = new QGroupBox( "Opis", this );
    QVBoxLayout *pLayout = new QVBoxLayout;

    QLabel *pReputationDesc = new QLabel( m_reputationDesc, pDescriptionBox );
    QLabel *pFameDesc = new QLabel( m_fameDesc, pDescriptionBox );
    pReputationDesc->setWordWrap( true );
    pFameDesc->setWordWrap( true );

    pLayout->addWidget( pReputationDesc );
    pLayout->addSpacing( 10 );
    pLayout->addWidget( pFameDesc );
    pLayout->addStretch();

    pDescriptionBox->setLayout( pLayout );
    return pDescriptionBox;
}
