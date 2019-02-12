#include "NPCSpecializationManagerWidget.h"
#include "../Utils/DataLoader.h"

#include <QGridLayout>
#include <QJsonValue>
#include <QJsonObject>

NPCSpecializationManagerWidget::NPCSpecializationManagerWidget(QWidget *parent)
    : QWidget(parent),
      m_pSpecs(new QComboBox(this)),
      m_pDescriptionLabel(new QLabel(this))
{
    m_specializations = DataLoader::loadJson( ":/data/json/Specializations.json" );
    m_pDescriptionLabel->setWordWrap( true );

    connect( m_pSpecs, &QComboBox::currentTextChanged,
             this, &NPCSpecializationManagerWidget::setSpec );

    m_pSpecs->insertItems( 0, specs() );

    QGridLayout *pLayout = new QGridLayout;
    pLayout->addWidget( new QLabel("Specjalizacja", this), 0, 0 );
    pLayout->addWidget( m_pSpecs, 0, 1 );
    pLayout->addWidget( descriptionBox(), 1, 0, 1, 2 );

    setLayout( pLayout );
}

void NPCSpecializationManagerWidget::setSpec(const QString &specName)
{
    for ( const QJsonValue tSpec: m_specializations ) {
        const QJsonObject &spec = tSpec.toObject();
        if ( specName == spec.value("name").toString() ) {
            m_pDescriptionLabel->setText( spec.value("description").toString() );
            break;
        }
    }
}

QStringList NPCSpecializationManagerWidget::specs()
{
    QStringList specsList;

    for ( const QJsonValue tSpec: m_specializations )
        specsList << tSpec.toObject().value("name").toString();

    return specsList;
}

QGroupBox *NPCSpecializationManagerWidget::descriptionBox()
{
    QGroupBox *pDescriptionBox = new QGroupBox( "Opis", this );
    QLabel *pLabel = new QLabel( pDescriptionBox );
    pLabel->setText( "*Na zakupienie Umiejętności związanych z dana specjalizacją masz dodatkowe"
                     " 35 punktów. Każdy pakiet umiejętności podsiada informację do której"
                     " specjalizacji należą. Dodatkowo rozwój umiejętności za punkty doświadczenia"
                     " jest o 20% tańszy." );
    pLabel->setWordWrap( true );

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( m_pDescriptionLabel );
    pLayout->addWidget( pLabel );
    pDescriptionBox->setLayout( pLayout );
    return pDescriptionBox;
}
