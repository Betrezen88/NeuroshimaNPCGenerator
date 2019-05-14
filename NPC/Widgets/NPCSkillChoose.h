#ifndef NPCSKILLCHOOSE_H
#define NPCSKILLCHOOSE_H

#include <QWidget>

class QLineEdit;
class QLabel;
class QComboBox;
class QRadioButton;
class QPushButton;

class NPCSkillChoose : public QWidget
{
    Q_OBJECT
public:
    NPCSkillChoose(QWidget *parent = nullptr);

signals:
    void otherSkillChoosed(const QString name, const QString attribute);
    void closeWidget();

private slots:
    void setAttributeName(const QString &skillName);
    void okBtnClicked();

private:
    void loadData();

private:
    QLineEdit *m_pName{nullptr};
    QLabel *m_pAttribute{nullptr};
    QComboBox *m_pNames{nullptr};
    QComboBox *m_pAttributes{nullptr};
    QRadioButton *m_pChose{nullptr};
    QRadioButton *m_pAdd{nullptr};
    QPushButton *m_pOkBtn{nullptr};
    QPushButton *m_pCloseBtn{nullptr};

    QHash<QString, QString> m_skills;
    QStringList m_attributes;
};

#endif // NPCSKILLCHOOSE_H
