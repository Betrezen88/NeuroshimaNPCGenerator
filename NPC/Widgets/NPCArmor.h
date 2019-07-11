#ifndef NPCARMOR_H
#define NPCARMOR_H

#include "NPCCustomWidget.h"

#include <QJsonObject>

class QLabel;

class NPCArmor: public NPCCustomWidget
{
    Q_OBJECT
public:
    NPCArmor(QWidget *parent);

signals:
    void unequip(const QJsonObject &item);

public slots:
    void addArmor(const QJsonObject &item);

private slots:
    void removeArmor(const QJsonObject &item);

private:
    QLabel *infoLabel(QString text, QString name, QString style);

    void addArmorPiece(const QString &location, const QString &name, const int &value);
    void removeArmorPiece(const QString &location);

    QJsonArray overlapingArmor(QJsonArray &defence);

private:
    QLabel *m_pTitle{nullptr};
    QLabel *m_pHead{nullptr};
    QLabel *m_pHeadItem{nullptr};
    QLabel *m_pTorso{nullptr};
    QLabel *m_pTorsoItem{nullptr};
    QLabel *m_pLeftHand{nullptr};
    QLabel *m_pLeftHandItem{nullptr};
    QLabel *m_pRightHand{nullptr};
    QLabel *m_pRightHandItem{nullptr};
    QLabel *m_pLeftLeg{nullptr};
    QLabel *m_pLeftLegItem{nullptr};
    QLabel *m_pRightLeg{nullptr};
    QLabel *m_pRightLegItem{nullptr};

    QHash<QString, QJsonObject> m_equiped;

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

    const QString m_infoStyle{
        "QLabel#Info{"
        " font: bold 10pt;"
        "}"
    };

    const QString m_subtitleStyle{
        "QLabel#Subtitle{"
        " font: bold 10pt;"
        " color: white;"
        " padding: 3px;"
        " background-color: black;"
        " border: 1px solid black;"
        " border-radius: 5px;"
        "}"
    };
};

#endif // NPCARMOR_H
