#ifndef NPCTRICKSWIDGET_H
#define NPCTRICKSWIDGET_H

#include <QWidget>

#include <QListWidget>
#include <QPushButton>

class NPCTricksWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NPCTricksWidget(QWidget *parent = nullptr);

signals:

public slots:

private:
    QListWidget *m_pTricks{nullptr};
    QPushButton *m_pBuyBtn{nullptr};
    QPushButton *m_pSellBtn{nullptr};
};

#endif // NPCTRICKSWIDGET_H
