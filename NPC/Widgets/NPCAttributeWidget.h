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

    QHash<QString, NPCSkillPackWidget *> *skillPacks();
    const QHash<QString, NPCSkillPackWidget *> *skillPacks() const;
    void addSkillPack(const QString &name, NPCSkillPackWidget *skillPack);
    const int *value() const;
    const int *modValue() const;

signals:
    void skillPackBougth(const bool &checked, const QStringList &specs);
    void valueChanged(const int &value);
    void currentValueChanged(const int &value);

public slots:
    void setValue(const int &value);
    void setModValue(const int &modValue);

private:
    QWidget *createTitleBar(const QVector<QPair<QString, int>> &mods);
    QWidget *createValueWidget(const QString &name, const int &value);

private:
    QLabel* m_pName{nullptr};
    const QVector<QPair<QString, int>> &m_mods;
    QHash<QString, NPCSkillPackWidget*> m_skillPacks;

    int m_value{0};
    int m_modValue{0};

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
