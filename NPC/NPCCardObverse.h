#ifndef NPCCARDOBVERSE_H
#define NPCCARDOBVERSE_H

#include <QWidget>

#include <QComboBox>
#include <QHash>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

#include "Widgets/NPCAttributeWidget.h"
#include "Widgets/NPCAttributesModsInfoWidget.h"
#include "Widgets/NPCProgressWidget.h"

class NPCCardObverse : public QWidget
{
    Q_OBJECT
public:
    explicit NPCCardObverse(QWidget *parent = nullptr);

    const QHash<QString, NPCAttributeWidget *> *attributes() const;
    const NPCProgressWidget *progressWidget() const;

signals:
    void heroNameChanged(const QString &name);

public slots:
    void onOriginChange(const QString &name);
    void onProfessionChanged(const QString &name);
    void onFeatureChanged(QComboBox *pFeature, const QJsonArray &features);
    void onAttributeChanged(QVector<int> attributes);

private:
    QLabel *createSpecialLabel(const QString &text,
                               const QString &objName = "",
                               const QString &style = "",
                               const int &width = 0,
                               const int &heigth = 0,
                               Qt::Alignment aligment = Qt::AlignHCenter);
    QWidget *createPersonalSection();

    NPCSkillPackWidget *createSkillPack(const QJsonObject &skillPack);

    void setAttributes(const QJsonArray &attributes);
    void setSpecializations(const QJsonArray &specializations);
    void setOrigins(const QJsonArray &origins);
    void setProfessions(const QJsonArray &professions);

    QVBoxLayout *column1();
    QVBoxLayout *column2();
    QVBoxLayout *column3();
    QVBoxLayout *column4();

private:
    // Personal Section Variables
    QLabel *m_pPortrait{nullptr};
    QLineEdit *m_pName{nullptr};
    QComboBox *m_pOrigin{nullptr};
    QComboBox *m_pProfession{nullptr};
    QComboBox *m_pSpecialization{nullptr};
    QLineEdit *m_pSickness{nullptr};
    QComboBox *m_pFeature1{nullptr};
    QComboBox *m_pFeature2{nullptr};
    QSpinBox *m_pReputation{nullptr};
    QSpinBox *m_pFame{nullptr};

    // Others widgets
    QListWidget *m_pTricks{nullptr};
    NPCProgressWidget *m_pProgressWidget{nullptr};
    NPCAttributesModsInfoWidget *m_pAttributesModsInfo{nullptr};

    // Data containers
    QHash<QString, NPCAttributeWidget*> m_attributes{};
    QHash<QString, QJsonObject> m_origins;
    QJsonArray m_originFeatures;
    QHash<QString, QJsonObject> m_professions;
    QJsonArray m_professionFeatures;
    QJsonArray m_attributesMods;

    // Data
    const QVector<QPair<QString, int>> m_mods{
        QPair<QString, int>("Łat.", 2),
        QPair<QString, int>("Prze.", 0),
        QPair<QString, int>("Prob.", -2),
        QPair<QString, int>("Trud.", -5),
        QPair<QString, int>("B.Tr.", -8),
        QPair<QString, int>("Ch.T.", -11),
        QPair<QString, int>("Fuks", -15)
    };

    // Styles
    const QString m_titleStyle{
        "QLabel#Title{"
        " padding: 5px;"
        " background-color: black;"
        " color: white;"
        " border-radius: 5px;"
        " font: 20px;"
        "}"
    };
};

#endif // NPCCARDOBVERSE_H
