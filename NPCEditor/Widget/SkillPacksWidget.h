#ifndef SKILLPACKSWIDGET_H
#define SKILLPACKSWIDGET_H

#include "CardWidget.h"

#include <QWidget>
#include <QJsonArray>
#include <QHash>
#include <QLabel>
#include <QSpinBox>
#include <QGridLayout>

class SkillPacksWidget : public CardWidget
{
    Q_OBJECT
public:
    explicit SkillPacksWidget(const QString &name,
                              const QJsonArray &specialization,
                              const QJsonArray &skills,
                              QWidget *parent = nullptr);
private:
    QGridLayout *createSkillsLayout(const QJsonArray &skills);
    QString specLabel(const QJsonArray &specialization);

private:
    QLabel *m_pTitlePack{nullptr};
    QHash<QString, QSpinBox*> m_skills;
    const QString m_widgetStyle{ "QWidget#SkillPackWidget{"
                                 " border: 1px solid black;"
                                 " border-radius: 10px;"
                                 "}" };
    const QString m_skillPackLabelStyle{ "QLabel#SkillPackTitle{"
                                " font: 12px;"
                                " font-weight: bold;"
                                " padding-left: 5px;"
                                " padding-bottom: 5px;"
                                "}" };
};

#endif // SKILLPACKSWIDGET_H
