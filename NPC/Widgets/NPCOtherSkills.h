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

    QVector<QStringList> skills() const;

signals:
    void skillValueChanged(const int &value, const QStringList &specs, const bool &increase);

public slots:
    void onAvailableSkillpointsChanged(const int &skill);

private slots:
    void addSkill(const QString name, const QString attribute);
    void showSkillChooseDialog();

private:
    QLabel *m_pName{nullptr};
    QPushButton *m_pAddBtn{nullptr};
    QListWidget *m_pSkills{nullptr};

    QVector<QLabel*> m_skillNames;
    QVector<QLabel*> m_attributeNames;
    QVector<SkillSpinBox*> m_skillValues;

    const QString m_style {
        "QWidget#OtherSkills{"
        " border: 1px solid black;"
        " border-radius: 5px;"
        " padding: 5px;"
        "}"
    };
};

#endif // NPCOTHERSKILLS_H
