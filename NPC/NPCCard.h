#ifndef NPCCARD_H
#define NPCCARD_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QListWidget>
#include <QVarLengthArray>
#include <QVBoxLayout>
#include <QJsonArray>
#include <QJsonObject>
#include <QHash>

#include "Widgets/NPCAttributeWidget.h"
#include "Widgets/NPCProgressWidget.h"
#include "Widgets/NPCTricksWidget.h"

#include "Utils/Dice.h"

class NPCCard : public QWidget
{
    Q_OBJECT
public:
    explicit NPCCard(QWidget *parent = nullptr);

private slots:
    void onOriginChange(const QString &name);
    void onProfessionChanged(const QString &name);
    void onFeatureChanged(QComboBox *pFeature, const QJsonArray &features);
    void onRandomSicknessClicked();

private:
    void initCardData();
    QWidget *createPersonalSection();
    QWidget *createModificatorSection();
    QWidget *createModificatorWidget();
    QWidget *createWoundsSection();
    QWidget *createWoundsModificatorsSection();

    NPCSkillPackWidget *createSkillPack(const QJsonObject &skillPack);

    void fillAttributes(const QJsonArray &attributes);
    void fillSpecializations(const QJsonArray &specializations);
    void fillOrigins(const QJsonArray &origins);
    void fillProfessions(const QJsonArray &professions);

    QLabel *createLabel(const QString &text,
                        const QString &objName = "",
                        const QString &style = "",
                        const int &width = 0,
                        const int &heigth = 0,
                        Qt::Alignment aligment = Qt::AlignHCenter);

    QVBoxLayout *column1();
    QVBoxLayout *column2();
    QVBoxLayout *column3();
    QVBoxLayout *column4();

    QJsonArray loadJson(const QString &fileName);

private:
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

    NPCTricksWidget *m_pTricks{nullptr};
    NPCProgressWidget *m_pProgressWidget{nullptr};

    QPushButton *m_pRandomSicknessBtn{nullptr};

    QHash<QString, NPCAttributeWidget*> m_attributes;

    QHash<QString, QJsonObject> m_origins;
    QJsonArray m_originFeatures;
    QHash<QString, QJsonObject> m_professions;
    QJsonArray m_professionFeatures;
    QJsonArray m_sickness;

    Dice m_dice;

    const QStringList m_woundsList{ "Draśnięcia", "Lekkie", "Ciężkie", "Krytyczne" };
    const QStringList m_shortModsList{ "Łat.", "Prze.", "Prob.", "Trud.", "B.Tr.", "Ch.T.", "Fart" };
    const QVarLengthArray<int, 7> m_modsVals{ 2, 0, -2, -5, -8, -11, -15 };

    const QVector<QPair<QString, int>> m_mods{ QPair<QString, int>("Łat.", 2),
                                               QPair<QString, int>("Prze.", 0),
                                               QPair<QString, int>("Prob.", -2),
                                               QPair<QString, int>("Trud.", -5),
                                               QPair<QString, int>("B.Tr.", -8),
                                               QPair<QString, int>("Ch.T.", -11),
                                               QPair<QString, int>("Fuks", -15)};

    const QString m_titleStyle{ "QLabel#Title{"
                                " padding: 5px;"
                                " background-color: black;"
                                " color: white;"
                                " border-radius: 5px;"
                                " font: 20px;"
                                "}" };
    const QString m_modLabStyle{ "QLabel#ModLabel{"
                                 " font: 7px;"
                                 " padding-top: 3px;"
                                 "}" };
    const QString m_modTitle{ "QLabel#ModTitle{"
                              " font: 11px;"
                              " color: white;"
                              " background-color: black;"
                              " padding: 5px;"
                              "}" };
    const QString m_cellTitle{ "QLabel#CellTitle{"
                               " font: 9px;"
                               "}" };
    const QString m_cellValue{ "QLabel#CellValue{"
                               " font: solid 11px;"
                               " border: 1px solid;"
                               " border-radius: 50px;"
                               " background-color: white;"
                               "}" };
    const QString m_cellWidget{ "QWidget#CellWidget{"
                                " border: 1px solid;"
                                " background-color: gray;"
                                " padding: 3px;"
                                "}" };
    const QString m_valueWidget{ "QWidget#ValueWidget{"
                                 " border: 1px solid black;"
                                 " border-radius: 10px;"
                                 " background-color: white;"
                                 "}" };
    const QString m_attributeLabel{ "QLabel#AttributeName{"
                                    " font: bold 15px;"
                                    " padding-right: 5px;"
                                    "}" };
    const QString m_attributeTitle{ "QWidget#AttributeTitle{"
                                    " border: 1px solid;"
                                    " background-color: gray;"
                                    "}" };
    const QString m_skillPackTitle{ "QLabel#SkillPackTitle{"
                                    " font: bold 13px;"
                                    " padding-right: 5px;"
                                    "}" };
    const QString m_skillPackWidget{ "QWidget#SkillPackWidget{"
                                     " border: 1px solid black;"
                                     " border-radius: 5px;"
                                     "}" };
    const QString m_woundsWidget{ "QWidget#WoundsWidget{"
                                  " border: 1px solid;"
                                  " border-radius: 5px;"
                                  "}" };
    const QString m_woundsModificatorsWidget{ "QTextEdit#WoundsModificator{"
                                              " border: 1px solid;"
                                              " border-radius: 5px;"
                                              "}" };
    const QString m_progressWidget{ "QWidget#ProgressWidget{"
                                    " border: 1px solid;"
                                    " border-radius: 5px;"
                                    "}" };
};

#endif // NPCCARD_H
