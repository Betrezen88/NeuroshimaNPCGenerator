#include "NPCSicknessManagerWidget.h"
#include "../Utils/DataLoader.h"
#include "../Utils/Dice.h"

#include <QLabel>
#include <QJsonObject>
#include <QJsonValue>

NPCSicknessManagerWidget::NPCSicknessManagerWidget(QWidget *parent)
    : QWidget(parent),
      m_pThrows(new QSpinBox(this)),
      m_pThrow(new QPushButton("Losuj", this)),
      m_pResultBox(new QGroupBox("Wylosowane Choroby", this)),
      m_pLayout(new QGridLayout())
{
    connect( m_pThrow, &QPushButton::clicked,
             this, &NPCSicknessManagerWidget::throwClicked );

    m_pThrows->setRange( 1, 3 );

    m_pLayout->addWidget( new QLabel("Ilość rzutów", this), 0, 0 );
    m_pLayout->addWidget( m_pThrows, 0, 1 );
    m_pLayout->addWidget( m_pResultBox, 1, 0, 1, 2 );
    m_pLayout->addWidget( m_pThrow, 2, 1 );

    setLayout( m_pLayout );
}

void NPCSicknessManagerWidget::throwClicked()
{
    QJsonArray sicknesses = DataLoader::loadJson( ":/data/json/Sickness.json" );
    Dice d{20};
    QVector<int> diceValues;

    while ( m_pThrows->value() > diceValues.count() ) {
        int diceValue = static_cast<int>(d.throwValue());
        if ( !diceValues.contains(diceValue) ) {
            diceValues.push_back( diceValue );
            m_sickness.push_back( sicknesses.at(diceValue) );
        }
    }

    updateResults();
}

void NPCSicknessManagerWidget::updateResults()
{
    if ( nullptr != m_pResultBox ) {
        m_pLayout->removeWidget( m_pResultBox );
        delete m_pResultBox;
    }

    m_pResultBox = new QGroupBox("Wylosowane Choroby", this);

    QVBoxLayout *pLayout = new QVBoxLayout;
    for ( const QJsonValue tSickness: m_sickness ) {
        const QJsonObject &sickness = tSickness.toObject();
        QRadioButton *pRadioBtn = new QRadioButton(sickness.value("name").toString(), m_pResultBox);
        pRadioBtn->setToolTip( createTooltip(sickness) );
        pRadioBtn->setToolTipDuration( 5 * 60 * 100 );
        pLayout->addWidget( pRadioBtn );

        connect( pRadioBtn, &QRadioButton::toggled,
                 [this, sickness](bool checked){ if ( checked ) sicknessChanged(sickness); } );

        if ( tSickness == m_sickness.first() ) {
            pRadioBtn->setChecked( true );
            pRadioBtn->toggled( true );
        }
    }
    m_pResultBox->setLayout( pLayout );
    m_pLayout->addWidget( m_pResultBox, 1, 0, 1, 2 );
}

QString NPCSicknessManagerWidget::createTooltip(const QJsonObject &object)
{
    QString tooltip;
    const QJsonObject &symptoms = object.value("symptoms").toObject();

    tooltip = "<b>Nazwa:</b> " + object.value("name").toString()
            + "<br><br><b>Opis:</b> " + object.value("description").toString()
            + "<br><br><b>Lekarstwo:</b> " + object.value("cure").toString()
            + "<br><br><b>Symptomy</b>:"
            + "<ul>"
            + "<li><b>Pierwsze symptomy:</b> " + symptoms.value("1").toObject().value("description").toString()
            + "<br><b>Kary:</b>" + symptoms.value("1").toObject().value("penalties").toString() + "</li>"
            + "<li><b>Stan ostry:</b> " + symptoms.value("2").toObject().value("description").toString()
            + "<br><b>Kary:</b>" + symptoms.value("2").toObject().value("penalties").toString() + "</li>"
            + "<li><b>Stan krytyczny:</b> " + symptoms.value("3").toObject().value("description").toString()
            + "<br><b>Kary:</b>" + symptoms.value("3").toObject().value("penalties").toString() + "</li>"
            + "<li><b>Stan terminalny:</b> " + symptoms.value("4").toObject().value("description").toString()
            + "<br><b>Kary:</b>" + symptoms.value("4").toObject().value("penalties").toString() + "</li>"
            + "</ul>";

    return tooltip;
}
