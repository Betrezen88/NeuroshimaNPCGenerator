#include "CardConverter.h"

#include <QJsonValue>

CardConverter::CardConverter()
{

}

const QJsonObject CardConverter::toJson(const NPCCardTab *card) const
{
    QJsonObject cardJson;

    cardJson.insert( "personal", personalJson(card->obverse()) );

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
