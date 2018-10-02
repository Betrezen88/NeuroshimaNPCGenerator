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
    explicit NPCSkillPackWidget(const QString &name, QWidget *parent = nullptr);

    void addSkill(const QString &name, SkillSpinBox *skillBox);
    bool hasSkill(const QString &name) const;
    SkillSpinBox *skill(const QString &name);
    QVector<SkillSpinBox *> skills();

    void addSpecialization(const QString &spec);
    QStringList specializations() const;

    void paintEvent(QPaintEvent *);

signals:
    void bougth(const bool &state, const QStringList &specs);
    void refundPoints(const int value, const QStringList &specs);

public slots:
    void onAvailableSkillPointsChanged(const int value);

private slots:
    void onBougth(const bool &checked);

private:
    const QString createSkillPackName(const QString &name, const QJsonArray &specs);

private:
    QLabel *m_pName{nullptr};
    QStringList m_specs;
    QVector<QPair<const QLabel*, SkillSpinBox*>> m_skills;
    QCheckBox *m_pBougth{nullptr};

    QGridLayout *m_pSkillLayout{nullptr};

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
