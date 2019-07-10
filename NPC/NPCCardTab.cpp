#include "NPCCardTab.h"

NPCCardTab::NPCCardTab(QWidget *parent)
    : QTabWidget(parent),
      m_pObverse(new NPCCardObverse(this)),
      m_pReverse(new NPCCardReverse(this))
{
    setTabPosition( TabPosition::West );
    addTab( m_pObverse, "Awers" );
    addTab( m_pReverse, "Rewers" );
}

NPCCardObverse *NPCCardTab::obverse() const
{
    return m_pObverse;
}

NPCCardReverse *NPCCardTab::reverse() const
{
    return m_pReverse;
}
