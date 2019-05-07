#ifndef NPCCUSTOMWIDGET_H
#define NPCCUSTOMWIDGET_H

#include <QWidget>

#include <QStyleOption>
#include <QPainter>

class NPCCustomWidget : public QWidget
{
public:
    NPCCustomWidget(QWidget *parent = nullptr) : QWidget (parent) {}

    void paintEvent(QPaintEvent *)
    {
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
};

#endif // NPCCUSTOMWIDGET_H
