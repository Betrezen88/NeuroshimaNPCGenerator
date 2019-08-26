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
    connect( m_pThrowBtn, &QPushButton::clicked,
             this, &NPCAttributeManagerWidget::throwBtnClicked );
    connect( m_pDistributeType, &QComboBox::currentTextChanged,
             this, &NPCAttributeManagerWidget::onDistribiutionChanged);

    m_pRollCount->setRange( 1, 5 );
    m_pDistributeType->insertItems( 0, m_distributionTypes );

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
                QVector<DragDropAreaWidget*> values =
                        m_resultRows.at(m_radioBtn.indexOf(pRadioBtn))->results();
                DragDropAreaWidget *pMin = values.first();
                for ( QWidget *pValue: m_attributesValues ) {
                    DragDropAreaWidget *pAttribute = dynamic_cast<DragDropAreaWidget*>(pValue);
                    pAttribute->addLabel( QString::number(values.at(index)->value()) );
                    if ( pMin->value() > pAttribute->value() )
                        pMin = pAttribute;
                    ++index;
                }
                if ( m_pExtraDice->isChecked() ) {
                    const int value = values.at(index)->value();

                    if ( pMin->value() < value )
                        pMin->addLabel( QString::number(value) );
                }
                break;
            }
        }
    }
}

void NPCAttributeManagerWidget::onDistribiutionChanged(const QString &distribiution)
{
    if ( !m_attributesValues.isEmpty() )
        removeAttributeValues();
    if ( "Własny" == distribiution ) {
        m_pThrowBtn->setDisabled( true );
        m_pExtraDice->setDisabled( true );
        m_pExtraDice->setChecked( false );

        for ( int row{0}; row<m_attributesNames.size(); ++row ) {
            QSpinBox *pSpinBox = new QSpinBox( this );
            pSpinBox->setRange( 0, 20 );
            connect( pSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
                     [this, row, pSpinBox] () {
                emit attributeChanged(m_attributesNames.at(row)->text(), pSpinBox->value());
            } );
            m_attributesValues.push_back( pSpinBox );
            m_pAttributeLayout->addWidget( pSpinBox, row, 1 );
        }
    }
    else {
        m_pThrowBtn->setEnabled( true );
        m_pExtraDice->setEnabled( true );

        for ( int row{0}; row<m_attributesNames.size(); ++row ) {
            DragDropAreaWidget *pDrag = new DragDropAreaWidget( this );
            connect( pDrag, &DragDropAreaWidget::hasLabelChanged,
                     [this, row, pDrag](){
                emit attributeChanged(m_attributesNames.at(row)->text(), pDrag->value());
            } );
            m_attributesValues.push_back( pDrag );
            m_pAttributeLayout->addWidget( pDrag, row, 1 );
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

    m_pAttributeLayout = new QGridLayout;

    int row{0};
    for ( const QJsonValue tAttribute: *m_attributes ) {
        const QJsonObject &attribute = tAttribute.toObject();
        QLabel *pLabel = new QLabel(attribute.value("name").toString());
        m_attributesNames.push_back( pLabel );

        m_pAttributeLayout->addWidget( pLabel, row, 0 );

        ++row;
    }
    onDistribiutionChanged( m_pDistributeType->currentText() );

    pAttributeBox->setLayout( m_pAttributeLayout );

    return pAttributeBox;
}

void NPCAttributeManagerWidget::removeAttributeValues()
{
    for ( int i{0}; i<m_attributesNames.size(); ++i ) {
        m_pAttributeLayout->removeWidget( m_attributesValues.at(i) );
        m_attributesValues.at(i)->deleteLater();
        emit attributeChanged( m_attributesNames.at(i)->text(), 0 );
    }
    m_attributesValues.clear();
}
