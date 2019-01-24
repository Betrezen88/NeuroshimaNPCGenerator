#ifndef CARDCONVERTER_H
#define CARDCONVERTER_H

#include <QJsonObject>

#include "NPC/NPCCardTab.h"

class CardConverter
{
public:
    CardConverter();

    const QJsonObject toJson(const NPCCardTab *card) const;
};

#endif // CARDCONVERTER_H
