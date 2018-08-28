#ifndef PERSONALWIDGET_H
#define PERSONALWIDGET_H

#include "CardWidget.h"

#include <QJsonArray>
#include <QHash>

#include <QVBoxLayout>

class PersonalWidget : public CardWidget
{
    Q_OBJECT
public:
    explicit PersonalWidget(const QJsonArray &json, QWidget *parent = nullptr);
    ~PersonalWidget() = default;

private:
    QVBoxLayout *createLayout(const QJsonArray &json);
    QWidget *createWidget(const QString &type);
    QWidget *createElement(const QJsonObject &obj);

private:
    QLabel *m_pPortrait{nullptr};
    QHash<QString, QWidget*> m_widgets;

    const QString m_portraitStyle{ "QLabel{"
                                   " border: 2px solid;"
                                   " border-radius: 5px;"
                                   "}" };
    const QString m_personalWidgetStyle{ "QWidget#PersonalItem{"
                                         " border: 1px solid black;"
                                         " border-radius: 10px;"
                                         "}" };
};

#endif // PERSONALWIDGET_H
