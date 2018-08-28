#ifndef ATTRIBUTEVALUEWIDGET_H
#define ATTRIBUTEVALUEWIDGET_H

#include "CardWidget.h"

class AttributeValueWidget : public CardWidget
{
public:
    AttributeValueWidget(const QString &name, const int &value, QWidget *parent = nullptr);

private:
    QLabel *m_pNameLabel{nullptr};
    QLabel *m_pValueLabel{nullptr};
    int m_value{0};

    const QString m_widgetStyle{"QWidget#ValueWidget{"
                                " border 1px solid black;"
                                " border-radius: 10px;"
                                " background-color: white;"
                                "}"};
    const QString m_labelStyle{"QLabel#Label{"
                               " font: 8px;"
                               " padding-bottom: 5px;"
                               "}"};
    const QString m_valueStyle{"QLabel#Value{"
                               " font: 12px;"
                               " margin: -5px;"
                               "}"};
};

#endif // ATTRIBUTEVALUEWIDGET_H
