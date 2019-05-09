#ifndef NPCATTRIBUTEMANAGERWIDGET_H
#define NPCATTRIBUTEMANAGERWIDGET_H

#include <QWidget>

#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QJsonArray>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QRadioButton>

#include "../Utils/Dice.h"
#include "NPCAttributeResultsRow.h"

class NPCAttributeManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NPCAttributeManagerWidget(const QJsonArray *attributes, QWidget *parent = nullptr);

signals:
    void attributeChanged(const QString &name, const int &value);

public slots:

private slots:
    void throwBtnClicked();
    void distributeResults();

private:
    QGroupBox *optionsBox();
    QGroupBox *attributesBox();

private:
    const QJsonArray *m_attributes;
    QSpinBox *m_pRollCount{nullptr};
    QComboBox *m_pDistributeType{nullptr};
    QCheckBox *m_pExtraDice{nullptr};
    QPushButton *m_pThrowBtn{nullptr};
    QGroupBox *m_pResultBox{nullptr};

    QGridLayout *m_pMainLayout{nullptr};

    QVector<QLabel*> m_attributesNames;
    QVector<DragDropAreaWidget*> m_attributesValues;

    QVector<QRadioButton*> m_radioBtn;
    QVector<NPCAttributeResultsRow*> m_resultRows;

    const QStringList m_distributionTypes{
        "Dowolny",
        "Określony"
    };
};

#endif // NPCATTRIBUTEMANAGERWIDGET_H
