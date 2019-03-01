#ifndef NPCSKILLSMANAGERWIDGET_H
#define NPCSKILLSMANAGERWIDGET_H

#include <QWidget>
#include <QHash>
#include <QGroupBox>
#include <QLabel>
#include <QJsonObject>

#include "../Widgets/NPCAttributeWidget.h"

class NPCSkillsManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NPCSkillsManagerWidget(const QJsonArray *attributes, QWidget *parent = nullptr);

signals:
    void skillPointsValueChanged(const int &value);
    void availableSkillpointsValueChanged(const int &skill, const int &specs, const QString &spec);

public slots:
    void setAttributeValue(const QString &name, const int &value);
    void setAttributeModValue(const QString &name, const int &value);
    void setSpecialization(const QString &spec);
    void setProfessionBonus(const QJsonObject &bonus);
    void setBonusSkills(const QString &name, const int &value);

private slots:
    void buySkillPack(const bool &bougth, const QStringList &specs);
    void buySkill(const int &value, const QStringList &specs, const bool &increase);

private:
    void setAttributes(const QJsonArray &attributes);
    void addBonus(const QJsonObject &bonus);
    void removeBonus(const QJsonObject &bonus);

    QGroupBox *infoLabels();
    QVBoxLayout *columnA();
    QVBoxLayout *columnB();
    QVBoxLayout *columnC();

    void updateInfoLabels();
    void spendPoints(const int &value, const QStringList &specs);
    int calculateSkillCost(const int &value, const bool &increase);

private:
    QLabel *m_pSkillPointsLabel{nullptr};
    QLabel *m_pSpecPointsLabel{nullptr};

    QHash<const QString, NPCAttributeWidget*> m_attributes;

    QString m_specialization;
    QPair<int, int> m_specPoints{ 35, 0 };
    QPair<int, int> m_skillPoints{ 30, 0 };

    QJsonObject m_professionBonus;

    // TO DO: replace to load from jason
    const QVector<QPair<QString, int>> m_mods{
        QPair<QString, int>("Łat.", 2),
        QPair<QString, int>("Prze.", 0),
        QPair<QString, int>("Prob.", -2),
        QPair<QString, int>("Trud.", -5),
        QPair<QString, int>("B.Tr.", -8),
        QPair<QString, int>("Ch.T.", -11),
        QPair<QString, int>("Fuks", -15)
    };
};

#endif // NPCSKILLSMANAGERWIDGET_H
