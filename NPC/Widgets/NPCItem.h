#ifndef NPCITEM_H
#define NPCITEM_H

#include "NPCCustomWidget.h"

#include <QJsonObject>

class QLabel;

class NPCItem : public NPCCustomWidget
{
    Q_OBJECT

public:
    explicit NPCItem(const QJsonObject &item, QWidget *parent = nullptr);

    QString name() const;

    void increaseQuantity();

private:
    QJsonObject m_item;
    QLabel *m_pName{nullptr};
    QLabel *m_pQuantity{nullptr};
};

#endif // NPCITEM_H
