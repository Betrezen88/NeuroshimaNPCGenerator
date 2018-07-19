#ifndef ATTRIBUTEROW_H
#define ATTRIBUTEROW_H

#include <QWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QSpinBox>

class AttributeRow : public QWidget
{
public:
    AttributeRow(const QString &name, const QJsonArray& array);

private slots:
    void updateValues(const int& value);

private:
    void fillLabels();

private:
    const QJsonArray m_array;
    QSpinBox* m_value;
    QVector<QLabel*> m_labels;
};

#endif // ATTRIBUTEROW_H
