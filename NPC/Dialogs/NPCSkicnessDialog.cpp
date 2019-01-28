#include "NPCSkicnessDialog.h"
#include "../Utils/DataLoader.h"
#include "../Utils/Dice.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QJsonArray>
#include <QRadioButton>

#include <QDebug>

NPCSkicnessDialog::NPCSkicnessDialog(QWidget *parent)
    : QDialog (parent),
      m_pRandomBtn(new QPushButton("Losuj", this)),
      m_pCancelBtn(new QPushButton("Anuluj", this)),
      m_pAcceptBtn(new QPushButton("Akceptuj", this)),
      m_pThrows(new QSpinBox(this)),
      m_pResultBox(new QGroupBox("Wylosowane Choroby", this))
{
    setWindowTitle( "Losuj Chorobę" );
    setAttribute( Qt::WA_DeleteOnClose );

    m_pThrows->setRange( 1, 3 );
    m_pAcceptBtn->setEnabled( false );

    connect( m_pRandomBtn, &QPushButton::clicked,
             this, &NPCSkicnessDialog::randomSicnkess );
    connect( m_pCancelBtn, &QPushButton::clicked,
             this, &NPCSkicnessDialog::close );
    connect( m_pAcceptBtn, &QPushButton::clicked,
             this, &NPCSkicnessDialog::onAccept );

    QGridLayout *pLayout = new QGridLayout;
    pLayout->addWidget( new QLabel("Ilość prób", this), 0, 0 );
    pLayout->addWidget( m_pResultBox, 1, 0, 1, 3 );
    pLayout->addWidget( m_pThrows, 0, 1 );
    pLayout->addWidget( m_pCancelBtn, 2, 0 );
    pLayout->addWidget( m_pRandomBtn, 2, 1 );
    pLayout->addWidget( m_pAcceptBtn, 2, 2 );

    setLayout( pLayout );
}

void NPCSkicnessDialog::onAccept()
{
    for ( int i=0; i<m_results.count(); ++i ) {
        if ( m_results.at(i)->isChecked() ) {
            emit acceptSickness( m_results.at(i)->property("json").toJsonObject() );
            break;
        }
    }
    close();
}

void NPCSkicnessDialog::randomSicnkess()
{
    Dice d{20};
    QJsonArray results, sickness = DataLoader::loadJson( ":/data/json/Sickness.json" );
    QVector<int> diceValues;

    while( m_pThrows->value() > diceValues.count() ) {
        int diceValue = static_cast<int>(d.throwValue());
        if ( !diceValues.contains(diceValue) ) {
            diceValues.push_back( diceValue );
            results.push_back( sickness.at(diceValue) );
        }
    }

    updateResults( results );
}

void NPCSkicnessDialog::updateResults(const QJsonArray &results)
{
    if ( nullptr != m_pResultBox )
        delete m_pResultBox;

    m_pResultBox = new QGroupBox("Wylosowane Choroby", this);

    QVBoxLayout *pLayout = new QVBoxLayout;
    for ( int i=0; i<results.count(); ++i ) {
        const QJsonObject &sickness = results.at(i).toObject();
        QRadioButton *pRadioBtn = new QRadioButton(sickness.value("name").toString(), m_pResultBox);
        pRadioBtn->setProperty( "json", sickness );
        pRadioBtn->setToolTip( createTooltip(sickness) );
        pRadioBtn->setToolTipDuration( 5 * 60 * 100 );
        pLayout->addWidget( pRadioBtn );
        m_results.push_back( pRadioBtn );

        connect( pRadioBtn, &QRadioButton::clicked,
                 m_pAcceptBtn, &QPushButton::setEnabled );
    }
    m_pResultBox->setLayout( pLayout );
    static_cast<QGridLayout*>(this->layout())->addWidget( m_pResultBox, 1, 0, 1, 3 );
}

QString NPCSkicnessDialog::createTooltip(const QJsonObject &object)
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
