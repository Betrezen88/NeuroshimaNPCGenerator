#ifndef NPCATTRIBUTEMANAGERWIDGET_H
#define NPCATTRIBUTEMANAGERWIDGET_H

#include <QWidget>

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QHash>
#include <QPushButton>
#include <QSpinBox>

#include "../Utils/Dice.h"
#include "../Widgets/DragDropAreaWidget.h"

class NPCAttributeManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NPCAttributeManagerWidget(QWidget *parent = nullptr);

signals:

public slots:

private:
    QGroupBox *optionsBox();
    QGroupBox *attributesBox();
    QGroupBox *resulsBox();

private:
    QSpinBox *m_pRollCount{nullptr};
    QComboBox *m_pDistributeType{nullptr};
    QCheckBox *m_pExtraDice{nullptr};
    QPushButton *m_pThrowBtn{nullptr};

    QHash<QLabel*, DragDropAreaWidget*> m_results;

    const QStringList m_distributionTypes{
        "Dowolny",
        "Określony"
    };

    const QStringList m_attributes{
        "Budowa",
        "Zręczność",
        "Charakter",
        "Spryt",
        "Percepcja"
    };
};

#endif // NPCATTRIBUTEMANAGERWIDGET_H
