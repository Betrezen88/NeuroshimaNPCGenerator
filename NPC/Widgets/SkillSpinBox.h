#ifndef SKILLSPINBOX_H
#define SKILLSPINBOX_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class SkillSpinBox : public QWidget
{
    Q_OBJECT
public:
    explicit SkillSpinBox(QWidget *parent = nullptr);

    void setMinimum(const int &minValue);
    void setValue(const int &value);

signals:
    void skillValueChanged(const int &newValue, const bool &increase);

private slots:
    void skillUp();
    void skillDown();

private:
    QLineEdit *m_pValue{nullptr};
    QPushButton *m_pStepUp{nullptr};
    QPushButton *m_pStepDown{nullptr};

    int m_minValue{0};
};

#endif // SKILLSPINBOX_H
