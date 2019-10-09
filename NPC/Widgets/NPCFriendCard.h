#ifndef NPCFRIENDCARD_H
#define NPCFRIENDCARD_H

//#include "NPCCustomWidget.h"
#include <QTabWidget>
#include <QJsonObject>

class NPCFriendObverse;

class NPCFriendCard : public QTabWidget
{
public:
    explicit NPCFriendCard(const QJsonObject &pal, QWidget *parent = nullptr);

    const NPCFriendObverse &obverse() const;

private:
    NPCFriendObverse *m_pObverse{nullptr};
    QWidget *m_pReverse{nullptr};

    QJsonObject m_pal;
};

#endif // NPCFRIENDCARD_H
