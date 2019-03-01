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

void NPCSkillsManagerWidget::setProfessionBonus(const QJsonObject &bonus)
{
    if ( !m_professionBonus.isEmpty() )
        removeBonus( m_professionBonus );
    m_professionBonus = bonus;
    addBonus( bonus );
}

void NPCSkillsManagerWidget::setBonusSkills(const QString &name, const int &value)
{
    for ( NPCAttributeWidget *attribute: m_attributes ) {
        if ( attribute->skillPacks()->contains(name) ) {
            NPCSkillPackWidget *skillpack = attribute->skillPacks()->value(name);
            for ( QPair<const QLabel*, SkillSpinBox*> skill: skillpack->skills() ) {
                int minimum = skill.second->minimum() + value;
                int tValue = skill.second->value() + value;
                if ( 0 <= minimum ) {
                    skill.second->setMinimum( minimum );
                    skill.second->setValue( tValue );
                }
            }
            return;
        }
    }
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
    if ( "skillpack" == bonus.value("type").toString() ) {
        QStringList skillpacksName;
        if ( bonus.value("name").isString() )
            skillpacksName << bonus.value("name").toString();
        else if ( bonus.value("name").isArray() )
            for ( const QJsonValue name: bonus.value("name").toArray() )
                skillpacksName << name.toString();

        for ( NPCAttributeWidget *attribute: m_attributes ) {
            for ( const QString &name: skillpacksName ) {
                if ( attribute->skillPacks()->contains(name) ) {
                    NPCSkillPackWidget *skillpack = attribute->skillPacks()->value(name);
                    for ( QPair<const QLabel*, SkillSpinBox*> skill: skillpack->skills() ) {
                        const int &value = bonus.value("value").toInt();
                        int minimum = skill.second->minimum() + value;
                        int tValue = skill.second->value() + value;
                        skill.second->setValue( tValue );
                        skill.second->setMinimum( minimum );
                    }
                    break;
                }
            }
        }
    }
}

void NPCSkillsManagerWidget::removeBonus(const QJsonObject &bonus)
{
    if ( "skillpack" == bonus.value("type").toString() ) {
        QStringList skillpacksName;
        if ( bonus.value("name").isString() )
            skillpacksName << bonus.value("name").toString();
        else if ( bonus.value("name").isArray() )
            for ( const QJsonValue name: bonus.value("name").toArray() )
                skillpacksName << name.toString();

        for ( NPCAttributeWidget *attribute: m_attributes ) {
            for ( const QString &name: skillpacksName ) {
                if ( attribute->skillPacks()->contains(name) ) {
                    NPCSkillPackWidget *skillpack = attribute->skillPacks()->value(name);
                    for ( QPair<const QLabel*, SkillSpinBox*> skill: skillpack->skills() ) {
                        const int &value = bonus.value("value").toInt();
                        int minimum = skill.second->minimum() - value;
                        int tValue = skill.second->value() - value;
                        skill.second->setMinimum( minimum );
                        skill.second->setValue( tValue );
                    }
                    break;
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
