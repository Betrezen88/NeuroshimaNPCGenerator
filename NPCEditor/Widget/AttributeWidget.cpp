#include "AttributeWidget.h"

#include <QLabel>
#include <QVBoxLayout>

AttributeWidget::AttributeWidget(const QString &name, const QJsonArray &skillPacks, QWidget *parent) : QWidget(parent)
{
    QLabel *pLabel = new QLabel(name, this);

    QVBoxLayout* all = new QVBoxLayout;
    all->addWidget( pLabel );

    setLayout( all );
}
