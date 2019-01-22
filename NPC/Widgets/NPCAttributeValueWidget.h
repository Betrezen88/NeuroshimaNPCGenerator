#ifndef NPCATTRIBUTEVALUEWIDGET_H
#define NPCATTRIBUTEVALUEWIDGET_H

#include <QLabel>
#include <QWidget>

class NPCAttributeValueWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NPCAttributeValueWidget(const QString &modName, const int &modValue, QWidget *parent = nullptr);

signals:

public slots:
    void updateValueLabel(const int &value);

protected:
    void paintEvent(QPaintEvent *);

private:
    QLabel *m_pNameLabel{nullptr};
    QLabel *m_pValueLabel{nullptr};
    int m_modValue{0};

    const QString m_nameStyle{
       "QLabel#LabelStyle{"
       " font: 7px;"
       " padding-top: 3px;"
       "}"
    };
    const QString m_valueStyle{
       "QWidget#ValueStyle{"
       " font: solid 11px;"
       " border: 1px solid;"
       " border-radius: 10px;"
       " background-color: white;"
       "}"
    };
};

#endif // NPCATTRIBUTEVALUEWIDGET_H
