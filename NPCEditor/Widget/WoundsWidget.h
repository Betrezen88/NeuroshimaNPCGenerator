#ifndef WOUNDSWIDGET_H
#define WOUNDSWIDGET_H

#include "CardWidget.h"

#include <QJsonArray>
#include <QHBoxLayout>
#include <QHash>
#include <QSpinBox>

class WoundsWidget : public CardWidget
{
public:
    WoundsWidget(const QJsonArray &wounds, QWidget *parent = nullptr);

private:
    CardWidget *createWoundRow(const QJsonArray &wounds);

private:
    QHash<QString, QSpinBox*> m_wounds;

    const QString m_widgetStyle{ "QWidget#WoundsWidget{"
                                 " border: 1px solid black;"
                                 " border-radius: 10px;"
                                 "}" };
};

#endif // WOUNDSWIDGET_H
