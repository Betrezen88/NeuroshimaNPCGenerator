#ifndef NPCITEM_H
#define NPCITEM_H

#include "NPCCustomWidget.h"

#include <QJsonObject>

class QLabel;
class QPushButton;

class NPCItem : public NPCCustomWidget
{
    Q_OBJECT

public:
    explicit NPCItem(const QJsonObject &item, QWidget *parent = nullptr);

    QString name() const;

    void increaseQuantity();

signals:
    void returnItem(const QString &name, const int &price);
    void destroyItem(NPCItem *item);

private:
    QJsonObject m_item;
    QLabel *m_pName{nullptr};
    QLabel *m_pQuantity{nullptr};
    QPushButton *m_pReturnBtn{nullptr};
};

#endif // NPCITEM_H
