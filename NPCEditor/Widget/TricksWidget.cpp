#include "TricksWidget.h"

#include <QVBoxLayout>

TricksWidget::TricksWidget(QWidget *parent)
    : CardWidget("Sztuczki", parent)
{
    m_pTricksList = new QListWidget( this );
    m_pAddBtn = new QPushButton( "Dodaj", this );
    m_pRemoveBtn = new QPushButton( "Usuń", this );

    QHBoxLayout *pButtonsLyaout = new QHBoxLayout;
    pButtonsLyaout->addWidget( m_pAddBtn );
    pButtonsLyaout->addWidget( m_pRemoveBtn );

    QVBoxLayout *pAll = new QVBoxLayout;
    pAll->addWidget( m_pTitle );
    pAll->addWidget( m_pTricksList );
    pAll->addLayout( pButtonsLyaout );

    setLayout( pAll );
}
