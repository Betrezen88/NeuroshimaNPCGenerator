#include "NPCCard.h"

#include <QCheckBox>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include <QDebug>

NPCCard::NPCCard(QWidget *parent)
    : QWidget(parent),
      m_pPortrait(new QLabel(this)),
      m_pName(new QLineEdit(this)),
      m_pOrigin(new QComboBox(this)),
      m_pProfession(new QComboBox(this)),
      m_pSpecialization(new QComboBox(this)),
      m_pSickness(new QComboBox(this)),
      m_pFeature1(new QComboBox(this)),
      m_pFeature2(new QComboBox(this)),
      m_pReputation(new QSpinBox(this)),
      m_pFame(new QSpinBox(this)),
      m_pTricks(new QListWidget(this)),
      m_pSpendSkillPoints(new QLabel())
{
    initCardData();

    QHBoxLayout *pAll = new QHBoxLayout;
    setLayout( pAll );

    pAll->addLayout( column1() );
    pAll->addLayout( column2() );
    pAll->addLayout( column3() );
    pAll->addLayout( column4() );

}

void NPCCard::onSkillPackBougth(const bool &checked, const QStringList &specs)
{
    int value = (checked) ? 5 : -5;
    m_spendedSkillPoints += value;
    updateSpendedSkillPoints();
}

void NPCCard::onSkillValueChanged(const int &value, const QStringList &specs)
{
    qDebug() << "Skill changed: " << value << specs;
}

void NPCCard::initCardData()
{
    loadJsonObject( m_attributesJson, ":/Attributes.json" );
    loadJsonObject( m_specializationsJson, ":/Specializations.json" );

    QStringList tSpecs;
    for ( const QJsonValue &spec: m_specializationsJson )
        tSpecs << spec.toString();

    m_pSpecialization->insertItems( 0, tSpecs );

    createAndFillAttributes();
}

QWidget *NPCCard::createPersonalSection()
{
    QWidget *pWidget = new QWidget(this);
    QVBoxLayout *pLayout = new QVBoxLayout;

    QVBoxLayout *pTitleL = new QVBoxLayout;
    pTitleL->addWidget( m_pPortrait );
    pTitleL->addWidget( createLabel("Postać", "Title", m_titleStyle, 0, 40) );

    QHBoxLayout *pNameL = new QHBoxLayout;
    pNameL->addWidget( new QLabel("Imię", pWidget) );
    pNameL->addWidget( m_pName );

    QVBoxLayout *pOriginL = new QVBoxLayout;
    pOriginL->addWidget( new QLabel("Pochodzenie", pWidget) );
    pOriginL->addWidget( m_pOrigin );

    QVBoxLayout *pProfessionL = new QVBoxLayout;
    pProfessionL->addWidget( new QLabel("Profesja", pWidget) );
    pProfessionL->addWidget( m_pProfession );

    QVBoxLayout *pSpecializationL = new QVBoxLayout;
    pSpecializationL->addWidget( new QLabel("Specjalizacja", pWidget) );
    pSpecializationL->addWidget( m_pSpecialization );

    QVBoxLayout *pSicknessL = new QVBoxLayout;
    pSicknessL->addWidget( new QLabel("Choroba", pWidget) );
    pSicknessL->addWidget( m_pSickness );

    QVBoxLayout *pFeature1L = new QVBoxLayout;
    pFeature1L->addWidget( new QLabel("Cecha z pochodzenia", pWidget) );
    pFeature1L->addWidget( m_pFeature1 );

    QVBoxLayout *pFeatrue2L = new QVBoxLayout;
    pFeatrue2L->addWidget( new QLabel("Cecha z profesji", pWidget) );
    pFeatrue2L->addWidget( m_pFeature2 );

    QHBoxLayout *pReputationL = new QHBoxLayout;
    pReputationL->addWidget( new QLabel("Reputacja", pWidget) );
    pReputationL->addWidget( m_pReputation );

    QHBoxLayout *pFameL = new QHBoxLayout;
    pFameL->addWidget( new QLabel("Sława", pWidget) );
    pFameL->addWidget( m_pFame );

    pLayout->addLayout( pTitleL );
    pLayout->addLayout( pNameL );
    pLayout->addLayout( pOriginL );
    pLayout->addLayout( pProfessionL );
    pLayout->addLayout( pSpecializationL );
    pLayout->addLayout( pSicknessL );
    pLayout->addLayout( pFeature1L );
    pLayout->addLayout( pFeatrue2L );
    pLayout->addLayout( pReputationL );
    pLayout->addLayout( pFameL );
    pLayout->setSpacing( 2 );
    pLayout->setMargin( 0 );

    pWidget->setLayout( pLayout );
    return pWidget;
}

QWidget *NPCCard::createTricksSection()
{
    QWidget *pWidget = new QWidget(this);
    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addWidget( createLabel("Sztuczki", "Title", m_titleStyle, 0, 40) );
    pLayout->addWidget( m_pTricks );
    pLayout->setSpacing( 1 );
    pLayout->setMargin( 0 );

    pWidget->setLayout( pLayout );
    return pWidget;
}

QWidget *NPCCard::createModificatorSection()
{
    QWidget *pWidget = new QWidget(this);
    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addWidget( createLabel("Modyfikatory do cech z poziomów trudności",
                                    "ModTitle",
                                    m_modTitle,
                                    0, 25) );
    pLayout->addWidget( createModificatorWidget() );
    pLayout->setSpacing( 0 );
    pLayout->setMargin( 0 );
    pWidget->setLayout( pLayout );
    return pWidget;
}

QWidget *NPCCard::createModificatorWidget()
{
    QWidget *pWidget = new QWidget();
    pWidget->setObjectName( "CellWidget" );
    pWidget->setStyleSheet( m_cellWidget );
    QHBoxLayout *pLayout = new QHBoxLayout;

    for ( int i=0; i<m_shortModsList.size(); ++i ) {
        QVBoxLayout *pCellL = new QVBoxLayout;
        pCellL->addWidget( createLabel(m_shortModsList.at(i),
                                       "CellTitle",
                                       m_cellTitle) );
        pCellL->addWidget( createLabel(QString::number(m_modsVals.at(i)),
                                       "CellValue",
                                       m_cellValue) );
        pLayout->addLayout( pCellL );
    }

    pLayout->setSpacing( 0 );
    pWidget->setLayout( pLayout );
    return pWidget;
}

QWidget *NPCCard::createWoundsSection()
{
    QWidget *pWidget = new QWidget(this);
    pWidget->setObjectName( "WoundsWidget" );
    pWidget->setStyleSheet( m_woundsWidget );
    QVBoxLayout *pLayout = new QVBoxLayout;

    for ( const QString &tName: m_woundsList ) {
        QHBoxLayout *pRowL = new QHBoxLayout;
        pRowL->addWidget( new QLabel(tName, pWidget) );
        pRowL->addWidget( new QSpinBox(pWidget) );
        pLayout->addLayout( pRowL );
    }
    pLayout->setSpacing( 1 );
    pWidget->setLayout( pLayout );
    return pWidget;
}

QWidget *NPCCard::createWoundsModificatorsSection()
{
    QTextEdit *pTextEdit = new QTextEdit(this);
    pTextEdit->setObjectName( "WoundsModificator" );
    pTextEdit->setStyleSheet( m_woundsModificatorsWidget );
    pTextEdit->setFixedHeight( 80 );
    pTextEdit->setWordWrapMode( QTextOption::WordWrap );
    return pTextEdit;
}

QWidget *NPCCard::createProgressSection()
{
    QWidget *pWidget = new QWidget(this);
    pWidget->setObjectName( "ProgressWidget" );
    pWidget->setStyleSheet( m_progressWidget );
    QGridLayout *pLayout = new QGridLayout;

    updateSpendedSkillPoints();

    pLayout->addWidget( new QLabel("Atrybuty", this), 0, 0 );
    pLayout->addWidget( new QSpinBox(this), 0, 1 );
    pLayout->addWidget( new QLabel("Umiejętności", this), 1, 0 );
    pLayout->addWidget( m_pSpendSkillPoints, 1, 1 );
    pLayout->addWidget( new QLabel("Doświadczenie", this), 2, 0 );
    pLayout->addWidget( new QSpinBox(this), 2, 1 );

    pWidget->setLayout( pLayout );
    return pWidget;
}

void NPCCard::createAndFillAttributes()
{
    for ( const QJsonValue &attr: m_attributesJson ) {
        const QJsonObject &attribute = attr.toObject();
        const QString &name = attribute.value("name").toString();
        const QJsonArray &skillPacks = attribute.value("skillPacks").toArray();
        NPCAttributeWidget *pAttribute = new NPCAttributeWidget(name, skillPacks, m_mods, this);
        connect( pAttribute, &NPCAttributeWidget::skillPackBougth, this, &NPCCard::onSkillPackBougth );
        m_attributes.insert( name, pAttribute );
    }
}

QLabel *NPCCard::createLabel(const QString &text,
                             const QString &objName,
                             const QString &style,
                             const int &width,
                             const int &heigth,
                             Qt::Alignment aligment)
{
    QLabel *pLabel = new QLabel(text);
    pLabel->setAlignment( aligment );
    if ( "" != objName ) pLabel->setObjectName( objName );
    if ( "" != style ) pLabel->setStyleSheet( style );
    if ( 0 != width ) pLabel->setFixedWidth( width );
    if ( 0 != heigth ) pLabel->setFixedHeight( heigth );
    return pLabel;
}

void NPCCard::updateSpendedSkillPoints()
{
    m_pSpendSkillPoints->setText( QString::number(m_skillPoints
                                                  +m_specSkillPoints
                                                  -m_spendedSkillPoints) );
}

QVBoxLayout *NPCCard::column1()
{
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( createPersonalSection() );
    pLayout->addWidget( createTricksSection() );
    pLayout->setMargin( 0 );
    pLayout->setSpacing( 1 );
    return pLayout;
}

QVBoxLayout *NPCCard::column2()
{
    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addWidget( createLabel("Współczynniki i umiejętności", "Title", m_titleStyle, 0, 40) );
    pLayout->addWidget( m_attributes.value("Budowa") );
    pLayout->addWidget( m_attributes.value("Zręczność") );
    pLayout->addWidget( createModificatorSection() );

    pLayout->setMargin( 0 );
    pLayout->setSpacing( 1 );
    return pLayout;
}

QVBoxLayout *NPCCard::column3()
{
    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addWidget( m_attributes.value("Charakter") );
    pLayout->addWidget( m_attributes.value("Percepcja") );

    pLayout->setMargin( 0 );
    pLayout->setSpacing( 1 );
    return pLayout;
}

QVBoxLayout *NPCCard::column4()
{
    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addWidget( m_attributes.value("Spryt") );
    pLayout->addWidget( createLabel("Rany", "Title", m_titleStyle, 0, 40) );
    pLayout->addWidget( createWoundsSection() );
    pLayout->addWidget( createLabel("Modyfikatory", "Title", m_titleStyle, 0, 40) );
    pLayout->addWidget( createWoundsModificatorsSection() );
    pLayout->addWidget( createLabel("Punkty Rozwoju", "Title", m_titleStyle, 0, 40) );
    pLayout->addWidget( createProgressSection() );

    pLayout->setMargin( 0 );
    pLayout->setSpacing( 1 );
    return pLayout;
}

void NPCCard::loadJsonObject(QJsonArray &array, const QString &fileName)
{
    QFile file( fileName );
    file.open( QIODevice::ReadOnly );

    if ( file.isOpen() ) {
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson( file.readAll(), &error );
        if ( doc.isEmpty() )
            qDebug() << "Parsing error in file " << fileName << ": " << error.errorString();
        array = doc.array();
    }
    else {
        qDebug() << "File " << fileName << " not open !";
    }
}
