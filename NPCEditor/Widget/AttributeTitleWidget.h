#ifndef ATTRIBUTETITLEWIDGET_H
#define ATTRIBUTETITLEWIDGET_H

#include "CardWidget.h"

#include <QLabel>

#include <QHBoxLayout>

class AttributeTitleWidget : public CardWidget
{
    Q_OBJECT
public:
    explicit AttributeTitleWidget(const QString &name, QWidget *parent = nullptr);

    QHBoxLayout *createLayout();

private:
    QLabel *m_pTitleText{nullptr};
    const QString m_widgetStyle{ "QWidget#TitleWidget{"
                           " background-color: gray;"
                           " border: 1px solid;"
                           "}" };
    const QString m_labelStyle{ "QLabel#TitleLabel{"
                                " font: 15px;"
                                "}" };

    QStringList list{"Łat.", "Prze.", "Prob.", "Trud.", "B.Tr.", "Ch.T.", "Fart"};
};

#endif // ATTRIBUTETITLEWIDGET_H
