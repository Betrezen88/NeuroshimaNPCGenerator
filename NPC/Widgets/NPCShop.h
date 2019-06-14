#ifndef NPCSHOP_H
#define NPCSHOP_H

#include <QWidget>
#include <QHash>

class QComboBox;
class QLabel;
class QListWidget;
class QListWidgetItem;

class NPCShopItem;
class NPCItem;

class NPCShop : public QWidget
{
    Q_OBJECT
public:
    explicit NPCShop(QWidget *parent = nullptr);

signals:
    void moneyValueChanged(const int &value);

private slots:
    void addItemToInventory(const QJsonObject &item);
    void setSubcategory(const QString &categoryName);
    void showItems(const QString &subcategoryName);
    void checkItemAvailability(const int &availability, NPCShopItem *item);
    void checkItemQuantity(const QString &type, NPCShopItem *item);
    void returnItemToShop(const QString &name, const int &value);
    void destroyInventoryItem(NPCItem *item);

private:
    void addItemsToShop();
    void hideItems();
    NPCItem *findItemInInventoryByName(const QString &name);
    NPCShopItem *findItemInShopByName(const QString &name);

private:
    QComboBox *m_pCategory{nullptr};
    QComboBox *m_pSubcategory{nullptr};
    QListWidget *m_pInventory{nullptr};
    QListWidget *m_pShop{nullptr};
    QLabel *m_pMoney{nullptr};

    QHash<QString, QHash<QString, QVector<QListWidgetItem*>>> m_items;
};

#endif // NPCSHOP_H
