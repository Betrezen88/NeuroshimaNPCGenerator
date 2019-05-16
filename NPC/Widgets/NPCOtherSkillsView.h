#ifndef NPCOTHERSKILLSVIEW_H
#define NPCOTHERSKILLSVIEW_H

#include "NPCCustomWidget.h"

class QGridLayout;
class QLabel;

class NPCOtherSkillsView : public NPCCustomWidget
{
public:
    NPCOtherSkillsView(const QString &name, QWidget *parent = nullptr);

    void addSkill(const QString &name, const QString &attribute, const int &value);
    bool hasSkill(const QString &name) const;
    QHash<QLabel *, QPair<QLabel *, QLabel *> > skills() const;

private:
    QGridLayout *m_pLayout{nullptr};
    QLabel *m_pName{nullptr};

    QHash<QLabel*, QPair<QLabel*, QLabel*>> m_skills;

    const QString m_titleStyle{
        "QLabel#Title{"
        " font: bold;"
        " font-size: 12em;"
        " padding-left: 5px;"
        "}"
    };

    const QString m_valueStyle{
        "QLabel#Value{"
        " font: bold;"
        " padding-left: 3px;"
        "}"
    };

    const QString m_skillpackStyle{
        "QWidget#Skillpack{"
        " border: 1px solid black;"
        " border-radius: 5px;"
        "}"
    };
};

#endif // NPCOTHERSKILLSVIEW_H
