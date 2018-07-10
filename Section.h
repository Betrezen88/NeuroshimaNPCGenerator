#ifndef SECTION_H
#define SECTION_H

#include <QWidget>
#include <QGroupBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

class Section : public QWidget
{
    Q_OBJECT
public:
    Section(const QJsonDocument& json, QWidget *parent = nullptr);

protected:
    virtual QGroupBox* create(const QString& name) = 0;
    QWidget* createWidget(const QString& type);

protected:
    const QJsonDocument& m_json;
};

#endif // SECTION_H
