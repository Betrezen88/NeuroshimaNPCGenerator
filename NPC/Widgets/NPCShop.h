#ifndef NPCSHOP_H
#define NPCSHOP_H

#include <QWidget>

class QListWidget;

class NPCShop : public QWidget
{
    Q_OBJECT
public:
    explicit NPCShop(QWidget *parent = nullptr);

signals:

public slots:

private:
    void addItemsToShop();

private:
    QListWidget *m_pInventory{nullptr};
    QListWidget *m_pShop{nullptr};
};

#endif // NPCSHOP_H
