#ifndef NPCATTRIBUTERESULTSROW_H
#define NPCATTRIBUTERESULTSROW_H

#include <QWidget>
#include <QVector>
#include <QHBoxLayout>

#include "DragDropAreaWidget.h"
#include "../Utils/Dice.h"

class NPCAttributeResultsRow : public QWidget
{
public:
    NPCAttributeResultsRow(const int &throws, QWidget *parent = nullptr);

private:
    void createRow();
    int roll();

private:
    int m_throws{0};
    QHBoxLayout *m_pLayout{nullptr};

    Dice d20{20};
    QVector<DragDropAreaWidget*> m_results;
};

#endif // NPCATTRIBUTERESULTSROW_H
