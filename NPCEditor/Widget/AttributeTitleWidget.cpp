#include "AttributeTitleWidget.h"

AttributeTitleWidget::AttributeTitleWidget(const QString &name, QWidget *parent)
    : QWidget(parent),
      m_pTitle(new QLabel(name, this))
{
    m_pTitle->setStyleSheet( m_labelStyle );
    setStyleSheet( m_widgetStyle );
}
