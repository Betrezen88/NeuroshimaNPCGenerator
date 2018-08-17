#ifndef ATTRIBUTETITLEWIDGET_H
#define ATTRIBUTETITLEWIDGET_H

#include <QWidget>
#include <QLabel>

class AttributeTitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AttributeTitleWidget(const QString &name, QWidget *parent = nullptr);

private:
    QLabel *m_pTitle{nullptr};
    const QString m_widgetStyle{ "QWidget{"
                           " background-color: gray;"
                           " border: 1px solid;"
                           "}" };
    const QString m_labelStyle{ "QLabel{"
                                " font: 15px;"
                                "}" };
};

#endif // ATTRIBUTETITLEWIDGET_H
