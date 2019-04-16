#ifndef CARDCONVERTER_H
#define CARDCONVERTER_H

#include <QJsonObject>

#include "NPC/NPCCardTab.h"

class CardConverter
{
public:
    CardConverter();

    const QJsonObject toJson(const NPCCardTab *card) const;

private:
    const QJsonObject personalJson(const NPCCardObverse *obverse) const;
    const QJsonArray tricksJson(const QListWidget *tricks) const;
    const QJsonArray attributesJson(const QHash<QString, NPCAttributeView *> *attributes) const;
    const QJsonArray packetsJson(const QHash<QString, NPCSkillpackView *> &skillPacks) const;
    const QJsonArray skillsJson(const QHash<QString, QLabel *> &skills) const;

    void personal(NPCCardTab *card, const QJsonObject &object);
    void stats(NPCCardTab *card, const QJsonArray &stats);
};

#endif // CARDCONVERTER_H
