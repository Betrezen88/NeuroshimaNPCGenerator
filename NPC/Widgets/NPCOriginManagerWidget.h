#ifndef NPCORIGINMANAGERWIDGET_H
#define NPCORIGINMANAGERWIDGET_H

#include <QWidget>

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QJsonArray>
#include <QJsonObject>

class NPCOriginManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NPCOriginManagerWidget(QWidget *parent = nullptr);

signals:
    void originDataChanged(const QJsonObject &origin);
    void originNameChanged(const QString &name);
    void originFeatureChanged(const QString name, const QString description);
    void originBonusChanged(const QJsonObject &bonus);
    void attributeBonusChanged(const QString &name, const int &value);
    void bonusSkillChanged(const QStringList &names, const int &value);

public slots:
    void setOrigin(const QString &originName);

private slots:
    void setFeature(const QJsonObject &feature);
    void setBonus(const QJsonObject &bonus);

private:
    QGroupBox *originDescriptionBox();
    void featuresBox();
    QGroupBox *featureDescriptionBox();
    void bonusBox();
    QStringList origins();
    QStringList selectData(const QString &type, const QJsonArray &select);
    void bonusLogic(QJsonObject &bonus);
    void removeBonus(const QJsonObject &bonus);

private:
    QComboBox *m_pOrigin{nullptr};
    QLabel *m_pAttributeBonus{nullptr};
    QLabel *m_pOriginDescription{nullptr};
    QLabel *m_pFeatureDescription{nullptr};
    QLabel *m_pBonusDescription{nullptr};
    QGroupBox *m_pFeatureBox{nullptr};
    QGroupBox *m_pBonusBox{nullptr};
    QComboBox *m_pSelect{nullptr};

    QGridLayout *m_pLayout{nullptr};
    QVBoxLayout *m_pBonusLayout{nullptr};

    QJsonArray m_origins;
    QJsonArray m_features;
    QJsonArray m_attributes;
    QJsonObject m_bonus;
};

#endif // NPCORIGINMANAGERWIDGET_H
