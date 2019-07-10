#ifndef NPCITEM_H
#define NPCITEM_H

#include "NPCCustomWidget.h"

#include <QJsonObject>

class QAction;
class QLabel;
class QPushButton;

class NPCItem : public NPCCustomWidget
{
    Q_OBJECT
public:
    enum class Type { INVENTORY, SHOP };

    explicit NPCItem(const QJsonObject &item, Type type, QWidget *parent = nullptr);

    QString name() const;
    Type type() const;

    void increaseQuantity();
    void decreaseQuantity();

signals:
    void typeChanged(const Type &type);
    void returnItem(const QString &name, const int &price);
    void destroyItem(NPCItem *item);
    void equip(QJsonObject &item);

public slots:
    void setType(const Type &type);

private slots:
    void throwOut();

private:
    void createActions();

private:
    QJsonObject m_item;
    Type m_type;
    QLabel *m_pName{nullptr};
    QLabel *m_pQuantity{nullptr};
    QPushButton *m_pButton{nullptr};

    QAction *m_pReturn{nullptr};
    QAction *m_pThrowOut{nullptr};
    QAction *m_pUse{nullptr};
    QAction *m_pEquip{nullptr};
};

#endif // NPCITEM_H
