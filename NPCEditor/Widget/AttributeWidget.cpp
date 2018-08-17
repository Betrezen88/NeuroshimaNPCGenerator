#include "AttributeWidget.h"

#include <QVBoxLayout>

AttributeWidget::AttributeWidget(const QString &name, const QJsonArray &skillPacks, QWidget *parent)
    : QWidget(parent),
      m_pTitle(new AttributeTitleWidget(name, this))
{
    QVBoxLayout* all = new QVBoxLayout;
    all->addWidget( m_pTitle );

    setLayout( all );
}
