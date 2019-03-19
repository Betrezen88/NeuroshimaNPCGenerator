#ifndef NPCSKILLPACKVIEW_H
#define NPCSKILLPACKVIEW_H

#include <QWidget>
#include <QLabel>

#include <QGridLayout>

class NPCSkillpackView : public QWidget
{
    Q_OBJECT
public:
    explicit NPCSkillpackView(const QString &name,
                              QWidget *parent = nullptr);

    void addSpecialization(const QString &name);
    void addSkill(const QString &name);

    const QString name() const;

signals:
    void specsChanged();

public slots:
    void setSkillValue(const QString &name, const int &value);

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void updateSpecLabel();

private:
    QLabel *m_pName{nullptr};
    QLabel *m_pSpecs{nullptr};

    QGridLayout *m_pLayout{nullptr};

    QStringList m_specializations;
    QHash<QString, QLabel*> m_skills;

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

#endif // NPCSKILLPACKVIEW_H
