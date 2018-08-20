#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>
#include <QLabel>

class CardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CardWidget(const QString &title, QWidget *parent = nullptr);
    ~CardWidget() = default;

    void paintEvent(QPaintEvent *);

protected:
    QLabel* m_pTitle{nullptr};

    const QString m_titleStyle{ "QLabel#Title{"
                                " padding: 5px;"
                                " background-color: black;"
                                " color: white;"
                                " border-radius: 5px;"
                                " font: 20px;"
                                " qproperty-alignment: AlignCenter;"
                                "}" };
};

#endif // CARDWIDGET_H
