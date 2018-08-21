#include "CardWidget.h"

#include <QStyleOption>
#include <QPainter>

CardWidget::CardWidget(const QString &title, QWidget *parent)
    : QWidget(parent)
{
    if ( !title.isEmpty() ) {
        m_pTitle = new QLabel(title, this);
        m_pTitle->setObjectName( "Title" );
        m_pTitle->setFixedHeight( 50 );
        m_pTitle->setStyleSheet( m_titleStyle );
    }
}

void CardWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
