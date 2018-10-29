#ifndef NPCTRICKWDGETITEM_H
#define NPCTRICKWDGETITEM_H

#include <QListWidgetItem>
#include <QJsonArray>
#include <QHash>

class NPCTrickWdgetItem : public QListWidgetItem
{
public:
    NPCTrickWdgetItem(const QString &name,
                      const QString &description,
                      const QString &action,
                      const QJsonArray &attributes,
                      const QJsonArray &skills,
                      const QJsonArray &orSkills);
    NPCTrickWdgetItem(const QString &name,
                      const QString &description,
                      const QString &action);
    ~NPCTrickWdgetItem()=default;

    const QHash<QString, int> *attributes() const;
    const QHash<QString, int> *skills() const;
    const QHash<QString, int> *orSkills() const;

    void addAttribute(const QString &name, const int &value);
    void addSkill(const QString &name, const int &value);
    void addOrSkill(const QString &name, const int &value);

    void createTooltipText();

private:
    QString toolTipText() const;

private:
    QHash<QString, int> m_attributes;
    QHash<QString, int> m_skills;
    QHash<QString, int> m_orSkills;

    QString m_description;
    QString m_action;
};

#endif // NPCTRICKWDGETITEM_H
