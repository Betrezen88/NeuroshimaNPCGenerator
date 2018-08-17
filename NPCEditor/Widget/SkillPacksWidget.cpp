#include "SkillPacksWidget.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QVBoxLayout>

SkillPacksWidget::SkillPacksWidget(const QString &name, const QJsonArray &skills, QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* all = new QVBoxLayout;

    all->addWidget( new QLabel(name, this) );

    for ( const QJsonValue &pack: skills ) {
        QLabel *pLabel = new QLabel(pack.toString(), this);
        all->addWidget( pLabel );
    }

    setLayout( all );
}
