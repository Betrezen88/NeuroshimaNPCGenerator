#ifndef NPCINVENTORY_H
#define NPCINVENTORY_H

#include "NPCCustomWidget.h"
#include <QJsonObject>

class NPCItem;

class QLabel;
class QListWidget;

class NPCInventory : public NPCCustomWidget
{
    Q_OBJECT
public:
    NPCInventory(QWidget *parent = nullptr);

    QVector<NPCItem*> items() const;

signals:
    void returnItemToShop(const QString &name, const int &price);
    void equipArmor(QJsonObject &item);
    void equipWeapon(QJsonObject &item);

public slots:
    void addItem(NPCItem *item);
    void addItem(const QJsonObject item, const int quantity);
    void destroyItem(NPCItem *item);

private slots:
    void onEquip(QJsonObject &item);

private:
    NPCItem *findItemByName(const QString &name);

private:
    QLabel *m_pTitle{nullptr};
    QListWidget *m_pInventory{nullptr};

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

#endif // NPCINVENTORY_H
