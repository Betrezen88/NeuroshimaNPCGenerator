#ifndef PERSONALSECTION_H
#define PERSONALSECTION_H

#include "Section.h"

#include <QMap>

class PersonalSection : public Section
{
public:
    PersonalSection(const QString& name, const QJsonDocument& json, QWidget* parent=0);

    QGroupBox* create(const QString &name);

    QWidget* value(const QString& name);

private:
    QMap<QString, QWidget*> m_values;
};

#endif // PERSONALSECTION_H
