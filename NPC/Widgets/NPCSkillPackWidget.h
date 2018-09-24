#ifndef NPCSKILLPACKWIDGET_H
#define NPCSKILLPACKWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QPair>
#include <QVector>
#include <QJsonArray>
#include <QStringList>
#include <QGridLayout>

#include "SkillSpinBox.h"

class NPCSkillPackWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NPCSkillPackWidget(const QString &name,
                                const QJsonArray &skills,
                                const QJsonArray &specializations,
                                QWidget *parent = nullptr);

    bool hasSkill(const QString &name) const;
    SkillSpinBox *skill(const QString &name);

    void paintEvent(QPaintEvent *);

signals:
    void bougth(const bool &state, const QStringList &specs);

private slots:
    void onBougth(const bool &checked);

private:
    QGridLayout *createSkills(const QJsonArray &skills);
    const QString createSkillPackName(const QString &name, const QJsonArray &specs);
    void fillSpecs(const QJsonArray &specs);

private:
    QLabel *m_pName{nullptr};
    QStringList m_specs;
    QVector<QPair<const QLabel*, SkillSpinBox*>> m_skills;
    QCheckBox *m_pBougth{nullptr};

    const QString m_skillPackWidgetStyle{ "QWidget#SkillPack{"
                                          " border: 1px solid black;"
                                          " border-radius: 5px;"
                                          " padding: 5px;"
                                          "}" };

    const QString m_skillPackTitleStyle{ "QLabel#SkillPackTitle{"
                                         " padding-left: 5px;"
                                         " font: 12 bold;"
                                         "}" };
};

#endif // NPCSKILLPACKWIDGET_H
