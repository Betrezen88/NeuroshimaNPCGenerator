#include "AttributeWidget.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QVBoxLayout>

AttributeWidget::AttributeWidget(const QString &name, const QJsonArray &skillPacks, QWidget *parent)
    : CardWidget("", parent),
      m_pTitleBar(new AttributeTitleWidget(name, this))
{
    QVBoxLayout* all = new QVBoxLayout;
    all->addWidget( m_pTitleBar );

    for ( const QJsonValue &pack: skillPacks ) {
        const QJsonObject &obj = pack.toObject();
        const QString &name = obj.value("name").toString();
        const QJsonArray &skills = obj.value("skills").toArray();
        m_skillPacks.insert( name, new SkillPacksWidget(name, skills) );
        all->addWidget( m_skillPacks.value(name) );
    }
    all->setSpacing( 1 );

    setLayout( all );
}
