#ifndef NPCFRIENDMANAGER_H
#define NPCFRIENDMANAGER_H

#include <QWidget>

class QLabel;
class QListWidget;
class QPushButton;

class NPCFriendManager : public QWidget
{
public:
    NPCFriendManager(QWidget *parent = nullptr);

private slots:
    void showCreatorDialog() const;

private:
    QListWidget *m_pFriends{nullptr};
    QPushButton *m_pAddBtn{nullptr};
    QPushButton *m_pEditBtn{nullptr};
    QPushButton *m_pRemoveBtn{nullptr};

    QLabel *m_pCash{nullptr};

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
