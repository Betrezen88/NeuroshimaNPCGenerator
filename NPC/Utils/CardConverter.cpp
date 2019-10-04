#include "CardConverter.h"
#include "NPC/Widgets/NPCAbstractSkillpackView.h"
#include "NPC/Widgets/NPCAttributeView.h"
#include "NPC/Widgets/NPCInventory.h"
#include "NPC/Widgets/NPCItem.h"
#include "NPC/Widgets/NPCArmor.h"
#include "NPC/Widgets/NPCWeaponView.h"
#include "NPC/Widgets/NPCReputationView.h"
#include "NPC/Widgets/NPCTrickWidgetItem.h"

#include <QJsonValue>

#include <QDebug>

CardConverter::CardConverter()
{

}

QJsonObject CardConverter::toJson(const NPCCardTab *card, QString filePath) const
{
    QJsonObject cardJson;

    cardJson.insert( "personal", personalJson(card->obverse(), filePath) );
    cardJson.insert( "tricks", tricksJson(card->obverse()->tricks()) );
    cardJson.insert( "stats", attributesJson(card->obverse()->attributes()) );

    QJsonObject equipment;
    equipment.insert( "inventory", inventoryJson(card->reverse()->inventory()->items()) );
    equipment.insert( "armor", card->reverse()->armor()->equiped() );
    equipment.insert( "weapons", card->reverse()->weaponView()->weapons() );

    cardJson.insert( "equipment", equipment );

    return cardJson;
}

NPCCardTab* CardConverter::toCard(const QJsonObject &object)
{
    NPCCardTab *pCard = new NPCCardTab();

    personal( pCard, object.value("personal").toObject() );
    stats( pCard, object.value("stats").toArray() );
    tricks( pCard, object.value("tricks").toArray() );
    equipment( pCard->reverse(), object.value("equipment").toObject() );

    return pCard;
}

QJsonObject CardConverter::personalJson(const NPCCardObverse *obverse, QString filePath) const
{
    QJsonObject personalObj;

    personalObj.insert( "name", obverse->heroName() );
    personalObj.insert( "origin", obverse->origin() );
    personalObj.insert( "profession", obverse->profession() );
    personalObj.insert( "specialization", obverse->specialization() );
    personalObj.insert( "sickness", obverse->sickness() );
    personalObj.insert( "reputation", reputationJson(obverse->reputation()->reputation()) );
    personalObj.insert( "portrait", filePath );

    QJsonObject origin;
    origin.insert( "name", obverse->originFeature() );
    origin.insert( "description", obverse->originFeatureDescription() );

    QJsonObject profession;
    profession.insert( "name", obverse->professionFeature() );
    profession.insert( "description", obverse->professionFeatureDescription() );

    QJsonObject featuresObj;
    featuresObj.insert( "origin", origin );
    featuresObj.insert( "profession", profession );

    personalObj.insert( "features", featuresObj );

    return personalObj;
}

QJsonArray CardConverter::reputationJson(QHash<QString, int> reputation) const
{
    QJsonArray tReputation;

    for ( const QString &place: reputation.keys() ) {
        QJsonObject tPlace;
        tPlace.insert( "name", place );
        tPlace.insert( "value", reputation.value(place) );
        tReputation.push_back( tPlace );
    }

    return tReputation;
}

QJsonArray CardConverter::tricksJson(const QListWidget *tricks) const
{
    QJsonArray tricksObj;
    for ( int i=0; i<tricks->count(); ++i ) {
        NPCTrickWidgetItem *pTrick = dynamic_cast<NPCTrickWidgetItem*>(tricks->item(i));
        QJsonObject trick;
        trick.insert( "name", pTrick->text() );
        trick.insert( "description", pTrick->description() );
        trick.insert( "action", pTrick->action() );

        QJsonArray attributes;
        for ( const QString &attribute: pTrick->attributes()->keys() ) {
            QJsonObject tAttribute;
            tAttribute.insert( "name", attribute );
            tAttribute.insert( "value", pTrick->attributes()->value(attribute) );
            attributes.push_back( tAttribute );
        }
        if ( !attributes.isEmpty() ) trick.insert( "attributes", attributes );

        QJsonArray skills;
        for ( const QString &skill: pTrick->skills()->keys() ) {
            QJsonObject tSkill;
            tSkill.insert( "name", skill );
            tSkill.insert( "value", pTrick->skills()->value(skill) );
            skills.push_back( tSkill );
        }
        if ( !skills.isEmpty() ) trick.insert( "skills", skills );

        QJsonArray orSkills;
        for ( const QString &orSkill: pTrick->orSkills()->keys() ) {
            QJsonObject tOrSkill;
            tOrSkill.insert( "name", orSkill );
            tOrSkill.insert( "value", pTrick->orSkills()->value(orSkill) );
            orSkills.push_back( tOrSkill );
        }
        if ( !orSkills.isEmpty() ) trick.insert( "orSkill", orSkills );

        tricksObj.push_back( trick );

    }
    return tricksObj;
}

QJsonArray CardConverter::attributesJson(const QHash<QString, NPCAttributeView *> *attributes) const
{
    QJsonArray attributesObj;

    for ( NPCAttributeView *attribute: *attributes ) {
        QJsonObject tAttributeObj;
        tAttributeObj.insert( "name", attributes->key(attribute) );
        tAttributeObj.insert( "value", attribute->value() );
        tAttributeObj.insert( "mod", attribute->modValue() );
        tAttributeObj.insert( "skillPacks", packetsJson(attribute->skillpacks()) );
        attributesObj.push_back( tAttributeObj );
    }

    return attributesObj;
}

QJsonArray CardConverter::packetsJson(const QHash<QString, NPCAbstractSkillpackView *> &skillPacks) const
{
    QJsonArray packetsArray;

    for ( NPCAbstractSkillpackView *skillPack: skillPacks ) {
        QJsonObject skillPackObj;
        skillPackObj.insert( "name", skillPacks.key(skillPack) );
        skillPackObj.insert( "skills", skillsJson(skillPack->skills()) );
        packetsArray.push_back( skillPackObj );
    }

    return packetsArray;
}

QJsonArray CardConverter::skillsJson(const QVector<QPair<QString, int>> &skills) const
{
    QJsonArray skillsArray;

    for ( const QPair<QString, int> &skill: skills ) {
        QJsonObject skillObj;
        skillObj.insert( "name", skill.first );
        skillObj.insert( "value", skill.second );
        skillsArray.push_back( skillObj );
    }

    return skillsArray;
}

QJsonArray CardConverter::inventoryJson(const QVector<NPCItem *> items) const
{
    QJsonArray inventory;
    for ( NPCItem *item: items ) {
        QJsonObject tItem;
        tItem.insert( "item", item->object() );
        tItem.insert( "quantity", item->quantity() );
        inventory.push_back( tItem );
    }
    return inventory;
}

void CardConverter::personal(NPCCardTab *card, const QJsonObject &object)
{
    NPCCardObverse *pObverse = card->obverse();
    pObverse->setName( object.value("name").toString() );
    pObverse->setOrigin( object.value("origin").toString() );
    pObverse->setProfession( object.value("profession").toString() );
    pObverse->setSickness( object.value("sickness").toString(), "" );
    pObverse->setSpecialization( object.value("specialization").toString() );

    reputation( pObverse->reputation(), object.value("reputation").toArray() );

    const QJsonObject &origin = object.value("features").toObject().value("origin").toObject();
    const QJsonObject &profession = object.value("features").toObject().value("profession").toObject();

    pObverse->setOriginFeature( origin.value("name").toString(),
                                origin.value("description").toString() );
    pObverse->setProfessionFeature( profession.value("name").toString(),
                                    profession.value("description").toString() );
    pObverse->setPortrait( QPixmap(object.value("portrait").toString()) );
}

void CardConverter::reputation(NPCReputationView *reputationView, const QJsonArray &reputation)
{
    for ( const QJsonValue place: reputation ) {
        const QJsonObject &tPlace = place.toObject();
        reputationView->setPlaceReputation( tPlace.value("name").toString(), tPlace.value("value").toInt() );
    }
}

void CardConverter::stats(NPCCardTab *card, const QJsonArray &stats)
{
    NPCCardObverse *pObverse = card->obverse();

    for ( const QJsonValue attribute: stats ) {
        const QJsonObject &tAttribute = attribute.toObject();
        NPCAttributeView *pAttribute =
                pObverse->attributes()->value(tAttribute.value("name").toString());
        pAttribute->setValue( tAttribute.value("value").toInt() );
        pAttribute->setModValue( tAttribute.value("mod").toInt() );

        for ( const QJsonValue skillpack: tAttribute.value("skillPacks").toArray() ) {
            const QJsonObject &tSkillpack = skillpack.toObject();
            NPCAbstractSkillpackView *pSkillpack =
                    pAttribute->skillpacks().value( tSkillpack.value("name").toString() );
            for ( const QJsonValue skill: tSkillpack.value("skills").toArray() ) {
                const QJsonObject &tSkill = skill.toObject();
                pSkillpack->setSkillValue( tSkill.value("name").toString(),
                                           tSkill.value("value").toInt() );
            }
        }
    }
}

void CardConverter::tricks(NPCCardTab *card, const QJsonArray &tricks)
{
    QVector<QListWidgetItem*> tTricks;
    NPCCardObverse *pObverse = card->obverse();
    for ( const QJsonValue trick: tricks ) {
        const QJsonObject &tTrick = trick.toObject();
        NPCTrickWidgetItem *pTrick = new NPCTrickWidgetItem( tTrick.value("name").toString(),
                                                             tTrick.value("description").toString(),
                                                             tTrick.value("action").toString(),
                                                             tTrick.value("attributes").toArray(),
                                                             tTrick.value("skills").toArray(),
                                                             tTrick.value("orSkill").toArray() );

        tTricks.push_back( dynamic_cast<QListWidgetItem*>(pTrick) );
    }
    pObverse->setTricks( tTricks );
}

void CardConverter::equipment(NPCCardReverse *reverse, const QJsonObject &equipment)
{
    if ( !equipment.value("armor").toArray().isEmpty() ) {
        for ( const QJsonValue armor: equipment.value("armor").toArray() )
            reverse->armor()->addArmor( armor.toObject() );
    }

    if ( !equipment.value("weapons").toArray().isEmpty() ) {
        for ( const QJsonValue weapon: equipment.value("weapons").toArray() )
            reverse->weaponView()->addWeapon( weapon.toObject() );
    }

    if ( !equipment.value("inventory").toArray().isEmpty() ) {
        for ( const QJsonValue item: equipment.value("inventory").toArray() )
            reverse->inventory()->addItem( item.toObject().value("item").toObject(),
                                           item.toObject().value("quantity").toInt() );
    }
}
