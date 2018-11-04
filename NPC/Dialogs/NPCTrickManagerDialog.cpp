#include "NPCTrickManagerDialog.h"

#include "../Utils/DataLoader.h"
#include "NPC/Widgets/NPCTrickWidgetItem.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QGridLayout>

#include <QDebug>

NPCTrickManagerDialog::NPCTrickManagerDialog(const QHash<QString, NPCAttributeWidget*> *attributes,
                                             QWidget *parent)
    : QDialog(parent),
      m_pAttributes(attributes),
      m_pAvailable(new QListWidget(this)),
      m_pUnavailable(new QListWidget(this)),
      m_pBougth(new QListWidget(this)),
      m_pAcceptBtn(new QPushButton("Akceptuj", this)),
      m_pCancelBtn(new QPushButton("Anuluj", this))
{
    setWindowTitle( "Zarządzaj sztuczkami" );
    setMinimumSize( 200, 200 );

    QHBoxLayout *pButtonsL = new QHBoxLayout;
    pButtonsL->addWidget( m_pAcceptBtn );
    pButtonsL->addWidget( m_pCancelBtn );

    connect( m_pAvailable, &QListWidget::itemDoubleClicked,
             this, &NPCTrickManagerDialog::buyTrick );
    connect( m_pBougth, &QListWidget::itemDoubleClicked,
             this, &NPCTrickManagerDialog::resignTrick );
    connect( m_pCancelBtn, &QPushButton::clicked,
             this, &NPCTrickManagerDialog::close);

    QGridLayout *pLayout = new QGridLayout;
    pLayout->addWidget( new QLabel("Dostępne", this), 0, 0 );
    pLayout->addWidget( m_pAvailable, 1, 0 );
    pLayout->addWidget( new QLabel("Niedostępne", this), 2, 0 );
    pLayout->addWidget( m_pUnavailable, 3, 0 );
    pLayout->addWidget( new QLabel("Posiadane", this), 0, 1 );
    pLayout->addWidget( m_pBougth, 1, 1, 3, 1 );
    pLayout->addLayout( pButtonsL, 4, 1 );
    setLayout( pLayout );
}

void NPCTrickManagerDialog::distributeTricks()
{
    m_pAvailable->clear();
    m_pUnavailable->clear();

    addTricks( DataLoader::loadJson( ":/Tricks.json" ) );
}

void NPCTrickManagerDialog::trickBougth(NPCTrickWidgetItem *trick)
{
    m_pBougth->addItem( m_pAvailable->takeItem(m_pAvailable->row(trick)) );
}

void NPCTrickManagerDialog::trickRemove(NPCTrickWidgetItem *trick)
{
    m_pAvailable->addItem( m_pBougth->takeItem(m_pBougth->row(trick)) );
}

void NPCTrickManagerDialog::addTricks(const QJsonArray &tricks)
{
    for ( const QJsonValue &tTrick: tricks ) {
        const QJsonObject &trick = tTrick.toObject();

        const QString &name = trick.value("name").toString();
        const QString &description = trick.value("description").toString();
        const QString &action = trick.value("action").toString();
        const QJsonObject &requirements = trick.value("requirements").toObject();

        NPCTrickWidgetItem *pItem = new NPCTrickWidgetItem(name, description, action);
        if ( requirements.keys().contains("attributes") ) {
            const QJsonArray &attributes = requirements.value("attributes").toArray();
            for ( const QJsonValue &tAttribute: attributes ) {
                const QJsonObject &attribute = tAttribute.toObject();
                pItem->addAttribute( attribute.value("name").toString(), attribute.value("value").toInt() );
            }
        }

        if ( requirements.keys().contains("skills") ) {
            const QJsonArray &skills =  requirements.value("skills").toArray();
            for ( const QJsonValue &tSkill: skills ) {
                const QJsonObject &skill = tSkill.toObject();
                pItem->addSkill( skill.value("name").toString(), skill.value("value").toInt() );
            }
        }

        if ( requirements.keys().contains("orSkills") ) {
            const QJsonArray &skills = requirements.value("orSkills").toArray();
            for ( const QJsonValue &tSkill: skills ) {
                const QJsonObject &skill = tSkill.toObject();
                pItem->addOrSkill( skill.value("name").toString(), skill.value("value").toInt() );
            }
        }

        pItem->createTooltipText();

        if ( isTrickAvailable(pItem) )
            m_pAvailable->addItem( pItem );
        else
            m_pUnavailable->addItem( pItem );
    }
}

bool NPCTrickManagerDialog::isTrickAvailable(const NPCTrickWidgetItem *pItem) const
{
    bool attr{true}, skill{true}, orSkill{true};

    const QHash<QString, int> *attributes = pItem->attributes();
    const QHash<QString, int> *skills = pItem->skills();
    const QHash<QString, int> *orSkills = pItem->orSkills();

    if ( !attributes->isEmpty() ) {
        for ( const QString &name: attributes->keys() ) {
            if ( !name.isEmpty() )
            if ( attributes->value(name) > *m_pAttributes->value(name)->value() ) {
                attr = false;
                break;
            }
        }
    }

    if ( !skills->empty() ) {
        for ( const QString &name: skills->keys() ) {
            if ( !name.isEmpty() )
                for ( const QString &attribute: m_pAttributes->keys() ) {
                    const QList<NPCSkillPackWidget*> &skillPacks = m_pAttributes->value(attribute)->skillPacks()->values();
                    for ( const NPCSkillPackWidget *skillPack: skillPacks ) {
                        if ( skillPack->hasSkill(name) ) {
                            if ( skills->value(name) > skillPack->skill(name)->value() ) {
                                skill = false;
                                break;
                            }
                        }
                    }
                }
            }
    }

    if ( !orSkills->empty() ) {
        for ( const QString &name: orSkills->keys() ) {
            if ( !name.isEmpty() )
                for ( const QString &attribute: m_pAttributes->keys() ) {
                    const QList<NPCSkillPackWidget*> &skillPacks = m_pAttributes->value(attribute)->skillPacks()->values();
                    for ( const NPCSkillPackWidget *skillPack: skillPacks ) {
                        if ( skillPack->hasSkill(name) ) {
                            if ( orSkills->value(name) > skillPack->skill(name)->value() ) {
                                orSkill = false;
                                break;
                            }
                        }
                    }
                }
            }
    }

    return attr && skill && orSkill;
}
