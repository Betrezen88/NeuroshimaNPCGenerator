#ifndef NPCWEAPONVIEW_H
#define NPCWEAPONVIEW_H

#include "NPCCustomWidget.h"
#include "NPCWeaponItem.h"

#include <QJsonObject>

class QLabel;
class QListWidget;

class NPCWeaponView : public NPCCustomWidget
{
    Q_OBJECT
public:
    NPCWeaponView(QWidget *parent);

signals:
    void unequip(const QJsonObject &item);

public slots:
    void addWeapon(const QJsonObject &item);

private slots:
    void removeWeapon(NPCWeaponItem *weapon);

private:
    QLabel *m_pTitle{nullptr};
    QListWidget *m_pWeapons{nullptr};

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

#endif // NPCWEAPONVIEW_H
