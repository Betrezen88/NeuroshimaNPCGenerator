#include "NPCAttributeResultsRow.h"
#include <QtMath>

NPCAttributeResultsRow::NPCAttributeResultsRow(const int &throws, QWidget *parent)
    : QWidget (parent),
      m_throws(throws),
      m_pLayout(new QHBoxLayout)
{
    setLayout( m_pLayout );
    createRow();
}

QVector<DragDropAreaWidget *> NPCAttributeResultsRow::results() const
{
    return m_results;
}

void NPCAttributeResultsRow::createRow()
{
    for ( int i{0}; i<m_throws; ++i ) {
        int tRoll = roll();
        DragDropAreaWidget *pDrag = new DragDropAreaWidget(this);
        pDrag->addLabel( QString::number((tRoll>=8)?tRoll:8) );
        m_results.push_back( pDrag );
        m_pLayout->addWidget( pDrag );
    }
}

int NPCAttributeResultsRow::roll()
{
    qreal result{0};
    for ( int i{0}; i<3; ++i )
        result += d20.throwValue();
    return qCeil(result/3);
}
