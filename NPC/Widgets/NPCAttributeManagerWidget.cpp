#include "NPCAttributeManagerWidget.h"

#include <QGridLayout>
#include <QJsonObject>

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

void NPCAttributeManagerWidget::throwBtnClicked()
{
    if ( nullptr != m_pResultBox ) {
        m_pMainLayout->removeWidget( m_pResultBox );
        delete m_pResultBox;
    }

    int throws = (m_pExtraDice->isChecked()) ? 6 : 5;
    m_pResultBox = new QGroupBox( "Wyniki", this );
    QVBoxLayout *pLayout = new QVBoxLayout;
    for ( int i{0}; i<m_pRollCount->value(); ++i ) {
        QHBoxLayout *pRowLayout = new QHBoxLayout;
        QRadioButton *pRadioBtn = new QRadioButton(m_pResultBox);

        NPCAttributeResultsRow *pRow = new NPCAttributeResultsRow( throws, this );

        connect( pRadioBtn, &QRadioButton::toggled,
                 pRow, &QWidget::setEnabled );
        connect( pRadioBtn, &QRadioButton::toggled,
                 this, &NPCAttributeManagerWidget::distributeResults );

        pRadioBtn->toggled( i == 0 );
        pRadioBtn->setChecked( i == 0 );

        pRowLayout->addWidget( pRadioBtn );
        pRowLayout->addWidget( pRow );
        pLayout->addLayout( pRowLayout );
//        m_rows.insert( pRadioBtn, pRow );
        m_radioBtn.push_back( pRadioBtn );
        m_resultRows.push_back( pRow );
    }
    m_pResultBox->setLayout( pLayout );
    m_pMainLayout->addWidget( m_pResultBox, 1, 1 );
}

void NPCAttributeManagerWidget::distributeResults()
{
    if ( m_distributionTypes[1] == m_pDistributeType->currentText() ) {
        for ( QRadioButton *pRadioBtn: m_radioBtn ) {
            if ( pRadioBtn->isChecked() ) {
                int index{0};
                for ( DragDropAreaWidget *pDrag:
                      m_resultRows.at(m_radioBtn.indexOf(pRadioBtn))->results() ) {
                    m_attributesValues.at(index)->addLabel( QString::number(pDrag->value()) );
                    ++index;
                }
                break;
            }
        }
    }
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
        m_attributesNames.push_back( pLabel );
        m_attributesValues.push_back( pDragDrop );

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
