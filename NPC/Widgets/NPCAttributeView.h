#ifndef NPCATTRIBUTEVIEW_H
#define NPCATTRIBUTEVIEW_H

#include <QWidget>
#include <QLabel>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include "NPCSkillpackView.h"

class NPCAttributeView : public QWidget
{
    Q_OBJECT
public:
    explicit NPCAttributeView(const QString &name,
                              const int &value,
                              const int &modValue,
                              QWidget *parent = nullptr);

    void addSkillpack(NPCSkillpackView *skillpack);

    void setValue(const int &value);
    int value() const;

    void setModValue(const int &value);
    int modValue() const;

signals:
    void currentValueChanged(const int &value);

public slots:

private:
    QVBoxLayout *pointsView();

private:
    QLabel *m_pName{nullptr};
    int m_value{0};
    int m_modValue{0};

    QHBoxLayout *m_pLayout{nullptr};
    QVBoxLayout *m_pSkillsLayout{nullptr};

    QHash<const QString&, NPCSkillpackView*> m_skillpacks;

    const QString m_titleStyle{
        "QLabel#Title{"
        " font: bold italic 18px;"
        " color: white;"
        " background-color: black;"
        " padding: 5px;"
        " border-radius: 5px;"
        "}"
    };
};

#endif // NPCATTRIBUTEVIEW_H
