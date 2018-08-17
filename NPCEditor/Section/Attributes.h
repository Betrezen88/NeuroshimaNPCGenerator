#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#include "Section.h"

#include <QLabel>

class Attributes : public Section
{
public:
    Attributes(const QJsonArray &json, QWidget *parent = nullptr);
    ~Attributes() = default;

private:
    QVBoxLayout *createLayout(const QJsonArray &json);


private:
    QLabel *m_pTitle{nullptr};
};

#endif // ATTRIBUTES_H
