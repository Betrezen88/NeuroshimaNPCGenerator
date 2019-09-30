#ifndef NPCFRIENDEQUIPMENT_H
#define NPCFRIENDEQUIPMENT_H

#include <QWidget>

class QComboBox;
class QLabel;
class QListWidget;
class QPushButton;

class NPCFriendEquipment : public QWidget
{
    Q_OBJECT
public:
    explicit NPCFriendEquipment(QWidget *parent = nullptr);

    QJsonArray equipment() const;

signals:
    void itemBougth(const int &value);
    void itemSold(const int &value);

public slots:
    void setAvailableCash(const int &value);

private slots:
    void showCategoryItems(const QString &category);
    void buyItem();
    void sellItem();

private:
    void addShopItems();
    void checkItemsAvailability();
    QString itemTooltip(const QJsonObject &item);
    bool isInEquipment(const QString &text);

private:
    QComboBox *m_pCategory{nullptr};
    QListWidget *m_pEquipment{nullptr};
    QListWidget *m_pShop{nullptr};
    QPushButton *m_pRemoveBtn{nullptr};
    QPushButton *m_pAddBtn{nullptr};

    int m_cash{0};
};

#endif // NPCFRIENDEQUIPMENT_H
