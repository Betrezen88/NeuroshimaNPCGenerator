#include "NPCReputationView.h"
#include "NPC/Utils/DataLoader.h"

#include <QLabel>
#include <QJsonArray>
#include <QJsonValue>
#include <QVBoxLayout>

NPCReputationView::NPCReputationView(QWidget *parent)
    : NPCCustomWidget (parent),
      m_pTitle( new QLabel("Reputacja", this) ),
      m_pLayout( new QVBoxLayout )
{
    connect( this, &NPCReputationView::reputationChanged,
             this, &NPCReputationView::updateFame );

    m_pTitle->setFixedHeight( 50 );
    m_pTitle->setAlignment( Qt::AlignCenter );
    m_pTitle->setObjectName( "Title" );
    m_pTitle->setStyleSheet( m_titleStyle );

    setLayout( m_pLayout );
    m_pLayout->addWidget( m_pTitle );
    m_pLayout->addWidget( createLayout() );
}

int NPCReputationView::fame() const
{
    return m_fame;
}

QHash<QString, int> NPCReputationView::reputation() const
{
    QHash<QString, int> tReputation;

    for ( QLabel *pPlace: m_places.keys() )
        tReputation.insert( pPlace->text(), m_places.value(pPlace)->text().toInt() );

    return tReputation;
}

void NPCReputationView::setPlaceReputation(const QString &place, const int &value)
{
    for ( QLabel *pPlace: m_places.keys() )
        if ( place == pPlace->text() ) {
            QLabel *pValue = m_places.value(pPlace);
            pValue->setNum( m_places.value(pPlace)->text().toInt() + value );
            emit reputationChanged( pValue->text().toInt() );
            break;
        }
}

void NPCReputationView::updateFame(const int &value)
{
    if ( 20 == value ) {
        ++m_fame;
        emit fameChanged( m_fame );
    }
}

NPCCustomWidget *NPCReputationView::createLayout()
{
    NPCCustomWidget *pWidget = new NPCCustomWidget();
    pWidget->setObjectName( "Widget" );
    pWidget->setStyleSheet( m_widgetStyle );
    QGridLayout *pLayout = new QGridLayout;
    QJsonArray places = DataLoader::loadJson( ":/data/json/Reputation.json" );

    int row{0}, column{0};
    for ( const QJsonValue place: places ) {
        QLabel *pPlace = new QLabel( place.toString(), this );
        QLabel *pValue = new QLabel( "0", this );
        m_places.insert( pPlace, pValue );
        pLayout->addWidget( pPlace, row, column );
        pLayout->addWidget( pValue, row, ++column );

        if (5 == column){
            column = 0;
            ++row;
        }
        else
            ++column;
    }

    pWidget->setLayout( pLayout );

    return pWidget;
}
