#ifndef CARDCONVERTER_H
#define CARDCONVERTER_H

#include <QJsonObject>

#include "NPC/NPCCardTab.h"

class NPCItem;

class CardConverter
{
public:
    CardConverter();

    const QJsonObject toJson(const NPCCardTab *card, QString filePath) const;
    NPCCardTab *toCard(const QJsonObject &object);

private:
    QJsonObject personalJson(const NPCCardObverse *obverse, QString filePath) const;
    QJsonArray tricksJson(const QListWidget *tricks) const;
    QJsonArray attributesJson(const QHash<QString, NPCAttributeView *> *attributes) const;
    QJsonArray packetsJson(const QHash<QString, NPCSkillpackView *> &skillPacks) const;
    QJsonArray skillsJson(const QHash<QLabel *, QLabel *> &skills) const;
    QJsonArray inventoryJson(const QVector<NPCItem*> items) const;
    QJsonArray armorJson(const QJsonArray items);

    void personal(NPCCardTab *card, const QJsonObject &object);
    void stats(NPCCardTab *card, const QJsonArray &stats);
    void tricks(NPCCardTab *card, const QJsonArray &tricks);
};

#endif // CARDCONVERTER_H
