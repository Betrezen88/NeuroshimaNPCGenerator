#ifndef NPCORIGINMANAGERDIALOG_H
#define NPCORIGINMANAGERDIALOG_H

#include <QComboBox>
#include <QDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QHash>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QPushButton>

#include "NPC/NPCCardObverse.h"

class NPCOriginManagerDialog : public QDialog
{
    Q_OBJECT
public:
    NPCOriginManagerDialog(const NPCCardObverse *card, QWidget *parent = nullptr);

signals:
    void acceptOrigin(const QJsonObject origin);

private slots:
    void onOriginChanged(const QString &origin);
    void onFeatureChanged(const QJsonObject &feature);
    void onAcceptBtnClicked();

private:
    void setOrigins(const QJsonArray &origins);
    void setBonus(const QJsonObject &bonus);
    void setOriginDescription(const QString &description);

    QJsonObject originJson();

    void createSimpleBonus(const QJsonObject &bonus);
    void createComplexBonus(const QJsonObject &bonus);
    QLabel *createBonusLabel(const QString &text);

    QStringList getData(const QJsonObject &bonus);

    QStringList skillpacksBySpec(const QString &spec);
    QStringList skillpacksByAttribute(const QString &attribute);

private:
    const NPCCardObverse *m_pCard;
    QComboBox *m_pOrigin{nullptr};
    QPushButton *m_pCancelBtn{nullptr};
    QPushButton *m_pAcceptBtn{nullptr};

    QGroupBox *m_pFeatureBox{nullptr};
    QGroupBox *m_pDescriptionBox{nullptr};
    QGroupBox *m_pBonusBox{nullptr};
    QGroupBox *m_pFeatureDescriptionBox{nullptr};

    QLabel *m_pAttributeBonus{nullptr};

    QGridLayout *m_pLayout{nullptr};

    QComboBox *m_pComplex{nullptr};

    QHash<const QString, QJsonObject> m_origins;
    QJsonObject m_feature;
};

#endif // NPCORIGINMANAGERDIALOG_H
