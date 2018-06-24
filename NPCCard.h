#ifndef NPCCARD_H
#define NPCCARD_H

#include <QWidget>
#include <QJsonDocument>

#include "PersonalSection.h"

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
};

#endif // NPCCARD_H
