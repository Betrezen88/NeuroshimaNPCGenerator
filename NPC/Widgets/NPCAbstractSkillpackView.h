#ifndef NPCABSTRACTSKILLPACKVIEW_H
#define NPCABSTRACTSKILLPACKVIEW_H

#include "NPCCustomWidget.h"

#include <QVector>

class QLabel;
class QGridLayout;

class NPCAbstractSkillpackView : public NPCCustomWidget
{
public:
    NPCAbstractSkillpackView(const QString &name, const QStringList &specs, QWidget *parent = nullptr);

    QString name() const;
    QStringList specializations() const;

    virtual bool hasSkill(const QString &name) const = 0;
    virtual QVector<QPair<QString, int>> skills() const = 0;

public slots:
    virtual void setSkillValue(const QString &name, const int &value) = 0;
    virtual void setSkillValueBy(const QString &name, const int &value) = 0;

protected:
    void setSpecializationsLabel(const QStringList &specs);
    virtual void addSkills(const QStringList &skills) = 0;

protected:
    QLabel *m_pName{nullptr};
    QLabel *m_pSpecs{nullptr};
    QGridLayout *m_pLayout{nullptr};

    QStringList m_specializations;

    const QString m_titleStyle{
        "QLabel#Title{"
        " font: bold;"
        " font-size: 12em;"
        "}"
    };

    const QString m_valueStyle{
        "QLabel#Value{"
        " font: bold;"
        " padding-left: 3px;"
        "}"
    };

    const QString m_skillpackStyle{
        "QWidget#Skillpack{"
        " border: 1px solid black;"
        " border-radius: 5px;"
        "}"
    };
};

#endif // NPCABSTRACTSKILLPACKVIEW_H
