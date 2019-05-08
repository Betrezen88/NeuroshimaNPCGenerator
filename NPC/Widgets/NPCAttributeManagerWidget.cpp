#include "NPCAttributeManagerWidget.h"

#include <QGridLayout>
#include <QJsonObject>
#include <QLabel>
#include <QRadioButton>
#include <QtMath>

NPCAttributeManagerWidget::NPCAttributeManagerWidget(const QJsonArray *attributes, QWidget *parent)
    : QWidget(parent),
      m_attributes(attributes),
      m_pRollCount(new QSpinBox(this)),
      m_pDistributeType(new QComboBox(this)),
      m_pExtraDice(new QCheckBox(this)),
      m_pThrowBtn(new QPushButton("Losuj", this)),
      m_pResultBox(new QGroupBox("Wyniki", this))
{
    m_pRollCount->setRange( 1, 5 );
    m_pDistributeType->insertItems( 0, m_distributionTypes );

    connect( m_pThrowBtn, &QPushButton::clicked,
             this, &NPCAttributeManagerWidget::throwBtnClicked );

    m_pMainLayout = new QGridLayout;
    m_pMainLayout->addWidget( optionsBox(), 0, 0, 1, 2 );
    m_pMainLayout->addWidget( attributesBox(), 1, 0 );
    m_pMainLayout->addWidget( m_pResultBox, 1, 1 );

    setLayout( m_pMainLayout );
}

QWidget * NPCAttributeManagerWidget::createResultRowWidget()
{
    int throws = (m_pExtraDice->isChecked()) ? 6 : 5;
    QWidget *pWidget = new QWidget;

    QHBoxLayout *pResultL = new QHBoxLayout;
    for ( int j{0}; j<throws; ++j ) {
        DragDropAreaWidget *pDrag = new DragDropAreaWidget();
        int tRoll = roll();
        pDrag->addLabel( QString::number((tRoll>=8)?tRoll:8));
        pResultL->addWidget( pDrag );
    }
    pWidget->setLayout( pResultL );

    return pWidget;
}

void NPCAttributeManagerWidget::throwBtnClicked()
{
    if ( nullptr != m_pResultBox ) {
        m_pMainLayout->removeWidget( m_pResultBox );
        delete m_pResultBox;
    }

    m_pResultBox = new QGroupBox( "Wyniki", this );
    QVBoxLayout *pLayout = new QVBoxLayout;
    for ( int i{0}; i<m_pRollCount->value(); ++i ) {
        QHBoxLayout *pRowLayout = new QHBoxLayout;
        QRadioButton *pRadioBtn = new QRadioButton(m_pResultBox);
        QWidget *pWidget = createResultRowWidget();

        connect( pRadioBtn, &QRadioButton::toggled,
                 pWidget, &QWidget::setEnabled );

        pRadioBtn->toggled( i == 0 );
        pRadioBtn->setChecked( i == 0 );

        pRowLayout->addWidget( pRadioBtn );
        pRowLayout->addWidget( pWidget );
        pLayout->addLayout( pRowLayout );
    }
    m_pResultBox->setLayout( pLayout );
    m_pMainLayout->addWidget( m_pResultBox, 1, 1 );
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
    for ( const QJsonValue tAttribute: *m_attributes ) {
        const QJsonObject &attribute = tAttribute.toObject();
        QLabel *pLabel = new QLabel(attribute.value("name").toString());
        DragDropAreaWidget *pDragDrop = new DragDropAreaWidget(this);
        m_results.insert( pLabel, pDragDrop );
        pLayout->addWidget( pLabel, row, 0 );
        pLayout->addWidget( pDragDrop, row, 1 );

        connect( pDragDrop, &DragDropAreaWidget::hasLabelChanged,
                 [this, pLabel, pDragDrop](){
            emit attributeChanged(pLabel->text(), pDragDrop->value());
        } );

        ++row;
    }

    pAttributeBox->setLayout( pLayout );

    return pAttributeBox;
}

int NPCAttributeManagerWidget::roll()
{
    qreal result{0};
    for ( int i{0}; i<3; ++i ) {
        result += d20.throwValue();
    }
    return qCeil(result/3);
}
