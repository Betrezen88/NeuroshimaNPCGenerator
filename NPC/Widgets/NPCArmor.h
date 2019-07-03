#ifndef NPCARMOR_H
#define NPCARMOR_H

#include "NPCCustomWidget.h"

#include <QJsonObject>

class QLabel;

class NPCArmor: public NPCCustomWidget
{
public:
    NPCArmor(QWidget *parent);

public slots:
    void addArmor(const QJsonObject &item);

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

    const QString m_titleStyle{
        "QLabel#Title{"
        " font: 12pt;"
        " padding: 5pt;"
        " border: 1px solid black;"
        " border-radius: 5px;"
        "}"
    };
};

#endif // NPCARMOR_H
