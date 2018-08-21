#include "TricksWidget.h"

#include <QVBoxLayout>

TricksWidget::TricksWidget(QWidget *parent)
    : CardWidget("Sztuczki", parent)
{
    m_pTricksList = new QListWidget( this );

    QVBoxLayout *pAll = new QVBoxLayout;
    pAll->addWidget( m_pTitle );
    pAll->addWidget( m_pTricksList );

    setLayout( pAll );
}
