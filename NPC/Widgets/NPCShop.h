#ifndef NPCSHOP_H
#define NPCSHOP_H

#include <QWidget>
#include <QHash>

class QComboBox;
class QListWidget;
class QListWidgetItem;

class NPCShopItem;

struct Category {

};

class NPCShop : public QWidget
{
    Q_OBJECT
public:
    explicit NPCShop(QWidget *parent = nullptr);

signals:

public slots:

private slots:
    void setSubcategory(const QString &categoryName);
    void showItems(const QString &subcategoryName);

private:
    void addItemsToShop();

    void hideItems();

private:
    QComboBox *m_pCategory{nullptr};
    QComboBox *m_pSubcategory{nullptr};
    QListWidget *m_pInventory{nullptr};
    QListWidget *m_pShop{nullptr};

    QHash<QString, QHash<QString, QVector<QListWidgetItem*>>> m_items;
};

#endif // NPCSHOP_H
