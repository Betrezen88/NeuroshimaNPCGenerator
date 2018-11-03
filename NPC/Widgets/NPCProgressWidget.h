﻿#ifndef NPCPROGRESSWIDGET_H
#define NPCPROGRESSWIDGET_H

#include <QWidget>
#include <QPair>
#include <QLabel>
#include <QComboBox>

#include "NPC/Widgets/NPCTrickWdgetItem.h"

class NPCProgressWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NPCProgressWidget(QComboBox *pSpecialization, QWidget *parent = nullptr);

signals:
    void availableSkillPointsChanged(const int value);
    void addTrick(NPCTrickWdgetItem *trick);

public slots:
    void onSkillPackBougth(const bool &bougth, const QStringList &specs);
    void onSkillBougth(const int &value, const QStringList &specs, const bool &increase);
    void onRefundPoints(const int value, const QStringList &specs);
    void onTrickBougth(QListWidgetItem *trick);

private:
    void updateLabels();
    int calculateSkillCost(const int &value, const bool &increase);
    int availableExperience();

private:
    const QComboBox *m_pSpecialization;

    QLabel *m_pSkillPoints{nullptr};
    QLabel *m_pGambels{nullptr};
    QLabel *m_pExperience{nullptr};

    QPair<const int, int> m_specPoints{ 35, 0 };
    QPair<const int, int> m_skillPoints{ 30, 0 };
    QPair<const int, int> m_gambels{ 100, 0 };
    QPair<const int, int> m_tricks{ 1, 0 };
    // Experience { Wydane, Zdobyte }
    QPair<int, int> m_experience{ 0, 0 };

    // costs
    const int m_trickCost{ 200 };
};

#endif // NPCPROGRESSWIDGET_H
