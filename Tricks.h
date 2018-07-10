#ifndef TRICKS_H
#define TRICKS_H

#include "Section.h"

#include <QListWidget>

class Tricks : public Section
{
    Q_OBJECT
public:
    explicit Tricks(const QString& name, const QJsonDocument& json, QWidget *parent = nullptr);

    QGroupBox* create(const QString &name);

private:
    QListWidget* m_tricks;
};

#endif // TRICKS_H
