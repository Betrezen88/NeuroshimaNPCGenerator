#ifndef NPCFRIENDEQUIPMENT_H
#define NPCFRIENDEQUIPMENT_H

#include <QWidget>

class QComboBox;
class QListWidget;
class QPushButton;

class NPCFriendEquipment : public QWidget
{
    Q_OBJECT
public:
    explicit NPCFriendEquipment(QWidget *parent = nullptr);

signals:

public slots:

private:
    void addShopItems();

private:
    QComboBox *m_pCategory{nullptr};
    QListWidget *m_pEquipment{nullptr};
    QListWidget *m_pShop{nullptr};
    QPushButton *m_pRemoveBtn{nullptr};
    QPushButton *m_pAddBtn{nullptr};
};

#endif // NPCFRIENDEQUIPMENT_H
