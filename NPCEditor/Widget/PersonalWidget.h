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
    QLayout *createElement(const QJsonObject &obj);

private:
    QLabel *m_pPortrait{nullptr};
    QHash<QString, QWidget*> m_widgets;

    const QString m_portraitStyle{ "QLabel{"
                                   " border: 2px solid;"
                                   " border-radius: 5px;"
                                   "}" };
};

#endif // PERSONALWIDGET_H
