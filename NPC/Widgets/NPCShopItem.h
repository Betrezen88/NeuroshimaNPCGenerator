#ifndef NPCSHOPITEM_H
#define NPCSHOPITEM_H

#include "NPCCustomWidget.h"

#include <QJsonObject>

class Dice;

class QFrame;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QPushButton;

class NPCShopItem : public NPCCustomWidget
{
    Q_OBJECT
public:
    NPCShopItem(const QJsonObject &item, QWidget *parent = nullptr);

    QString name() const;
    int quantity() const;

signals:
    void checkAvailability(const int &availability, NPCShopItem *item);
    void checkQuantity(const QString &type, NPCShopItem *item);
    void quantityChanged(const int &quantity);
    void itemBougth(const QJsonObject &item);

public slots:
    void setAvailable(const bool &available);
    void setQuantity(const int &value);
    void setSoldout(const int &quantity);
    void checkPrice(const int &money);

private slots:
    void buyItem();
    void setBuyBtn();

private:
    void handWeaponLayout();
    void tossingWeaponLayout();
    void gunWeaponLayout();
    void armorLayout();

    QHBoxLayout *addBonus();
    QHBoxLayout *addRequirements();
    QHBoxLayout *addDamage();
    QHBoxLayout *addPenetration();
    QHBoxLayout *addSpecial();
    void addDescription();
    void addAvailability();
    QHBoxLayout *addAmmo();
    QHBoxLayout *addMagazine();
    QHBoxLayout *addDurability();
    void addDefence();
    QHBoxLayout *addPenalty();
    void addButton();

    QString gambleText(const int &price);
    QString damageText(const QJsonValue &value);

private:
    QJsonObject m_item;
    QGridLayout *m_pLayout{nullptr};
    QLabel *m_pName{nullptr};
    QLabel *m_pPrice{nullptr};
    QLabel *m_pQuantity{nullptr};
    QFrame *m_pLine{nullptr};
    QPushButton *m_pButton{nullptr};

    int m_row{0};
    int m_quantity{0};

    const QString m_nameStyle2{
        "QLabel#Name{"
        " font: 13 bold;"
        " padding-right: 5px;"
        "}"
    };

    const QString m_nameStyle { "font: 13 bold;" };

    const QString m_infoStyle{ "font: 11 bold;" };
    const QString m_valueStyle{ "font: 11 italic;" };
};

#endif // NPCSHOPITEM_H
