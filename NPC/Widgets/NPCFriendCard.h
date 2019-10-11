#ifndef NPCFRIENDCARD_H
#define NPCFRIENDCARD_H

#include <QTabWidget>
#include <QJsonObject>

class NPCFriendObverse;
class NPCCardReverse;
class NPCCardForm;

class NPCFriendCard : public QTabWidget
{
public:
    explicit NPCFriendCard(const QJsonObject &pal, QWidget *parent = nullptr);

    NPCFriendObverse *obverse() const;
    NPCCardReverse *reverse() const;
    NPCCardForm *form() const;

private:
    NPCFriendObverse *m_pObverse{nullptr};
    NPCCardReverse *m_pReverse{nullptr};
    NPCCardForm *m_pForm{nullptr};

    QJsonObject m_pal;
};

#endif // NPCFRIENDCARD_H
