#ifndef NPCATTRIBUTEMANAGERDIALOG_H
#define NPCATTRIBUTEMANAGERDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QHBoxLayout>

#include "../Utils/Dice.h"
#include "../Widgets/DragDropAreaWidget.h"

class NPCAttributeManagerDialog : public QDialog
{
    Q_OBJECT
public:
    NPCAttributeManagerDialog(QWidget *parent = nullptr);

signals:
    void acceptAttributes(QVector<int> attributes);

private slots:
    void acceptBtnPressed();
    void addResultRow();
    void onResultChanged();

private:
    QWidget *createResultRowWidget();
    QGroupBox *createAttributeGroupBox();
    QGroupBox *createRollOptionsGroupBox();
    QGroupBox *createResultGroupBox();

    void createResultHash();
    int rollForAttribute();

    bool acceptBtnActive();

private:
    QPushButton *m_pRollBtn{nullptr};
    QPushButton *m_pAcceptBtn{nullptr};
    QPushButton *m_pCancelBtn{nullptr};
    QSpinBox *m_pRollCount{nullptr};
    QComboBox *m_pRollType{nullptr};
    QGroupBox *m_pResultGroupBox{nullptr};
    QHBoxLayout *m_pGroupBoxL{nullptr};

    Dice d20{20};

    QHash<const QString &, DragDropAreaWidget *> m_results;

    const QStringList m_rollTypes{
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

#endif // NPCATTRIBUTEMANAGERDIALOG_H
