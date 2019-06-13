#ifndef NPCSHOPITEM_H
#define NPCSHOPITEM_H

#include "NPCCustomWidget.h"

#include <QJsonObject>

class Dice;

class QFrame;
class QGridLayout;
class QLabel;
class QPushButton;

class NPCShopItem : public NPCCustomWidget
{
    Q_OBJECT
public:
    NPCShopItem(const QJsonObject &item, QWidget *parent = nullptr);

signals:
    void checkAvailability(const int &availability, NPCShopItem *item);
    void checkQuantity(const QString &type, NPCShopItem *item);
    void quantityChanged(const int &quantity);
    void itemBougth(const QJsonObject &item);

private slots:
    void checkItemAvailability();
    void transformToBuyBtn();
    void buyItem();

private:
    void handWeaponLayout();
    void tossingWeaponLayout();
    void gunWeaponLayout();
    void armorLayout();

    void addRequirements();
    void addDamage();
    void addPenetration();
    void addSpecial();
    void addDescription();
    void addAvailability();
    void addAmmo();
    void addMagazine();
    void addDurability();
    void addDefence();
    void addPenalty();
    void addButton();
    void setQuantity(const int &value);

    void rollQuantity();
    void ammoQuantity(int &quantity, Dice &k10);
    void handWeaponQuantity(int &quantity, Dice &k10);
    void armorQuantity(int &quantity, Dice &k10);
    void usableQuantity(int &quantity, Dice &k10);

    QString gambleText(const int &price);

private:
    QJsonObject m_item;
    QGridLayout *m_pLayout{nullptr};
    QLabel *m_pName{nullptr};
    QLabel *m_pPrice{nullptr};
    QLabel *m_pQuantity{nullptr};
    QFrame *m_pLine{nullptr};
    QPushButton *m_pBuyBtn{nullptr};

    int m_row{0};
    int m_quantity{0};

    const QString m_nameStyle{
        "QLabel#Name{"
        " font: 11 bold;"
        " padding-right: 5px;"
        "}"
    };
};

#endif // NPCSHOPITEM_H
