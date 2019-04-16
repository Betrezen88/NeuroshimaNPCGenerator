#include "CardConverter.h"

#include <QJsonValue>

CardConverter::CardConverter()
{

}

const QJsonObject CardConverter::toJson(const NPCCardTab *card) const
{
    QJsonObject cardJson;

    cardJson.insert( "personal", personalJson(card->obverse()) );
    cardJson.insert( "tricks", tricksJson(card->obverse()->tricks()) );
    cardJson.insert( "stats", attributesJson(card->obverse()->attributes()) );

    return cardJson;
}

NPCCardTab* CardConverter::toCard(const QJsonObject &object)
{
    NPCCardTab *pCard = new NPCCardTab();

    personal( pCard, object.value("personal").toObject() );
    stats( pCard, object.value("stats").toArray() );
    tricks( pCard, object.value("tricks").toArray() );

    return pCard;
}

const QJsonObject CardConverter::personalJson(const NPCCardObverse *obverse) const
{
    QJsonObject personalObj;

    personalObj.insert( "name", obverse->heroName() );
    personalObj.insert( "origin", obverse->origin() );
    personalObj.insert( "profession", obverse->profession() );
    personalObj.insert( "specialization", obverse->specialization() );
    personalObj.insert( "sickness", obverse->sickness() );
    personalObj.insert( "reputation", obverse->reputation() );
    personalObj.insert( "fame", obverse->fame() );

    QJsonObject featuresObj;
    featuresObj.insert( "origin", obverse->originFeature() );
    featuresObj.insert( "profession", obverse->professionFeature() );

    personalObj.insert( "features", featuresObj );

    return personalObj;
}

const QJsonArray CardConverter::tricksJson(const QListWidget *tricks) const
{
    QJsonArray tricksObj;
    for ( int i=0; i<tricks->count(); ++i )
        tricksObj.push_back( tricks->item(i)->text() );
    return tricksObj;
}

const QJsonArray CardConverter::attributesJson(const QHash<QString, NPCAttributeView *> *attributes) const
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

const QJsonArray CardConverter::packetsJson(const QHash<QString, NPCSkillpackView *> &skillPacks) const
{
    QJsonArray packetsArray;

    for ( NPCSkillpackView *skillPack: skillPacks ) {
        QJsonObject skillPackObj;
        skillPackObj.insert( "name", skillPacks.key(skillPack) );
        skillPackObj.insert( "skills", skillsJson(skillPack->skills()) );
        packetsArray.push_back( skillPackObj );
    }

    return packetsArray;
}

const QJsonArray CardConverter::skillsJson(const QHash<QString, QLabel *> &skills) const
{
    QJsonArray skillsArray;

    for ( QLabel *skill: skills ) {
        QJsonObject skillObj;
        skillObj.insert( "name", skills.key(skill) );
        skillObj.insert( "value", skill->text().toInt() );
        skillsArray.push_back( skillObj );
    }

    return skillsArray;
}

void CardConverter::personal(NPCCardTab *card, const QJsonObject &object)
{
    NPCCardObverse *pObverse = card->obverse();
    pObverse->setName( object.value("name").toString() );
    pObverse->setOrigin( object.value("origin").toString() );
    pObverse->setProfession( object.value("profession").toString() );
    pObverse->setSickness( object.value("sickness").toString(), "" );
    pObverse->setSpecialization( object.value("specialization").toString() );

    const QJsonObject &features = object.value("features").toObject();

    pObverse->setOriginFeature( features.value("origin").toString(), "" );
    pObverse->setProfessionFeature( features.value("profession").toString(), "" );
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
            NPCSkillpackView *pSkillpack =
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
    for ( const QJsonValue trick: tricks )
        tTricks.push_back( new QListWidgetItem(trick.toString()) );
    pObverse->setTricks( tTricks );
}
