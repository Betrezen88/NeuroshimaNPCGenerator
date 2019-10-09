#ifndef NPCGENERALSKILLPACK_H
#define NPCGENERALSKILLPACK_H

#include "NPCAbstractSkillpackView.h"

#include <QVector>

class QComboBox;
class QLabel;

class NPCGeneralSkillpack : public NPCAbstractSkillpackView
{
public:
    explicit NPCGeneralSkillpack(const QString &name,
                                 const QStringList &specs,
                                 const QStringList &skills,
                                 QWidget *parent = nullptr);
    explicit NPCGeneralSkillpack(const QString &name,
                                 const QJsonArray &specs,
                                 const QJsonArray &skills,
                                 QWidget *parent = nullptr);

    QVector<QPair<QString, int> > skills() const override;
    bool hasSkill(const QString &name) const override;

public slots:
    void setSkillValue(const QString &name, const int &value) override;
    void setSkillValueBy(const QString &name, const int &value) override;

private slots:
    void checkSkillSelection(QComboBox *pSkill, const QString &name);

private:
    void addSkills(const QStringList &skills) override;

private:
    QVector<QPair<QComboBox*, QLabel*>> m_skills;
};

#endif // NPCGENERALSKILLPACK_H
