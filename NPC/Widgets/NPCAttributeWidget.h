#ifndef NPCATTRIBUTEWIDGET_H
#define NPCATTRIBUTEWIDGET_H

#include <QWidget>

#include <QLabel>
#include <QPair>
#include <QJsonArray>
#include <QHash>

#include "NPCSkillPackWidget.h"
#include "NPCAttributeValueWidget.h"

class NPCAttributeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NPCAttributeWidget(const QString &name,
                                const QVector<QPair<QString, int>> &mods,
                                QWidget *parent = nullptr);

    QHash<const QString, NPCSkillPackWidget *> *skillPacks();
    void addSkillPack(const QString &name, NPCSkillPackWidget *skillPack);
    const int *value() const;
    void setValue(const int value);

signals:
    void skillPackBougth(const bool &checked, const QStringList &specs);
    void valueChanged(const int &value);

public slots:

private:
    QWidget *createTitleBar(const QVector<QPair<QString, int>> &mods);
    QWidget *createValueWidget(const QString &name, const int &value);

private:
    QLabel* m_pName{nullptr};
    const QVector<QPair<QString, int>> &m_mods;
    int m_value{0};
    QHash<const QString, NPCSkillPackWidget*> m_skillPacks;

    const QString m_nameStyle{ "QLabel#NameStyle{"
                                    " font: bold 15px;"
                                    " padding-right: 5px;"
                                    "}" };
    const QString m_titleStyle{ "QWidget#TitleStyle{"
                           " border: 1px solid;"
                           " background-color: gray;"
                           "}" };
};

#endif // NPCATTRIBUTEWIDGET_H
