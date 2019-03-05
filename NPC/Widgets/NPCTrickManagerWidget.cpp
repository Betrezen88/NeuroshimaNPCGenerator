#include "NPCTrickManagerWidget.h"

#include "../Utils/DataLoader.h"

#include <QGridLayout>
#include <QLabel>
#include <QJsonValue>
#include <QJsonObject>

NPCTrickManagerWidget::NPCTrickManagerWidget(
        const QHash<QString, NPCAttributeWidget*> *attributes,
        QWidget *parent)
    : QWidget(parent),
      m_pAttributes(attributes),
      m_pAvailable(new QListWidget(this)),
      m_pUnavailable(new QListWidget(this)),
      m_pBougth(new QListWidget(this))
{
    connect( m_pAvailable, &QListWidget::itemDoubleClicked,
             this, &NPCTrickManagerWidget::availableTrickDoubleClicked );
    connect( m_pBougth, &QListWidget::itemDoubleClicked,
             this, &NPCTrickManagerWidget::bougthTrickDoubleClicked );

    loadTricks( DataLoader::loadJson(":/data/json/Tricks.json") );

    QGridLayout *pLayout = new QGridLayout;
    pLayout->addWidget( new QLabel("Dostępne"), 0, 0 );
    pLayout->addWidget( m_pAvailable, 1, 0 );
    pLayout->addWidget( new QLabel("Nieodstępne"), 2, 0 );
    pLayout->addWidget( m_pUnavailable, 3, 0 );
    pLayout->addWidget( new QLabel("Kupione"), 0, 1 );
    pLayout->addWidget( m_pBougth, 1, 1, 4, 1 );
    setLayout( pLayout );
}

void NPCTrickManagerWidget::setBonusTrick(const QString &name)
{
    QListWidget *pListWidget = (!m_pAvailable->findItems(name, Qt::MatchCaseSensitive).isEmpty())
                                ? m_pAvailable : m_pUnavailable;
    QListWidgetItem* trick = pListWidget->findItems(name, Qt::MatchCaseSensitive).first();

    m_pBougth->addItem( pListWidget->takeItem(pListWidget->row(trick)) );
    m_pBougth->sortItems();
}

void NPCTrickManagerWidget::removeBonusTrick(const QString &name)
{
    QListWidgetItem *trick = m_pBougth->findItems( name, Qt::MatchCaseSensitive ).first();
    QListWidget *pListWidget = ( isTrickAvailable(static_cast<NPCTrickWidgetItem*>(trick)) )
                                ? m_pAvailable : m_pUnavailable;
    pListWidget->addItem( m_pBougth->takeItem(m_pBougth->row(trick)) );
    pListWidget->sortItems();
}

void NPCTrickManagerWidget::availableTrickDoubleClicked(QListWidgetItem *item)
{
    item->setData( 0x0100, "new" );
    m_pBougth->addItem( m_pAvailable->takeItem(m_pAvailable->row(item)) );
    m_pBougth->sortItems( Qt::AscendingOrder );
}

void NPCTrickManagerWidget::bougthTrickDoubleClicked(QListWidgetItem *item)
{
    if ( "new" == item->data(0x0100).toString() ) {
        item->setData( 0x0100, "" );
        m_pAvailable->addItem( m_pBougth->takeItem(m_pBougth->row(item)) );
        m_pAvailable->sortItems( Qt::AscendingOrder );
    }
}

void NPCTrickManagerWidget::loadTricks(const QJsonArray &tricks)
{
    for ( const QJsonValue tTrick: tricks ) {
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
                pItem->addAttribute( attribute.value("name").toString(),
                                     attribute.value("value").toInt() );
            }
        }

        if ( requirements.keys().contains("skills") ) {
            const QJsonArray &skills = requirements.value("skills").toArray();
            for ( const QJsonValue &tSkill: skills ) {
                const QJsonObject &skill = tSkill.toObject();
                pItem->addSkill( skill.value("name").toString(),
                                 skill.value("value").toInt() );
            }
        }

        if ( requirements.keys().contains("orSkills") ) {
            const QJsonArray &skills = requirements.value("orSkills").toArray();
            for ( const QJsonValue &tSkill: skills ) {
                const QJsonObject &skill = tSkill.toObject();
                pItem->addOrSkill( skill.value("name").toString(),
                                   skill.value("value").toInt() );
            }
        }

        pItem->createTooltipText();

        if ( isTrickAvailable(pItem) )
            m_pAvailable->addItem( pItem );
        else
            m_pUnavailable->addItem( pItem );
    }
}

bool NPCTrickManagerWidget::isTrickAvailable(const NPCTrickWidgetItem *pItem) const
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
                    const QList<NPCSkillPackWidget*> &skillPacks =
                            m_pAttributes->value(attribute)->skillPacks()->values();
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
                    const QList<NPCSkillPackWidget*> &skillPacks =
                            m_pAttributes->value(attribute)->skillPacks()->values();
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
