#ifndef NPCSICKNESSMANAGERWIDGET_H
#define NPCSICKNESSMANAGERWIDGET_H

#include <QWidget>

#include <QGridLayout>
#include <QGroupBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>

class NPCSicknessManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NPCSicknessManagerWidget(QWidget *parent = nullptr);

signals:
    void sicknessChanged(const QJsonObject &sickness);

public slots:

private slots:
    void throwClicked();

private:
    void updateResults();
    QString createTooltip(const QJsonObject &object);

private:
    QSpinBox *m_pThrows{nullptr};
    QPushButton *m_pThrow{nullptr};
    QGroupBox *m_pResultBox{nullptr};
    QGridLayout *m_pLayout{nullptr};

    QJsonArray m_sickness;
};

#endif // NPCSICKNESSMANAGERWIDGET_H
