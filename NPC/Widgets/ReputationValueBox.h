#ifndef REPUTATIONVALUEBOX_H
#define REPUTATIONVALUEBOX_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class ReputationValueBox : public QWidget
{
    Q_OBJECT
public:
    explicit ReputationValueBox(QWidget *parent = nullptr);

    int value() const;

signals:
    void valueUp();
    void valueDown();

public slots:
    void setValue(const int &value);
    void setUpDisable(const bool &disable);
    void setDownDisable(const bool &disable);
    void setMinimumValue(const int &min);
    void setMaximumValue(const int &max);

private slots:
    void upBtnClick();
    void downBtnClick();

private:
    QLineEdit *m_pValue{nullptr};
    QPushButton *m_pUpBtn{nullptr};
    QPushButton *m_pDownBtn{nullptr};

    int m_minimum{0};
    int m_maximum{0};
};

#endif // REPUTATIONVALUEBOX_H
