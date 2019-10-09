#ifndef NPCSKILLPACKVIEW_H
#define NPCSKILLPACKVIEW_H

#include "NPCAbstractSkillpackView.h"

#include <QJsonArray>

class NPCSkillpackView : public NPCAbstractSkillpackView
{
public:
    explicit NPCSkillpackView(const QString &name,
                              const QStringList &specs,
                              const QStringList &skills,
                              QWidget *parent = nullptr);

    explicit NPCSkillpackView(const QString &name,
                              const QJsonArray &specs,
                              const QJsonArray &skills,
                              QWidget *parent = nullptr);

    QVector<QPair<QString, int> > skills() const override;
    bool hasSkill(const QString &name) const override;

public slots:
    void setSkillValue(const QString &name, const int &value) override;
    void setSkillValueBy(const QString &name, const int &value) override;

private:
    void addSkills(const QStringList &skills) override;
    void addSkills(const QJsonArray &skills);

private:
    QVector<QPair<QLabel*, QLabel*>> m_skills;
};

#endif // NPCSKILLPACKVIEW_H
