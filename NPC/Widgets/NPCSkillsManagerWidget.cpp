#include "NPCSkillsManagerWidget.h"
#include "../Utils/DataLoader.h"

#include <QJsonValue>
#include <QJsonObject>
#include <QVBoxLayout>

#include <QDebug>

NPCSkillsManagerWidget::NPCSkillsManagerWidget(const QJsonArray *attributes, QWidget *parent)
    : QWidget(parent),
      m_pSkillPointsLabel(new QLabel(this)),
      m_pSpecPointsLabel(new QLabel(this))
{
    setAttributes( *attributes );
    updateInfoLabels();

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addLayout( columnA() );
    pLayout->addLayout( columnB() );
    pLayout->addLayout( columnC() );

    setLayout( pLayout );
}

void NPCSkillsManagerWidget::setAttributeValue(const QString &name, const int &value)
{
    m_attributes[name]->setValue( value );
}

void NPCSkillsManagerWidget::setAttributeModValue(const QString &name, const int &value)
{
    for ( NPCAttributeWidget *tAttribute: m_attributes )
        if ( tAttribute->modValue() ) {
            tAttribute->setModValue( 0 );
            break;
        }
    m_attributes.value(name)->setModValue( value );
}

void NPCSkillsManagerWidget::setSpecialization(const QString &spec)
{
    if ( spec != m_specialization ) {
        m_specialization = spec;
    }
}

void NPCSkillsManagerWidget::setOriginBonus(const QJsonObject &bonus)
{
    if ( !m_originBonus.isEmpty() )
        removeBonus( m_originBonus );
    addBonus( bonus );
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
        m_attributes.insert( name, new NPCAttributeWidget(name, m_mods, this) );

        for ( const QJsonValue tSkillpack: attribute.value("skillPacks").toArray() ) {
             const QJsonObject &skillpack = tSkillpack.toObject();
             NPCSkillPackWidget *pSkillpack = new NPCSkillPackWidget(skillpack.value("name").toString(),
                                                                     this);
             pSkillpack->setFixedHeight( 90 );
             for ( const QJsonValue tSpec: skillpack.value("Specialization").toArray() )
                 pSkillpack->addSpecialization( tSpec.toString() );

             for ( const QJsonValue tSkill: skillpack.value("skills").toArray() )
                 pSkillpack->addSkill( tSkill.toString(), new SkillSpinBox(this) );

             connect( pSkillpack, &NPCSkillPackWidget::bougth,
                      this, &NPCSkillsManagerWidget::buySkillPack );
             connect( this, &NPCSkillsManagerWidget::availableSkillpointsValueChanged,
                      pSkillpack, &NPCSkillPackWidget::onAvailableSkillPointsChanged );
             connect( pSkillpack, &NPCSkillPackWidget::skillValueChanged,
                      this, &NPCSkillsManagerWidget::buySkill );

             m_attributes.value(name)->addSkillPack(skillpack.value("name").toString(), pSkillpack);
        }
    }
}

void NPCSkillsManagerWidget::addBonus(const QJsonObject &bonus)
{
    m_originBonus = bonus;
    if ( "skillpack" == bonus.value("type").toString() ) {
        const QString &skillpackName = bonus.value("name").toString();
        for ( NPCAttributeWidget *attribute: m_attributes ) {
            if ( attribute->skillPacks()->contains(skillpackName) ) {
                NPCSkillPackWidget *skillpack = attribute->skillPacks()->value(skillpackName);
                for ( QPair<const QLabel*, SkillSpinBox*> skill: skillpack->skills() ) {
                    skill.second->setValue( bonus.value("value").toInt() );
                    skill.second->setMinimum( bonus.value("value").toInt() );
                }
                break;
            }
        }
    }
}

void NPCSkillsManagerWidget::removeBonus(const QJsonObject &bonus)
{
    if ( "skillpack" == bonus.value("type").toString() ) {
        const QString &skillpackName = bonus.value("name").toString();
        for ( NPCAttributeWidget *attribute: m_attributes ) {
            if ( attribute->skillPacks()->contains(skillpackName) ) {
                NPCSkillPackWidget *skillpack = attribute->skillPacks()->value(skillpackName);
                for ( QPair<const QLabel*, SkillSpinBox*> skill: skillpack->skills() ) {
                    skill.second->setMinimum( 0 );
                    skill.second->setValue( 0 );
                }
            }
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
    if ( specs.contains(m_specialization) )
        m_specPoints.second += value;
    else
        m_skillPoints.second += value;
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
