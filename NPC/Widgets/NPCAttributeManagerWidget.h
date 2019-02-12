#ifndef NPCATTRIBUTEMANAGERWIDGET_H
#define NPCATTRIBUTEMANAGERWIDGET_H

#include <QWidget>

#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
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
    void attributesChanged(QHash<QString,int> attributes);

public slots:

private slots:
    void throwBtnClicked();

private:
    QGroupBox *optionsBox();
    QGroupBox *attributesBox();

    QWidget *createResultRowWidget();

    int roll();

private:
    QSpinBox *m_pRollCount{nullptr};
    QComboBox *m_pDistributeType{nullptr};
    QCheckBox *m_pExtraDice{nullptr};
    QPushButton *m_pThrowBtn{nullptr};
    QGroupBox *m_pResultBox{nullptr};

    QGridLayout *m_pMainLayout{nullptr};

    QHash<QLabel*, DragDropAreaWidget*> m_results;

    Dice d20{20};

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
