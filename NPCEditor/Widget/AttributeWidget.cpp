#include "AttributeWidget.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QVBoxLayout>

AttributeWidget::AttributeWidget(const QString &name, const QJsonArray &skillPacks, QWidget *parent)
    : QWidget(parent),
      m_pTitle(new AttributeTitleWidget(name, this))
{
    QVBoxLayout* all = new QVBoxLayout;
    all->addWidget( m_pTitle );

    for ( const QJsonValue &pack: skillPacks ) {
        const QJsonObject &obj = pack.toObject();
        const QString &name = obj.value("name").toString();
        const QJsonArray &skills = obj.value("skills").toArray();
        m_skillPacks.insert( name, new SkillPacksWidget(name, skills) );
        all->addWidget( m_skillPacks.value(name) );
    }

    setLayout( all );
}
