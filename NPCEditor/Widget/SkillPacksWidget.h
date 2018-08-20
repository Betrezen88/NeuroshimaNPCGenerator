#ifndef SKILLPACKSWIDGET_H
#define SKILLPACKSWIDGET_H

#include <QWidget>
#include <QJsonArray>
#include <QHash>
#include <QLabel>
#include <QSpinBox>

class SkillPacksWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SkillPacksWidget(const QString &name,
                              const QJsonArray &skills,
                              QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);

private:
    QLabel *m_pTitle{nullptr};
    QHash<QString, QSpinBox*> m_skills;
    const QString m_widgetStyle{ "QWidget#A{"
                                 " border: 1px solid black;"
                                 " border-radius: 10px;"
                                 "}" };
    const QString m_titleStyle{ "QLabel{"
                                " font: 12px;"
                                " font-weight: bold;"
                                " padding-left: 5px;"
                                " padding-bottom: 5px;"
                                "}" };
};

#endif // SKILLPACKSWIDGET_H
