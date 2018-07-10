#ifndef NPCCARD_H
#define NPCCARD_H

#include <QWidget>
#include <QJsonDocument>

#include "PersonalSection.h"
#include "AttributeSection.h"
#include "SkillsSection.h"
#include "Tricks.h"

class NPCCard : public QWidget
{
    Q_OBJECT
public:
    explicit NPCCard(QWidget *parent = nullptr);

private:
    QJsonDocument json();

private:
    QJsonDocument m_json;

    PersonalSection* m_personal;
    AttributeSection* m_attributes;
    SkillsSection* m_skillsSection;
    Tricks* m_tricks;
};

#endif // NPCCARD_H
