#ifndef NPCCARDREVERSE_H
#define NPCCARDREVERSE_H

#include <QWidget>

class NPCArmor;
class NPCWeaponView;
class NPCInventory;

class NPCCardReverse : public QWidget
{
    Q_OBJECT
public:
    explicit NPCCardReverse(int *body, QWidget *parent = nullptr);

    NPCInventory *inventory() const;
    NPCArmor *armor() const;

signals:

public slots:

private:
    NPCArmor *m_pArmor{nullptr};
    NPCWeaponView *m_pWeaponView{nullptr};
    NPCInventory *m_pInventory{nullptr};
};

#endif // NPCCARDREVERSE_H
