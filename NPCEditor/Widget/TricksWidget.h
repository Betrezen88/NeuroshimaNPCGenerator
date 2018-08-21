#ifndef TRICKSWIDGET_H
#define TRICKSWIDGET_H

#include "CardWidget.h"

#include <QListWidget>
#include <QPushButton>

class TricksWidget : public CardWidget
{
public:
    TricksWidget(QWidget* parent = nullptr);

private:
    QListWidget *m_pTricksList{nullptr};
    QPushButton *m_pAddBtn{nullptr};
    QPushButton *m_pRemoveBtn{nullptr};
};

#endif // TRICKSWIDGET_H
