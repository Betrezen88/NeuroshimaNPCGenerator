#ifndef NPCFRIENDMANAGER_H
#define NPCFRIENDMANAGER_H

#include <QWidget>

class QLabel;
class QPushButton;
class QTabWidget;

class NPCFriendView;
class NPCFriendCard;

class NPCFriendManager : public QWidget
{
public:
    NPCFriendManager(QWidget *parent = nullptr);

private slots:
    void showCreatorDialog() const;
    void addFriend(const QJsonObject &pal, const int &cost);
    void removeFriend(const int &index);

private:
    QTabWidget *m_pFriends{nullptr};
    QPushButton *m_pAddBtn{nullptr};
    QPushButton *m_pEditBtn{nullptr};
    QPushButton *m_pRemoveBtn{nullptr};

    QLabel *m_pCash{nullptr};

    QVector<NPCFriendCard*> m_friends;
    QHash<QString, int> m_prices;

    const QString m_titleStyle{
        "QLabel#Title{"
        " padding: 5px;"
        " background-color: black;"
        " color: white;"
        " border-radius: 5px;"
        " font: 20px;"
        "}"
    };
};

#endif // NPCFRIENDMANAGER_H
