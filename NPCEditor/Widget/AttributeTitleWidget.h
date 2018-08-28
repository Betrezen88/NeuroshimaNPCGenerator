#ifndef ATTRIBUTETITLEWIDGET_H
#define ATTRIBUTETITLEWIDGET_H

#include "CardWidget.h"

#include <QLabel>
#include <QJsonArray>

#include <QHBoxLayout>

class AttributeTitleWidget : public CardWidget
{
    Q_OBJECT
public:
    explicit AttributeTitleWidget(const QString &name,
                                  const QJsonArray &modificators,
                                  QWidget *parent = nullptr);

    QHBoxLayout *createLayout(const QJsonArray &modificators);

private:
    QLabel *m_pTitleText{nullptr};
    const QString m_widgetStyle{ "QWidget#TitleWidget{"
                           " background-color: gray;"
                           " border: 1px solid;"
                           "}" };
    const QString m_labelStyle{ "QLabel#TitleLabel{"
                                " font: bold 15px;"
                                "}" };

    QStringList list{"Łat.", "Prze.", "Prob.", "Trud.", "B.Tr.", "Ch.T.", "Fart"};
};

#endif // ATTRIBUTETITLEWIDGET_H
