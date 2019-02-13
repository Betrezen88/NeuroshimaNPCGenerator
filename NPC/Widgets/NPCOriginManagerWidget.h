#ifndef NPCORIGINMANAGERWIDGET_H
#define NPCORIGINMANAGERWIDGET_H

#include <QWidget>

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QJsonArray>

class NPCOriginManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NPCOriginManagerWidget(QWidget *parent = nullptr);

signals:

public slots:

private slots:
    void setOrigin(const QString &originName);
    void setFeature(const QJsonObject &feature);
    void setBonus(const QJsonObject &bonus);

private:
    QGroupBox *originDescriptionBox();
    void featuresBox();
    QGroupBox *featureDescriptionBox();
    QStringList origins();
    QStringList selectData(const QString &type, const QJsonArray &select);

private:
    QComboBox *m_pOrigin{nullptr};
    QLabel *m_pAttributeBonus{nullptr};
    QLabel *m_pOriginDescription{nullptr};
    QLabel *m_pFeatureDescription{nullptr};
    QGroupBox *m_pFeatureBox{nullptr};
    QGroupBox *m_pBonusBox{nullptr};
    QComboBox *m_pSelect{nullptr};

    QGridLayout *m_pLayout{nullptr};

    QJsonArray m_origins;
    QJsonArray m_features;
    QJsonArray m_attributes;
};

#endif // NPCORIGINMANAGERWIDGET_H
