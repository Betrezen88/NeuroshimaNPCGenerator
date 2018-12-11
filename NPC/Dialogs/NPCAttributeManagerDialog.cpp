#include "NPCAttributeManagerDialog.h"

#include <QGridLayout>
#include <QtMath>
#include <QRadioButton>

#include <QDebug>

NPCAttributeManagerDialog::NPCAttributeManagerDialog(QWidget *parent)
    : QDialog(parent),
      m_pRollBtn(new QPushButton("Losuj")),
      m_pAcceptBtn(new QPushButton("Akceptuj")),
      m_pCancelBtn(new QPushButton("Anuluj")),
      m_pRollCount(new QSpinBox()),
      m_pRollType(new QComboBox()),
      m_pResultGroupBox( createResultGroupBox() )
{
    setWindowTitle( "Zarządzaj atrybutami" );
    setMinimumSize( 350, 310 );
    createResultHash();

    m_pRollCount->setRange( 1, 5 );
    m_pRollType->insertItems( 0, m_rollTypes );
    m_pAcceptBtn->setEnabled( acceptBtnActive() );

    connect( m_pRollBtn, &QPushButton::clicked,
             this, &NPCAttributeManagerDialog::addResultRow );
    connect( m_pCancelBtn, &QPushButton::clicked,
             this, &NPCAttributeManagerDialog::hideDialog );
    connect( m_pAcceptBtn, &QPushButton::clicked,
             this, &NPCAttributeManagerDialog::acceptBtnPressed );

    QHBoxLayout *pButtonsL = new QHBoxLayout;
    pButtonsL->addWidget( m_pRollBtn );
    pButtonsL->addWidget( m_pAcceptBtn );
    pButtonsL->addWidget( m_pCancelBtn );

    m_pGroupBoxL = new QHBoxLayout;
    m_pGroupBoxL->addWidget( createAttributeGroupBox() );
    m_pGroupBoxL->addWidget( m_pResultGroupBox );

    QVBoxLayout *pAll = new QVBoxLayout;
    pAll->addWidget( createRollOptionsGroupBox() );
    pAll->addLayout( m_pGroupBoxL );
    pAll->addLayout( pButtonsL );

    setLayout( pAll );
}

void NPCAttributeManagerDialog::acceptBtnPressed()
{
    QVector<int> results = QVector<int>();

    for ( const DragDropAreaWidget *pDrag: m_results.values() )
        results.push_back( pDrag->value() );

    emit acceptAttributes( results );
    hideDialog();
}

void NPCAttributeManagerDialog::addResultRow()
{
    m_pGroupBoxL->removeWidget( m_pResultGroupBox );
    delete m_pResultGroupBox;
    m_pResultGroupBox = createResultGroupBox();
    m_pGroupBoxL->addWidget( m_pResultGroupBox );

    for ( int i{0}; i<m_pRollCount->value(); ++i ) {
        QHBoxLayout *pLayout = new QHBoxLayout;

        QRadioButton *pRadioButton = new QRadioButton();
        QWidget *pWidget = createResultRowWidget();

        connect( pRadioButton, &QRadioButton::toggled,
                 pWidget, &QWidget::setEnabled );

        pLayout->addWidget( pRadioButton );
        pLayout->addWidget( pWidget );
        pRadioButton->toggled( i == 0 );
        pRadioButton->setChecked( i == 0 );

        qobject_cast<QVBoxLayout*>(m_pResultGroupBox->layout())->addLayout( pLayout );
    }
}

void NPCAttributeManagerDialog::onResultChanged()
{
    m_pAcceptBtn->setEnabled( acceptBtnActive() );
}

void NPCAttributeManagerDialog::hideDialog()
{
    emit hidden();
    hide();
}

QWidget *NPCAttributeManagerDialog::createResultRowWidget()
{
    QWidget *pWidget = new QWidget;
    QHBoxLayout *pLayout = new QHBoxLayout;
    pWidget->setLayout( pLayout );

    for ( int i{0}; i<5; ++i ) {
        DragDropAreaWidget *pDragDrop = new DragDropAreaWidget();
        pDragDrop->addLabel( QString::number(rollForAttribute()) );
        pLayout->addWidget( pDragDrop );
    }

    return pWidget;
}

QGroupBox *NPCAttributeManagerDialog::createAttributeGroupBox()
{
    QGroupBox *pGroupBox = new QGroupBox( "Atrybuty", this );
    QGridLayout *pLayout = new QGridLayout;

    int row{0};
    for ( const QString &label: m_attributes ) {
        pLayout->addWidget( new QLabel(label), row, 0 );
        pLayout->addWidget( m_results.value(label), row, 1 );
        ++row;
    }

    pGroupBox->setLayout( pLayout );
    pGroupBox->setFixedWidth( 110 );
    return pGroupBox;
}

QGroupBox *NPCAttributeManagerDialog::createRollOptionsGroupBox()
{
    QGroupBox *pGroupBox = new QGroupBox( "Opcje Rzutów", this );

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget( new QLabel("Ilość") );
    pLayout->addWidget( m_pRollCount );
    pLayout->addWidget( new QLabel("Typ") );
    pLayout->addWidget( m_pRollType );
    pLayout->setMargin( 2 );
    pLayout->setSpacing( 1 );
    pGroupBox->setLayout( pLayout );
    pGroupBox->setFixedHeight( 50 );
    return pGroupBox;
}

QGroupBox *NPCAttributeManagerDialog::createResultGroupBox()
{
    QGroupBox *pGroupBox = new QGroupBox( "Wyniki", this );
    QVBoxLayout *pLayout = new QVBoxLayout;
    pGroupBox->setLayout( pLayout );
    pGroupBox->setMinimumWidth( 180 );
    return pGroupBox;
}

void NPCAttributeManagerDialog::createResultHash()
{
    for ( const QString &label: m_attributes ) {
        DragDropAreaWidget *pDrag = new DragDropAreaWidget( this );
        connect( pDrag, &DragDropAreaWidget::hasLabelChanged,
                 this, &NPCAttributeManagerDialog::onResultChanged );
        m_results.insert( label, pDrag );
    }
}

int NPCAttributeManagerDialog::rollForAttribute()
{
    int sum{0};
    for ( int i{0}; i<3; ++i )
        sum += d20.throwValue();
    sum = qCeil( sum / 3 );
    return (sum >= 8) ? sum : 8;
}

bool NPCAttributeManagerDialog::acceptBtnActive()
{
    for ( const DragDropAreaWidget *pDrag: m_results )
        if ( !pDrag->hasLabel() )
            return false;
    return true;
}
