#ifndef NPCWEAPONITEM_H
#define NPCWEAPONITEM_H

#include "NPCCustomWidget.h"

#include <QJsonObject>

class QLabel;
class QVBoxLayout;
class QFrame;

class NPCWeaponItem : public NPCCustomWidget
{
public:
    NPCWeaponItem(const QJsonObject &item, const int &body, QWidget *parent = nullptr);

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
