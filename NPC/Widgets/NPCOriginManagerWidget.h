#ifndef NPCORIGINMANAGERWIDGET_H
#define NPCORIGINMANAGERWIDGET_H

#include <QWidget>

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QJsonArray>
#include <QJsonObject>

#include "NPCSpecializationManagerWidget.h"

class NPCOriginManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NPCOriginManagerWidget(const NPCSpecializationManagerWidget *pSpecManager,
                                    QWidget *parent = nullptr);

signals:
    void originDataChanged(const QJsonObject &origin);
    void originNameChanged(const QString &name);
    void originFeatureChanged(const QString name, const QString description);
    void originBonusChanged(const QJsonObject &bonus);
    void attributeBonusChanged(const QString &name, const int &value);
    void bonusSkillChanged(const QStringList &names, const int &value);
    void addBonusTrick(const QString &name);
    void removeBonusTrick(const QString &name);
    void bonusSkillpointsChanged(const int &value);

public slots:
    void setOrigin(const QString &originName);

private slots:
    void setFeature(const QJsonObject &feature);
    void setBonus(const QJsonObject &bonus);
    void setSpecBonusLogic(const QString &name);
    void setBonusSkillPacks(const QString &name);

private:
    QGroupBox *originDescriptionBox();
    void attributeBonus(const QJsonObject &attribute);
    void featuresBox();
    QGroupBox *featureDescriptionBox();
    void bonusBox();
    QStringList origins();
    QStringList selectData(const QString &type, const QJsonArray &select);
    void bonusLogic(QJsonObject &bonus);
    void removeBonus(const QJsonObject &bonus);

private:
    const NPCSpecializationManagerWidget *m_pSpecManager;
    QComboBox *m_pOrigin{nullptr};
    QLabel *m_pOriginDescription{nullptr};
    QLabel *m_pFeatureDescription{nullptr};
    QLabel *m_pBonusDescription{nullptr};
    QGroupBox *m_pFeatureBox{nullptr};
    QGroupBox *m_pBonusBox{nullptr};
    QComboBox *m_pSelect{nullptr};

    QGridLayout *m_pLayout{nullptr};
    QVBoxLayout *m_pBonusLayout{nullptr};
    QWidget *m_pAttributeBonus{nullptr};

    QJsonArray m_origins;
    QJsonArray m_features;
    QJsonArray m_attributes;
    QJsonObject m_bonus;
};

#endif // NPCORIGINMANAGERWIDGET_H
