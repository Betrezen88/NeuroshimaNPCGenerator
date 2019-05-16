#include "NPCSkillsManagerWidget.h"
#include "../Utils/DataLoader.h"

#include "NPCSkillPack.h"
#include "NPCCustomSkillPack.h"

#include <QJsonValue>
#include <QJsonObject>
#include <QVBoxLayout>

#include <QDebug>

NPCSkillsManagerWidget::NPCSkillsManagerWidget(const QJsonArray *attributes, QWidget *parent)
    : QWidget(parent),
      m_pSkillPointsLabel(new QLabel(this)),
      m_pSpecPointsLabel(new QLabel(this)),
      m_pOtherSkills(new NPCOtherSkills(this))
{
    setAttributes( *attributes );
    updateInfoLabels();

    connect( m_pOtherSkills, &NPCOtherSkills::skillValueChanged,
             this, &NPCSkillsManagerWidget::buySkill );
    connect( this, &NPCSkillsManagerWidget::availableSkillpointsValueChanged,
             [this](const int &skill, const int &specs, const QString &spec){
                Q_UNUSED(specs); Q_UNUSED(spec);
                this->m_pOtherSkills->onAvailableSkillpointsChanged(skill);
    } );

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addLayout( columnA() );
    pLayout->addLayout( columnB() );
    pLayout->addLayout( columnC() );

    setLayout( pLayout );
}

const QHash<const QString, NPCAttributeWidget *> *NPCSkillsManagerWidget::attributes() const
{
    return &m_attributes;
}

NPCOtherSkills *NPCSkillsManagerWidget::otherSkills() const
{
    return m_pOtherSkills;
}

void NPCSkillsManagerWidget::setAttributeValue(const QString &name, const int &value)
{
    m_attributes[name]->setValue( value );
}

void NPCSkillsManagerWidget::setAttributeModValue(const QString &name, const int &value)
{
    for ( NPCAttributeWidget *tAttribute: m_attributes )
        if ( *tAttribute->modValue() ) {
            tAttribute->setModValue( 0 );
            break;
        }
    m_attributes.value(name)->setModValue( value );
}

void NPCSkillsManagerWidget::setSpecialization(const QString &spec)
{
    if ( spec != m_specialization )
        m_specialization = spec;
}

void NPCSkillsManagerWidget::setBonusSkills(const QStringList &names, const int &value)
{
    for ( NPCAttributeWidget *attribute: m_attributes ) {
        for ( const QString &name: names ) {
            if ( attribute->skillPacks()->contains(name) ) {
                NPCAbstractSkillPack *skillpack = attribute->skillPacks()->value(name);
                for ( SkillSpinBox* skill: *skillpack->skillsValues() ) {
                    int minimum = skill->minimum() + value;
                    int tValue = skill->value() + value;
                    if ( 0 <= minimum ) {
                        skill->setMinimum( minimum );
                        skill->setValue( tValue );
                    }
                }
                break;
            }
        }
    }
}

void NPCSkillsManagerWidget::setBonusSpecPoints(const int &value)
{
    m_specPoints.first += value;
    updateInfoLabels();
}

void NPCSkillsManagerWidget::buySkillPack(const bool &bougth, const QStringList &specs)
{
    int tSkillPackCost = (bougth) ? 5 : -5;
    spendPoints( tSkillPackCost, specs );
}

void NPCSkillsManagerWidget::buySkill(const int &value, const QStringList &specs, const bool &increase)
{
    int tSkillCost = calculateSkillCost(value, increase);
    spendPoints( tSkillCost, specs );
}

void NPCSkillsManagerWidget::setAttributes(const QJsonArray &attributes)
{
    for ( const QJsonValue tAttribute: attributes ) {
        const QJsonObject &attribute = tAttribute.toObject();
        const QString &name = attribute.value("name").toString();
        NPCAttributeWidget *pAttribute = new NPCAttributeWidget(name, m_mods, this);
        pAttribute->setValue( 0 );
        m_attributes.insert( name, pAttribute );

        for ( const QJsonValue tSkillpack: attribute.value("skillPacks").toArray() ) {
            const QJsonObject &skillpack = tSkillpack.toObject();

            QStringList skills;
            for ( const QJsonValue skillName: skillpack.value("skills").toArray() )
                skills << skillName.toString();

            QStringList specs;
            for ( const QJsonValue specName: skillpack.value("Specialization").toArray() )
                specs << specName.toString();

            NPCAbstractSkillPack *pSkillpack;
            if ( skills.count() == 3 )
                pSkillpack = new NPCSkillPack(skillpack.value("name").toString(), specs, this);
            else
                pSkillpack = new NPCCustomSkillPack(skillpack.value("name").toString(), specs, this);
            pSkillpack->setFixedHeight( 90 );
            pSkillpack->addSkills( skills );

            connect( pSkillpack, &NPCAbstractSkillPack::bougth,
                  this, &NPCSkillsManagerWidget::buySkillPack );
            connect( this, &NPCSkillsManagerWidget::availableSkillpointsValueChanged,
                  pSkillpack, &NPCAbstractSkillPack::onAvailableSkillPointsChanged );
            connect( pSkillpack, &NPCAbstractSkillPack::skillValueChanged,
                  this, &NPCSkillsManagerWidget::buySkill );

            m_attributes.value(name)->addSkillPack(skillpack.value("name").toString(), pSkillpack);
        }
    }
}

QGroupBox *NPCSkillsManagerWidget::infoLabels()
{
    QGroupBox *pInfoGroupBox = new QGroupBox( "Punkty do wydania", this );

    QGridLayout *pLayout = new QGridLayout;
    pLayout->addWidget( new QLabel("Umiejętności:", pInfoGroupBox), 0, 0 );
    pLayout->addWidget( m_pSkillPointsLabel, 0, 1 );
    pLayout->addWidget( new QLabel("Specializacja:", pInfoGroupBox), 1, 0 );
    pLayout->addWidget( m_pSpecPointsLabel, 1, 1 );
    pInfoGroupBox->setLayout( pLayout );

    return pInfoGroupBox;
}

QVBoxLayout *NPCSkillsManagerWidget::columnA()
{
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( m_attributes.value("Budowa") );
    pLayout->addWidget( m_attributes.value("Zręczność") );
    pLayout->setSpacing( 1 );
    return pLayout;
}

QVBoxLayout *NPCSkillsManagerWidget::columnB()
{
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( m_attributes.value("Charakter") );
    pLayout->addWidget( m_attributes.value("Percepcja") );
    pLayout->setSpacing( 1 );
    return pLayout;
}

QVBoxLayout *NPCSkillsManagerWidget::columnC()
{
    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget( infoLabels() );
    pLayout->addWidget( m_attributes.value("Spryt") );
    pLayout->addWidget( m_pOtherSkills );
    pLayout->setSpacing( 1 );
    return pLayout;
}

void NPCSkillsManagerWidget::updateInfoLabels()
{
    m_pSkillPointsLabel->setText( QString::number(m_skillPoints.first-m_skillPoints.second) );
    m_pSpecPointsLabel->setText( QString::number(m_specPoints.first-m_specPoints.second) );
}

void NPCSkillsManagerWidget::spendPoints(const int &value, const QStringList &specs)
{
    if ( specs.contains(m_specialization) ) {
        if ( (0 < value) && (m_specPoints.first < m_specPoints.second+value) ) {
            const int costLeft = (m_specPoints.second + value) - m_specPoints.first;
            m_specPoints.second += value-costLeft;
            m_skillPoints.second += costLeft;
        }
        else if ( (0 > value) && (0 > m_specPoints.second + value) ) {
            const int pointsLeft = m_specPoints.second + value;
            m_specPoints.second += value-pointsLeft;
            m_skillPoints.second += pointsLeft;
        }
        else
            m_specPoints.second += value;
    }
    else {
        m_skillPoints.second += value;
    }
    emit availableSkillpointsValueChanged( m_skillPoints.first - m_skillPoints.second,
                                           m_specPoints.first - m_specPoints.second,
                                           m_specialization );
    updateInfoLabels();
}

int NPCSkillsManagerWidget::calculateSkillCost(const int &value, const bool &increase)
{
    int cost = 0;
    int multi = (increase) ? 1 : -1;
    int newValue = (increase) ? value : value + 1;

    if ( 1 < newValue )
        cost = newValue * multi;
    else
        cost = 3 * multi;

    return cost;
}
