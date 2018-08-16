#ifndef SECTION_H
#define SECTION_H

#include <QWidget>
#include <QVBoxLayout>
#include <QJsonObject>

class Section : public QWidget
{
    Q_OBJECT
public:
    explicit Section(QWidget *parent = nullptr): QWidget(parent) {}

protected:
    virtual QVBoxLayout *createLayout(const QJsonArray& json) = 0;

protected:
    const QString m_titleStyle{ "QLabel{"
                                " padding: 5px;"
                                " background-color: black;"
                                " color: white;"
                                " border-radius: 5px;"
                                " font: 20px;"
                                " qproperty-alignment: AlignCenter;"
                                "}" };
};

#endif // SECTION_H
