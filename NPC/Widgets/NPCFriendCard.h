#ifndef NPCFRIENDCARD_H
#define NPCFRIENDCARD_H

//#include "NPCCustomWidget.h"
#include <QTabWidget>
#include <QJsonObject>

class NPCFriendObverse;
class NPCCardReverse;

class NPCFriendCard : public QTabWidget
{
public:
    explicit NPCFriendCard(const QJsonObject &pal, QWidget *parent = nullptr);

    NPCFriendObverse *obverse() const;
    NPCCardReverse *reverse() const;

private:
    NPCFriendObverse *m_pObverse{nullptr};
    NPCCardReverse *m_pReverse{nullptr};

    QJsonObject m_pal;
};

#endif // NPCFRIENDCARD_H
