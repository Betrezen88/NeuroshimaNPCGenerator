#include "NPCAttributeManagerWidget.h"

#include <QGridLayout>
#include <QLabel>

NPCAttributeManagerWidget::NPCAttributeManagerWidget(QWidget *parent)
    : QWidget(parent),
      m_pRollCount(new QSpinBox(this)),
      m_pDistributeType(new QComboBox(this)),
      m_pExtraDice(new QCheckBox(this)),
      m_pThrowBtn(new QPushButton("Losuj", this))
{
    m_pRollCount->setRange( 1, 5 );
    m_pDistributeType->insertItems( 0, m_distributionTypes );

    QGridLayout *pLayout = new QGridLayout;
    pLayout->addWidget( optionsBox(), 0, 0, 1, 2 );
    pLayout->addWidget( attributesBox(), 1, 0 );
    pLayout->addWidget( resulsBox(), 1, 1 );

    setLayout( pLayout );
}

QGroupBox *NPCAttributeManagerWidget::optionsBox()
{
    QGroupBox *pOptionsBox = new QGroupBox( "Opcje rzutów", this );

    QGridLayout *pLayout = new QGridLayout;
    pLayout->addWidget( new QLabel("Ilość prób", this), 0, 0 );
    pLayout->addWidget( m_pRollCount, 0, 1 );
    pLayout->addWidget( new QLabel("Typ przydzielania", this), 1, 0 );
    pLayout->addWidget( m_pDistributeType, 1, 1 );
    pLayout->addWidget( new QLabel("Dodatkowa kość", this), 2, 0 );
    pLayout->addWidget( m_pExtraDice, 2, 1 );
    pLayout->addWidget( m_pThrowBtn, 3, 1 );
    pOptionsBox->setLayout( pLayout );

    return pOptionsBox;
}

QGroupBox *NPCAttributeManagerWidget::attributesBox()
{
    QGroupBox *pAttributeBox = new QGroupBox( "Atrybuty", this );

    QGridLayout *pLayout = new QGridLayout;

    int row{0};
    for ( const QString &attribute: m_attributes ) {
        QLabel *pLabel = new QLabel(attribute);
        DragDropAreaWidget *pDragDrop = new DragDropAreaWidget(this);
        m_results.insert( pLabel, pDragDrop );
        pLayout->addWidget( pLabel, row, 0 );
        pLayout->addWidget( pDragDrop, row, 1 );
        ++row;
    }

    pAttributeBox->setLayout( pLayout );

    return pAttributeBox;
}

QGroupBox *NPCAttributeManagerWidget::resulsBox()
{
    QGroupBox *pResultBox = new QGroupBox( "Wyniki", this );

    return pResultBox;
}
