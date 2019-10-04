#ifndef NPCATTRIBUTEVIEW_H
#define NPCATTRIBUTEVIEW_H

#include <QWidget>
#include <QLabel>

#include <QVBoxLayout>
#include <QHBoxLayout>

class NPCAbstractSkillpackView;

class NPCAttributeView : public QWidget
{
    Q_OBJECT
public:
    explicit NPCAttributeView(const QString &name,
                              const int &value,
                              const int &modValue,
                              QWidget *parent = nullptr);

    void addSkillpack(NPCAbstractSkillpackView *skillpack);
    QHash<QString, NPCAbstractSkillpackView*> skillpacks() const;

    QString name() const;

    void setValue(const int &value);
    int value() const;

    void setModValue(const int &value);
    int modValue() const;

    int *currentValue();

signals:
    void currentValueChanged(const int &value);

public slots:
    void setSkillValue(const QString &skillpack, const QString &skill, const int &value);

private slots:
    void setCurrentValue(const int &value);

private:
    QVBoxLayout *pointsView();

private:
    QLabel *m_pName{nullptr};
    int m_value{0};
    int m_modValue{0};
    int m_currentValue{0};

    QHBoxLayout *m_pLayout{nullptr};
    QVBoxLayout *m_pSkillsLayout{nullptr};

    QHash<QString, NPCAbstractSkillpackView*> m_skillpacks;

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
