#ifndef NPCPROFESSIONMANAGERWIDGET_H
#define NPCPROFESSIONMANAGERWIDGET_H

#include <QWidget>

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QJsonArray>
#include <QJsonObject>

class NPCProfessionManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NPCProfessionManagerWidget(QWidget *parent = nullptr);

signals:
    void professionBonusChanged(const QJsonObject &bonus);
    void professionNameChanged(const QString &name);
    void professionFeatureChanged(const QString &feature, const QString &description);
    void bonusSkillChanged(const QStringList &names, const int &value);

public slots:
    void setProfession(const QString &professionName);

private slots:
    void setFeature(const QJsonObject &feature);
    void setBonus(const QJsonObject &bonus);

private:
    QStringList professions();
    QGroupBox *professionDescriptionBox();
    void featureBox();
    QGroupBox *featureDescriptionBox();
    void bonusLogic(QJsonObject &bonus);
    void removeBonus(const QJsonObject &bonus);

private:
    QComboBox *m_pProfession{nullptr};
    QLabel *m_pProfessionDescription{nullptr};
    QLabel *m_pFeatureDescription{nullptr};
    QLabel *m_pBonusDescription{nullptr};
    QGroupBox *m_pFeatureBox{nullptr};
    QGroupBox *m_pBonusBox{nullptr};

    QGridLayout *m_pLayout{nullptr};
    QVBoxLayout *m_pBonusLayout{nullptr};

    QJsonArray m_professions;
    QJsonArray m_features;
    QJsonObject m_bonus;
};

#endif // NPCPROFESSIONMANAGERWIDGET_H
