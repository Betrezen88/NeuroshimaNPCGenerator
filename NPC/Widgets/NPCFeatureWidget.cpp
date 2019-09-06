#include "NPCFeatureWidget.h"
#include "../Utils/DataLoader.h"

#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>

NPCFeatureWidget::NPCFeatureWidget(const QJsonObject &feature, QWidget *parent)
    : QWidget( parent ),
      m_feature( feature )
{

}

SkillSelector::SkillSelector(const QJsonObject &feature, QWidget *parent)
    : NPCFeatureWidget( feature, parent ),
      m_pSkill( new QComboBox )
{
    m_pSkill->addItems( skills() );

    QHBoxLayout *pSkillRow = new QHBoxLayout;
    pSkillRow->addWidget( new QLabel(m_feature.value("name").toString()) );
    pSkillRow->addWidget( m_pSkill );

    QVBoxLayout *pLayout = new QVBoxLayout;
    setLayout( pLayout );
    pLayout->addLayout( pSkillRow );
    pLayout->addSpacerItem( new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding) );
}

void SkillSelector::createFeature()
{
    QJsonObject feature;

    feature.insert( "name", "Umiejętność "+m_pSkill->currentText()+" na poziomie "
                    +QString::number(m_feature.value("value").toInt()) );
    feature.insert( "price", m_feature.value("price").toInt() );

    emit sendFeature( feature );
}

QStringList SkillSelector::skills() const
{
    QStringList data;

    QJsonArray array = DataLoader::loadJson( ":/data/json/Attributes.json" );

    for ( const QJsonValue attribute: array ) {
        const QJsonObject &tAttribute = attribute.toObject();
        for ( const QJsonValue skillPack: tAttribute.value("skillPacks").toArray() ) {
            const QJsonObject &tSkillPack = skillPack.toObject();
            for ( const QJsonValue skill: tSkillPack.value("skills").toArray() ) {
                data.push_back( skill.toString() );
            }
        }
    }

    return data;
}

SkillsSelector::SkillsSelector(const QJsonObject &feature, QWidget *parent)
    : NPCFeatureWidget (feature, parent),
      m_pSpec( new QComboBox )
{
    for ( int i{0}; i<=2; ++i ) {
        QComboBox *pSkill = new QComboBox(this);
        m_skills.push_back( pSkill );

        connect( pSkill, &QComboBox::currentTextChanged,
                 [this, pSkill](const QString &text){
            this->setUniqueSkills(pSkill, text);
        });
    }

    connect( m_pSpec, &QComboBox::currentTextChanged,
             this, &SkillsSelector::onSpecializationChanged );
    m_pSpec->addItems( specializations() );

    QGridLayout *pLayout = new QGridLayout;
    setLayout( pLayout );
    pLayout->addWidget( new QLabel("Specjalizacja:"), 0, 0 );
    pLayout->addWidget( m_pSpec, 0, 1 );
    pLayout->addWidget( new QLabel("Umiejętność na poziomie 4"), 1, 0 );
    pLayout->addWidget( m_skills.at(0), 1, 1 );
    pLayout->addWidget( new QLabel("Umiejętność na poziomie 3"), 2, 0 );
    pLayout->addWidget( m_skills.at(1), 2, 1 );
    pLayout->addWidget( new QLabel("Umiejętność na poziomie 2"), 3, 0 );
    pLayout->addWidget( m_skills.at(2), 3, 1 );
}

void SkillsSelector::createFeature()
{
    QJsonObject feature;

    feature.insert( "name", "Umiejętności specjalizacji "+m_pSpec->currentText()
                            +" ("+m_skills.at(0)->currentText()+": 4, "
                            +m_skills.at(1)->currentText()+": 3, "
                            +m_skills.at(2)->currentText()+": 2 )" );
    feature.insert( "price", m_feature.value("price").toInt() );

    emit sendFeature( feature );
}

void SkillsSelector::onSpecializationChanged(const QString &spec)
{
    QStringList skills = skillsBySpec( spec );

    for ( QComboBox *pSkill: m_skills ) {
        pSkill->clear();
        pSkill->addItems( skills );
    }
}

void SkillsSelector::setUniqueSkills(QComboBox *pSkill, const QString &text)
{
    for ( QComboBox *skill: m_skills ) {
        if ( skill != pSkill && skill->currentText() == text )
            skill->setCurrentIndex( skill->currentIndex()+1 );
    }
}

QStringList SkillsSelector::specializations() const
{
    return QStringList{ "Cwaniak", "Ranger", "Wojownik", "Technik" };
}

QStringList SkillsSelector::skillsBySpec(const QString &spec) const
{
    QStringList data;

    QJsonArray array = DataLoader::loadJson( ":/data/json/Attributes.json" );

    for ( const QJsonValue attribute: array ) {
        const QJsonObject &tAttribute = attribute.toObject();
        for ( const QJsonValue skillPack: tAttribute.value("skillPacks").toArray() ) {
            const QJsonObject &tSkillPack = skillPack.toObject();
            if ( tSkillPack.value("Specialization").toArray().contains(spec) ) {
                for ( const QJsonValue skill: tSkillPack.value("skills").toArray() ) {
                    data.push_back( skill.toString() );
                }
            }
        }
    }

    return data;
}

AttributeSelector::AttributeSelector(const QJsonObject &feature, QWidget *parent)
    : NPCFeatureWidget (feature, parent),
      m_pAttribute( new QComboBox )
{
    m_pAttribute->addItems( attributes() );

    QHBoxLayout *pAttributRow = new QHBoxLayout;
    pAttributRow->addWidget( new QLabel("Współczynnik na poziomie "
                            +QString::number(m_feature.value("value").toInt())+":") );
    pAttributRow->addWidget( m_pAttribute );

    QVBoxLayout *pLayout = new QVBoxLayout;
    setLayout( pLayout );
    pLayout->addLayout( pAttributRow );
    pLayout->addSpacerItem( new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding) );
}

void AttributeSelector::createFeature()
{
    QJsonObject feature;

    feature.insert( "name", "Współczynnik "+m_pAttribute->currentText()
                    +" na poziomie "+QString::number(m_feature.value("value").toInt()) );
    feature.insert( "price", m_feature.value("price").toInt() );

    emit sendFeature( feature );
}

QStringList AttributeSelector::attributes() const
{
    QStringList data;

    QJsonArray array = DataLoader::loadJson( ":/data/json/Attributes.json" );

    for ( const QJsonValue attribute: array ) {
        const QJsonObject &tAttribute = attribute.toObject();
        data.push_back( tAttribute.value("name").toString() );
    }

    return data;
}

ReputationSelector::ReputationSelector(const QJsonObject &feature, QWidget *parent)
    : NPCFeatureWidget (feature, parent),
      m_pPlace( new QComboBox )
{
    m_pPlace->addItems( places() );

    QVBoxLayout *pLayout = new QVBoxLayout;
    setLayout( pLayout );
    pLayout->addWidget( new QLabel(m_feature.value("name").toString()) );
    pLayout->addWidget( m_pPlace );
    pLayout->addSpacerItem( new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding) );
}

void ReputationSelector::createFeature()
{
    QJsonObject feature;

    feature.insert( "name", m_feature.value("name").toString()+" "+m_pPlace->currentText() );
    feature.insert( "price", m_feature.value("price").toInt() );

    emit sendFeature( feature );
}

QStringList ReputationSelector::places() const
{
    QStringList data;

    QJsonArray array = DataLoader::loadJson( ":/data/json/Reputation.json" );

    for ( const QJsonValue place: array )
        data << place.toString();

    return data;
}

DebtSelector::DebtSelector(const int &profit, const QJsonObject &feature, QWidget *parent)
    : NPCFeatureWidget (feature, parent),
      m_pDebt( new QComboBox ),
      m_pProfit( new QLabel("0") )
{
    connect( m_pDebt, QOverload<int>::of(&QComboBox::currentIndexChanged),
             this, &DebtSelector::setProfitValue );

    fillDebtBox( profit );

    QHBoxLayout *pDebtRow = new QHBoxLayout;
    pDebtRow->addWidget( new QLabel("Wartość długu:") );
    pDebtRow->addWidget( m_pDebt );

    QHBoxLayout *pProfitRow = new QHBoxLayout;
    pProfitRow->addWidget( new QLabel("Zysk:") );
    pProfitRow->addWidget( m_pProfit );

    QVBoxLayout *pLayout = new QVBoxLayout;
    setLayout( pLayout );
    pLayout->addLayout( pDebtRow );
    pLayout->addLayout( pProfitRow );
    pLayout->addSpacerItem( new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding) );
}

void DebtSelector::createFeature()
{
    QJsonObject feature;

    feature.insert( "name", m_feature.value("name").toString()+ " "
                    + m_pDebt->currentText() + " gambli." );
    feature.insert( "price", m_pProfit->text().toInt() );

    emit sendFeature( feature );
}

void DebtSelector::setProfitValue(const int &index)
{
    int profit = m_feature.value("price").toInt() * (index+1);
    m_pProfit->setNum( profit );
}

void DebtSelector::fillDebtBox(const int &profit)
{
    int debt = m_feature.value("value").toInt();
    for ( int i{0}; i<6; ++i ) {
        if ( (profit-((i+1)*5)) >= -30 )
            m_pDebt->addItem( QString::number(debt*(i+1)) );
        else
            break;
    }
}
