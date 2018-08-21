#ifndef TRICKSWIDGET_H
#define TRICKSWIDGET_H

#include "CardWidget.h"

#include <QListWidget>

class TricksWidget : public CardWidget
{
public:
    TricksWidget(QWidget* parent = nullptr);

private:
    QListWidget *m_pTricksList{nullptr};
};

#endif // TRICKSWIDGET_H
