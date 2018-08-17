#ifndef SECTION_H
#define SECTION_H

#include <QWidget>
#include <QVBoxLayout>
#include <QJsonArray>
#include <QLabel>

class Section : public QWidget
{
    Q_OBJECT
public:
    explicit Section(const QString &title, QWidget *parent = nullptr)
        : QWidget(parent),
          m_pTitle(new QLabel(title, this))
    {
        m_pTitle->setStyleSheet( m_titleStyle );
        m_pTitle->setFixedHeight( 50 );
    }

protected:
    virtual QVBoxLayout *createLayout(const QJsonArray& json) = 0;

protected:
    QLabel* m_pTitle{nullptr};
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
