#ifndef NPCOTHERSKILLS_H
#define NPCOTHERSKILLS_H

#include "NPCCustomWidget.h"
#include "SkillSpinBox.h"

#include <QHash>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QVector>

class NPCOtherSkills : public NPCCustomWidget
{
    Q_OBJECT
public:
    NPCOtherSkills(QWidget *parent = nullptr);

    QHash<QLabel*, QPair<QLabel*, SkillSpinBox*>> skills() const;

signals:
    void skillValueChanged(const int &value, const QStringList &specs, const bool &increase);

public slots:
    void onAvailableSkillpointsChanged(const int &skill);

private slots:
    void addSkill(const QString name, const QString attribute);
    void showSkillChooseDialog();
    void removeSkill(QListWidgetItem *row, QLabel *name);

private:
    bool isSkillOnList(const QString &name);

private:
    QLabel *m_pName{nullptr};
    QPushButton *m_pAddBtn{nullptr};
    QListWidget *m_pSkills{nullptr};

    QHash<QLabel*, QPair<QLabel*, SkillSpinBox*>> m_skills;

    const QString m_style {
        "QWidget#OtherSkills{"
        " border: 1px solid black;"
        " border-radius: 5px;"
        " padding: 5px;"
        "}"
    };

    const QString m_title {
        "QLabel#Title{"
        " padding-left: 5px;"
        " font: 12 bold;"
        "}"
    };
};

#endif // NPCOTHERSKILLS_H
