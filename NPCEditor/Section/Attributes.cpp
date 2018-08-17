#include "Attributes.h"

#include <QJsonObject>
#include <QJsonValue>

#include <QDebug>

Attributes::Attributes(const QJsonArray &json, QWidget *parent)
    : Section("Współczynniki i umiejętności", parent)
{
    setLayout( createLayout(json) );
}

QVBoxLayout *Attributes::createLayout(const QJsonArray &json)
{
    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addWidget( Section::m_pTitle );

    for ( const QJsonValue &value: json ) {
        const QJsonObject &attribute = value.toObject();
        const QString &name = attribute.value("name").toString();
        const QJsonArray &skillPacks = attribute.value("skillPacks").toArray();
        AttributeWidget *pAttributeWidget = new AttributeWidget(name, skillPacks, this);
        m_attributes.insert( name, pAttributeWidget );
        pLayout->addWidget( pAttributeWidget );
    }

    return pLayout;
}
