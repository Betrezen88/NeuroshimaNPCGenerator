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

const QJsonObject CardConverter::personalJson(const NPCCardObverse *obverse) const
{
    QJsonObject personalObj;

    personalObj.insert( "name", obverse->heroName() );
    personalObj.insert( "origin", obverse->origin() );
    personalObj.insert( "profession", obverse->origin() );
    personalObj.insert( "specialization", obverse->origin() );
    personalObj.insert( "sickness", obverse->origin() );
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

const QJsonArray CardConverter::attributesJson(const QHash<QString, NPCAttributeWidget *> *attributes) const
{
    QJsonArray attributesObj;

    for ( NPCAttributeWidget *attribute: *attributes ) {
        QJsonObject tAttributeObj;
        tAttributeObj.insert( "name", attributes->key(attribute) );
        tAttributeObj.insert( "value", *attribute->value() );
        tAttributeObj.insert( "mod", *attribute->modValue() );
        tAttributeObj.insert( "skillPacks", packetsJson(*attribute->skillPacks()) );
        attributesObj.push_back( tAttributeObj );
    }

    return attributesObj;
}

const QJsonArray CardConverter::packetsJson(const QHash<const QString, NPCSkillPackWidget *> &skillPacks) const
{
    QJsonArray packetsArray;

    for ( NPCSkillPackWidget *skillPack: skillPacks ) {
        QJsonObject skillPackObj;
        skillPackObj.insert( "name", skillPacks.key(skillPack) );
        skillPackObj.insert( "bougth", skillPack->isBougth() );
        skillPackObj.insert( "skills", skillsJson(skillPack->skills()) );
        packetsArray.push_back( skillPackObj );
    }

    return packetsArray;
}

const QJsonArray CardConverter::skillsJson(const QVector<QPair<const QLabel *, SkillSpinBox *> > &skills) const
{
    QJsonArray skillsArray;

    for ( QPair<const QLabel*, SkillSpinBox*> skill: skills ) {
        QJsonObject skillObj;
        skillObj.insert( "name", skill.first->text() );
        skillObj.insert( "value", skill.second->value() );
        skillsArray.push_back( skillObj );
    }

    return skillsArray;
}
