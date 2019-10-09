#include "NPCFriendCard.h"
#include "NPCFriendObverse.h"

#include <QJsonArray>

NPCFriendCard::NPCFriendCard(const QJsonObject &pal, QWidget *parent)
    : QTabWidget( parent ),
      m_pObverse( new NPCFriendObverse(pal.value("personal").toObject(),
                                       pal.value("attributes").toArray(),
                                       pal.value("features").toArray(),
                                       this) ),
      m_pReverse( new QWidget ),
      m_pal( pal )
{
    setTabPosition( TabPosition::West );
    addTab( m_pObverse, "Statystyki" );
    addTab( m_pReverse, "Ekwipunek" );
}

const NPCFriendObverse& NPCFriendCard::obverse() const
{
    return *m_pObverse;
}
