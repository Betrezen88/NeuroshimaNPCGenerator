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
    const QJsonArray attributesJson(const QHash<QString, NPCAttributeWidget*> *attributes) const;
    const QJsonArray packetsJson(QHash<QString, NPCSkillPackWidget *> &skillPacks) const;
    const QJsonArray skillsJson(const QVector<QPair<const QLabel*, SkillSpinBox*>> &skills) const;
};

#endif // CARDCONVERTER_H
