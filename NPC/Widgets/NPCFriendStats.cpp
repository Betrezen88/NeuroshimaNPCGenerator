#include "NPCFriendStats.h"
#include "NPCAttributeView.h"
#include "NPCSkillpackView.h"
#include "NPCGeneralSkillpack.h"
#include "../Utils/DataLoader.h"
#include "../Utils/Dice.h"

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QLineEdit>
#include <QPushButton>
#include <QStandardItem>
#include <QHBoxLayout>
#include <QVBoxLayout>

NPCFriendStats::NPCFriendStats(QWidget *parent)
    : QWidget(parent),
      m_pName( new QLineEdit(this) ),
      m_pSurname( new QLineEdit(this) ),
      m_pNickname( new QLineEdit(this) ),
      m_pProfession( new QLineEdit(this) ),
      m_pArchetype( new QComboBox(this) ),
      m_pConnection( new QComboBox(this) ),
      m_pOrigin( new QComboBox(this) ),
      m_pSickness( new QComboBox(this) ),
      m_pMod( new QComboBox(this) )
{
    connect( m_pArchetype, &QComboBox::currentTextChanged,
             this, &NPCFriendStats::onArchetypeChanged );
    connect( m_pMod, &QComboBox::currentTextChanged,
             this, &NPCFriendStats::onModChanged );
    connect( m_pConnection, &QComboBox::currentTextChanged,
             [this](){
        const int cost = m_pConnection->currentData().toInt();
        emit this->connectionChanged( cost );
    });

    init();

    QGridLayout *pLayout = new QGridLayout;
    setLayout( pLayout );
    pLayout->addWidget( personalBox(), 0, 0, 1, 1 );
    pLayout->addWidget( attributeBox(), 1, 0, 4, 1 );
}

QString NPCFriendStats::name() const
{
    return m_pName->text();
}

QString NPCFriendStats::surname() const
{
    return m_pSurname->text();
}

QString NPCFriendStats::nickname() const
{
    return m_pNickname->text();
}

QString NPCFriendStats::profession() const
{
    return m_pProfession->text();
}

QString NPCFriendStats::origin() const
{
    return m_pOrigin->currentText();
}

QString NPCFriendStats::sickness() const
{
    return m_pSickness->currentText();
}

QString NPCFriendStats::connection() const
{
    return m_pConnection->currentText();
}

const QHash<QString, NPCAttributeView *> &NPCFriendStats::attributes() const
{
    return m_attributes;
}

const QVector<NPCAbstractSkillpackView *> &NPCFriendStats::skillpacks() const
{
    return m_skillpacks;
}

void NPCFriendStats::random()
{
    Dice d20{20};
    Dice d6{6};

    randomAttributes();

    m_pArchetype->setCurrentIndex( static_cast<int>(d6.throwValue()-1) );

    int origin = static_cast<int>( d20.throwValue() );

    for ( int i{0}; i<m_pOrigin->count(); ++i ) {
        if ( "parent" == m_pOrigin->itemData( i, Qt::AccessibleDescriptionRole ).toString() ) {
            QJsonArray range = m_pOrigin->itemData( i, Qt::UserRole ).toJsonArray();
            if ( range.count() == 1 && range.first().toInt() == origin ) {
                m_pOrigin->setCurrentIndex( i );
                break;
            }
            else if ( range.first().toInt() <= origin && range.last().toInt() >= origin ) {
                int index = (1==i) ? (1+static_cast<int>(d20.throwValue())) : i;
                m_pOrigin->setCurrentIndex( index );
                break;
            }
        }
    }

    m_pSickness->setCurrentIndex( static_cast<int>(d20.throwValue()-1) );

    QJsonObject skills = DataLoader::loadJson( ":/data/json/Friend.json" ).at(6).toObject();
    randomSkills( skills.value("rules").toArray(), skills.value("data").toArray() );

    Dice d5{5};
    m_pMod->setCurrentIndex( static_cast<int>(d5.throwValue()-1) );
}

void NPCFriendStats::applyFeature(const QJsonObject &feature)
{
    if ( feature.contains("skill") ) {
        const QJsonObject &skill = feature.value("skill").toObject();
        setSkillValueBy( skill.value("name").toString(), skill.value("value").toInt() );
    }
    else if ( feature.contains("skills") ) {
        const QJsonArray &skills = feature.value("skills").toArray();
        for ( const QJsonValue skill: skills ) {
            const QJsonObject &tSkill = skill.toObject();
            setSkillValueBy( tSkill.value("name").toString(), tSkill.value("value").toInt() );
        }
    }
    else if ( feature.contains("attribute") ) {
        const QJsonObject &tAttribute = feature.value("attribute").toObject();
        setFixedAttributeValue( tAttribute.value("name").toString(), tAttribute.value("value").toInt() );
    }
}

void NPCFriendStats::onArchetypeChanged()
{
    int attributeValue{0};
    for ( const QJsonValue attribute: m_pArchetype->currentData(Qt::UserRole).toJsonArray() ) {
        const QString &name = attribute.toString();
        int value = (m_attributesFixed.contains(name))
                ? m_attributesFixed.value(name) : m_attributesValues.at(attributeValue).toInt();
        m_attributes.value(name)->setValue( value );
        ++attributeValue;
    }
}

void NPCFriendStats::onModChanged()
{
    Dice d20{20};
    QJsonObject mod = m_pMod->currentData(Qt::UserRole).toJsonObject();

    if ( !m_modSkillpacks.isEmpty() ) {
        setSkillpacksValues( m_modSkillpacks, false );
        m_modSkillpacks = QJsonArray();
    }

    for ( QJsonValue rule: mod.value("throws").toArray() ) {
        const QJsonObject &tRule = rule.toObject();
        for ( int count{0}; count<tRule.value("count").toInt(); ++count ) {
            int roll = static_cast<int>( d20.throwValue() );
            for ( const QJsonValue skillpack: mod.value("data").toArray() ) {
                const QJsonObject &tSkillpack = skillpack.toObject();
                const QJsonArray &range = tSkillpack.value("range").toArray();
                if ( (2 == range.count() && roll >= range.first().toInt() && roll <= range.last().toInt())
                     || (roll == range.first().toInt()) ) {
                    const QString &name = tSkillpack.value("name").toString();
                    QJsonObject pack;
                    pack.insert( "name", name );
                    pack.insert( "value", tRule.value("value").toInt() );
                    m_modSkillpacks.push_back( pack );
                }
            }
        }
    }

    if ( mod.contains("extra") ) {
        const QJsonObject &extra = mod.value("extra").toObject();
        for ( const QJsonValue skillpack: extra.value("skillpacks").toArray() ) {
            QJsonObject pack;
            pack.insert( "name", skillpack.toString() );
            pack.insert( "value", extra.value("value").toInt() );
            m_modSkillpacks.push_back( pack );
        }
    }

    QStringList skillpacks;
    for ( QJsonValue modSkillpack: m_modSkillpacks )
        skillpacks << modSkillpack.toObject().value("name").toString();

    for ( const QJsonValue skillpack: mod.value("data").toArray() ) {
        const QString &tSkillpackName = skillpack.toObject().value("name").toString();
        if ( !skillpacks.contains(tSkillpackName) ) {
            QJsonObject pack;
            pack.insert( "name", tSkillpackName );
            pack.insert( "value", 1 );
            m_modSkillpacks.push_back( pack );
        }
    }

    setSkillpacksValues( m_modSkillpacks, true );
}

void NPCFriendStats::setSkillpacksValues(const QJsonArray &skillpacks, const bool &add)
{
    int mod = (add) ? 1 : -1;
    for ( const QJsonValue skillpack: skillpacks ) {
        const QJsonObject &tSkillpack = skillpack.toObject();
        for ( NPCAbstractSkillpackView *pSkillpack: m_skillpacks ) {
            if ( tSkillpack.value("name").toString() == pSkillpack->name() ) {
                for ( const QPair<QString, int> &skill: pSkillpack->skills() )
                    pSkillpack->setSkillValueBy( skill.first, (mod*tSkillpack.value("value").toInt()) );
            }
        }
    }
}

void NPCFriendStats::init()
{
    QJsonArray attributes = DataLoader::loadJson( ":/data/json/Attributes.json" );
    for ( const QJsonValue attribute: attributes ) {
        const QJsonObject &tAttribute = attribute.toObject();
        const QString &attributeName = tAttribute.value("name").toString();
        NPCAttributeView *pAttribute = new NPCAttributeView(attributeName, 0, 0, this);
        m_attributes.insert( attributeName, pAttribute );

        for ( const QJsonValue skillpack: attribute.toObject().value("skillPacks").toArray() ) {
            const QJsonObject &tSkillpack = skillpack.toObject();

            QStringList specs, skills;
            for ( const QJsonValue spec: tSkillpack.value("Specialization").toArray() )
                specs << spec.toString();
            for ( const QJsonValue skill: tSkillpack.value("skills").toArray() )
                skills << skill.toString();

            NPCAbstractSkillpackView *pSkillpack;
            if ( 3 == skills.count() ) {
                pSkillpack = new NPCSkillpackView( tSkillpack.value("name").toString(),
                                                   specs,
                                                   skills,
                                                   this );
            }
            else {
                pSkillpack = new NPCGeneralSkillpack( tSkillpack.value("name").toString(),
                                                      specs,
                                                      skills,
                                                      this );
            }
            pAttribute->addSkillpack( pSkillpack );
            m_skillpacks.push_back( pSkillpack );
        }
    }

    randomAttributes();

    QJsonArray data = DataLoader::loadJson( ":/data/json/Friend.json" );

    for ( const QJsonValue object: data ) {
        const QJsonObject &tObject = object.toObject();
        if ( "connection" == tObject.value("name").toString() )
            initConnections( tObject.value("data").toArray() );
        else if ( "origin" == tObject.value("name").toString() )
            initOrigins( tObject.value("data").toArray() );
        else if ( "archetypes" == tObject.value("name").toString() )
            initArchetypes( tObject.value("data").toArray() );
        else if ( "skills" == tObject.value("name").toString() )
            randomSkills( tObject.value("rules").toArray(), tObject.value("data").toArray() );
        else if ( "mods" == tObject.value("name").toString() )
            initMods( tObject.value("data").toArray() );
    }

    QJsonArray diseases = DataLoader::loadJson( ":/data/json/Sickness.json" );

    for ( const QJsonValue disease: diseases )
        m_pSickness->addItem( disease.toObject().value("name").toString() );
}

void NPCFriendStats::initConnections(const QJsonArray &connections)
{
    QStandardItemModel *connectionModel = new QStandardItemModel;
    for ( const QJsonValue connection: connections ) {
        const QJsonObject &tConnection = connection.toObject();
        QStandardItem *pItem = new QStandardItem( tConnection.value("name").toString() );
        pItem->setToolTip( "<div style=\"width: 300px; word-wrap: break-word;\" align=\"justify\">"
                           +tConnection.value("description").toString()
                           +"</div>" );
        pItem->setData( tConnection.value("value").toInt(), Qt::UserRole );
        connectionModel->appendRow( pItem );
    }
    m_pConnection->setModel( connectionModel );
}

void NPCFriendStats::initOrigins(const QJsonArray &origins)
{
    QStandardItemModel *originModel = new QStandardItemModel;
    for ( const QJsonValue origin: origins ) {
        const QJsonObject &tOrigin = origin.toObject();
        QStandardItem *pItem = new QStandardItem( tOrigin.value("name").toString() );
        pItem->setData( tOrigin.value("range").toArray(), Qt::UserRole );
        pItem->setData( "parent", Qt::AccessibleDescriptionRole );
        originModel->appendRow( pItem );

        if ( tOrigin.contains("data") ) {
            pItem->setFlags( pItem->flags() & ~(Qt::ItemIsEnabled | Qt::ItemIsSelectable) );
            for ( const QJsonValue city: tOrigin.value("data").toArray() ) {
                QStandardItem *pSubitem = new QStandardItem( city.toString() );
                pSubitem->setData( "child", Qt::AccessibleDescriptionRole );
                originModel->appendRow( pSubitem );
            }
        }
    }
    m_pOrigin->setModel( originModel );
    m_pOrigin->setItemDelegate( new OriginDelegate );
}

void NPCFriendStats::initArchetypes(const QJsonArray &archetypes)
{
    QStandardItemModel *archetypeModel = new QStandardItemModel;
    for ( const QJsonValue archetype: archetypes ) {
        const QJsonObject &tArchetype = archetype.toObject();
        QStandardItem *pItem = new QStandardItem( tArchetype.value("name").toString() );
        pItem->setData( tArchetype.value("attributes").toArray(), Qt::UserRole );
        archetypeModel->appendRow( pItem );
    }
    m_pArchetype->setModel( archetypeModel );
}

void NPCFriendStats::initMods(const QJsonArray &mods)
{
    QStandardItemModel *modModel = new QStandardItemModel;
    for ( const QJsonValue mod: mods ) {
        const QJsonObject &tMod = mod.toObject();
        QStandardItem *pItem = new QStandardItem( tMod.value("name").toString() );
        pItem->setData( tMod, Qt::UserRole );
        modModel->appendRow( pItem );
    }
    m_pMod->setModel( modModel );
}

QGroupBox *NPCFriendStats::personalBox()
{
    QGroupBox *pGroupBox = new QGroupBox("Personalia");

    QGridLayout *pLayout = new QGridLayout;
    pGroupBox->setLayout( pLayout );
    pLayout->setSpacing( 5 );
    pLayout->addWidget( new QLabel("Imie:"), 0, 0 );
    pLayout->addWidget( m_pName, 0, 1 );
    pLayout->addWidget( new QLabel("Nazwisko:"), 1, 0 );
    pLayout->addWidget( m_pSurname, 1, 1 );
    pLayout->addWidget( new QLabel("Ksywa:"), 2, 0 );
    pLayout->addWidget( m_pNickname, 2, 1 );
    pLayout->addWidget( new QLabel("Profesja:"), 3, 0 );
    pLayout->addWidget( m_pProfession, 3, 1 );
    pLayout->addWidget( new QLabel("Archetyp:"), 4, 0 );
    pLayout->addWidget( m_pArchetype, 4, 1 );
    pLayout->addWidget( new QLabel("Powiązanie:"), 0, 2 );
    pLayout->addWidget( m_pConnection, 0, 3 );
    pLayout->addWidget( new QLabel("Pochodzenie:"), 1, 2 );
    pLayout->addWidget( m_pOrigin, 1, 3 );
    pLayout->addWidget( new QLabel("Choroba:"), 2, 2 );
    pLayout->addWidget( m_pSickness, 2, 3 );
    pLayout->addWidget( new QLabel("Modyfikator:"), 3, 2 );
    pLayout->addWidget( m_pMod, 3, 3 );

    return pGroupBox;
}

QGroupBox *NPCFriendStats::attributeBox()
{
    QGroupBox *pGroupBox = new QGroupBox("Współczynniki");
    pGroupBox->setMinimumWidth( 100 );

    QVBoxLayout *pColumn1 = new QVBoxLayout;
    pColumn1->addWidget( m_attributes.value("Budowa") );
    pColumn1->addWidget( m_attributes.value("Zręczność") );

    QVBoxLayout *pColumn2 = new QVBoxLayout;
    pColumn2->addWidget( m_attributes.value("Charakter") );
    pColumn2->addWidget( m_attributes.value("Percepcja") );

    QVBoxLayout *pColumn3 = new QVBoxLayout;
    pColumn3->addWidget( m_attributes.value("Spryt") );
    pColumn3->addSpacerItem( new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding) );

    QHBoxLayout *pLayout = new QHBoxLayout;
    pGroupBox->setLayout( pLayout );
    pLayout->addLayout( pColumn1 );
    pLayout->addLayout( pColumn2 );
    pLayout->addLayout( pColumn3 );

    return pGroupBox;
}

void NPCFriendStats::randomSkills(const QJsonArray &rules, const QJsonArray &data)
{
    Dice d20{20};
    if ( !m_basicSkillpacks.isEmpty() ) {
        setSkillpacksValues( m_basicSkillpacks, false );
        m_basicSkillpacks = QJsonArray();
    }

    for ( const QJsonValue rule: rules ) {
        const QJsonObject &tRule = rule.toObject();

        for ( int i{0}; i<tRule.value("count").toInt(); ++i ) {
            int roll = static_cast<int>( d20.throwValue() );
            for ( const QJsonValue skillpack: data ) {
                const QJsonObject &tSkillpack = skillpack.toObject();
                const QJsonArray &range = tSkillpack.value("range").toArray();
                if ( (2 == range.count() && roll >= range.first().toInt() && roll <= range.last().toInt())
                     || (roll == range.first().toInt()) ) {
                    const QString &name = tSkillpack.value("name").toString();
                    QJsonObject pack;
                    pack.insert( "name", name );
                    pack.insert( "value", tRule.value("value").toInt() );
                    m_basicSkillpacks.push_back( pack );
                }
            }
        }
    }
    setSkillpacksValues( m_basicSkillpacks, true );
}

void NPCFriendStats::randomAttributes()
{
    Dice d20{20};
    QJsonArray attributes = DataLoader::loadJson(":/data/json/Friend.json").at(4).toObject().value("data").toArray();

    m_attributesValues = attributes.at(static_cast<int>(d20.throwValue())).toArray();
}

void NPCFriendStats::setSkillValueBy(const QString &name, const int &value)
{
    for ( NPCAttributeView *pAttribute: m_attributes.values() )
        for ( NPCAbstractSkillpackView *pSkillpack: pAttribute->skillpacks().values() )
            if ( pSkillpack->hasSkill(name) ) {
                pSkillpack->setSkillValueBy( name, value );
                break;
            }
}

void NPCFriendStats::setFixedAttributeValue(const QString &name, const int &value)
{
    m_attributesFixed.insert( name, value );
    m_attributes.value(name)->setValue( value );
}
