#include "Attributes.h"

#include <QDebug>

Attributes::Attributes(const QJsonArray &json, QWidget *parent)
    : Section("Współczynniki i umiejętności", parent)
{
    setLayout( createLayout(json) );
}

QVBoxLayout *Attributes::createLayout(const QJsonArray &json)
{
    QVBoxLayout *pLayout = new QVBoxLayout;

    qDebug() << "Array count: " << json.count();

    pLayout->addWidget( Section::m_pTitle );

    return pLayout;
}
