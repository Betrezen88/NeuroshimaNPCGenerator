#ifndef NPCABSTRACTSKILLPACK_H
#define NPCABSTRACTSKILLPACK_H


#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QVector>

#include "NPCCustomWidget.h"
#include "SkillSpinBox.h"

class NPCAbstractSkillPack : public NPCCustomWidget
{
    Q_OBJECT
public:
    NPCAbstractSkillPack(const QString &name,
                         const QStringList &specs,
                         QWidget *parent = nullptr);

    QStringList specializations() const;

    bool isBougth() const;

    SkillSpinBox *skill(const QString &name) const;

    virtual void addSkills(const QStringList &skills) = 0;
    virtual bool hasSkill(const QString &name) const = 0;

signals:
    void bougth(const bool &state, const QStringList &specs);
    void refundPoints(const int value, const QStringList &specs);
    void onSpecsChanged(const QStringList &specs);
    void skillValueChanged(const int &value, const QStringList &specs, const bool &increase);

public slots:
    void onAvailableSkillPointsChanged(const int &skill, const int &specs, const QString &spec);

protected slots:
    virtual void onBougth(const bool &checked) = 0;
    virtual void enableBougthCheckbox() = 0;

protected:
    void createTitleLabel(const QString &name, const QStringList &specs);

protected:
    QString m_name;
    QStringList m_specs;
    QLabel *m_pName{nullptr};
    QCheckBox *m_pBougth{nullptr};
    QGridLayout *m_pSkillsLayout{nullptr};
    QVector<SkillSpinBox*> m_skillValues;

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

#endif // NPCABSTRACTSKILLPACK_H
