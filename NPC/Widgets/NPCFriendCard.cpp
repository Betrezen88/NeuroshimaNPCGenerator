#include "NPCFriendCard.h"
#include "NPCFriendObverse.h"
#include "../NPCCardReverse.h"
#include "NPCAttributeView.h"
#include "NPCInventory.h"

#include <QJsonArray>

NPCFriendCard::NPCFriendCard(const QJsonObject &pal, QWidget *parent)
    : QTabWidget( parent ),
      m_pObverse( new NPCFriendObverse(pal.value("personal").toObject(),
                                       pal.value("attributes").toArray(),
                                       pal.value("features").toArray(),
                                       this) ),
      m_pReverse( new NPCCardReverse(m_pObverse->attribute("Budowa")->currentValue(), this) ),
      m_pal( pal )
{
    setTabPosition( TabPosition::West );
    addTab( m_pObverse, "Statystyki" );
    addTab( m_pReverse, "Ekwipunek" );

    for ( const QJsonValue item: pal.value("equipment").toArray() )
        m_pReverse->inventory()->addItem( item.toObject(), 1 );
}

NPCFriendObverse *NPCFriendCard::obverse() const
{
    return m_pObverse;
}

NPCCardReverse *NPCFriendCard::reverse() const
{
    return m_pReverse;
}
