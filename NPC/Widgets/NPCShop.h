#ifndef NPCSHOP_H
#define NPCSHOP_H

#include <QWidget>
#include <QHash>

class QComboBox;
class QLabel;
class QListWidget;
class QListWidgetItem;

class NPCShopItem;
class NPCInventory;
class NPCItem;

class NPCShop : public QWidget
{
    Q_OBJECT
public:
    explicit NPCShop(QWidget *parent = nullptr);

    NPCInventory *inventory() const;

signals:
    void moneyValueChanged(const int &value);

private slots:
    void addItemToInventory(const QJsonObject &item);
    void setSubcategory(const QString &categoryName);
    void showItems(const QString &subcategoryName);
    void checkItemAvailability(const int &availability, NPCShopItem *item);
    void checkItemQuantity(const QString &type, NPCShopItem *item);
    void returnItemToShop(const QString &name, const int &value);

private:
    void addItemsToShop();
    void hideItems();
    NPCShopItem *findItemInShopByName(const QString &name);

private:
    QComboBox *m_pCategory{nullptr};
    QComboBox *m_pSubcategory{nullptr};
    NPCInventory *m_pInventory{nullptr};
    QListWidget *m_pShop{nullptr};
    QLabel *m_pMoney{nullptr};

    QHash<QString, QHash<QString, QVector<QListWidgetItem*>>> m_items;

    const QString m_titleStyle{
        "QLabel#Title{"
        " font: bold 12pt;"
        " color: white;"
        " background-color: black;"
        " padding: 5px;"
        " border: 1px solid black;"
        " border-radius: 5px;"
        "}"
    };
};

#endif // NPCSHOP_H
