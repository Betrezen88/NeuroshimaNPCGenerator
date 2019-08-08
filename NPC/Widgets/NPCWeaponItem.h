#ifndef NPCWEAPONITEM_H
#define NPCWEAPONITEM_H

#include "NPCCustomWidget.h"

#include <QJsonObject>

class QFrame;
class QLabel;
class QPushButton;
class QVBoxLayout;

class NPCWeaponItem : public NPCCustomWidget
{
    Q_OBJECT
public:
    NPCWeaponItem(const QJsonObject &item, const int &body, QWidget *parent = nullptr);

    QJsonObject item() const;

signals:
    void unequip(const QJsonObject &item);
    void destroyView(NPCWeaponItem *item);

private:
    void gunLayout();
    void handWeaponLayout();

    QLabel *addDexterityBonus();
    QLabel *addDamage();
    QLabel *addSpecial();
    QLabel *addPenetration();
    QLabel *addAmmo();
    QLabel *addMagazine();
    QLabel *addJam();
    QLabel *addFirerate();
    QString damage(QString text);

private:
    QJsonObject m_item;
    int m_body{0};

    QVBoxLayout *m_pLayout{nullptr};
    QLabel *m_pName{nullptr};
    QPushButton *m_pButton{nullptr};
    QFrame *m_pLine{nullptr};

    const QString m_nameStyle{
        "QLabel#Name{"
        " font: bold 10pt;"
        "}"
    };

    const QString m_infoStyle{
        "QLabel#Info{"
        " font: 10pt;"
        "}"
    };

    const QString m_valueStyle{
        "QLabel#Value{"
        " font: italic 10pt;"
        "}"
    };
};

#endif // NPCWEAPONITEM_H
