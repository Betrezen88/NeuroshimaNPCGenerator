#ifndef NPCTRICKSWIDGET_H
#define NPCTRICKSWIDGET_H

#include <QWidget>

#include <QListWidget>
#include <QPushButton>
#include <QJsonArray>

class NPCTricksWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NPCTricksWidget(QWidget *parent = nullptr);

signals:
    void clicked();

public slots:

private:
    bool fitRequirements(const QJsonArray &attributes, const QJsonArray &skills) const;

private:
    QListWidget *m_pTricks{nullptr};
    QPushButton *m_pManageBtn{nullptr};

    QJsonArray m_tricks;
};

#endif // NPCTRICKSWIDGET_H
