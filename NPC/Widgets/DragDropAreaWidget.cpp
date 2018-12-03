#include "DragDropAreaWidget.h"

#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <QStyleOption>
#include <QWindow>

#include <QDebug>

DragDropAreaWidget::DragDropAreaWidget(QWidget *parent) : QWidget(parent)
{
    setAcceptDrops( true );
    setFixedSize( 30, 20 );
    setStyleSheet( m_style );
}

void DragDropAreaWidget::addLabel(const QString &text)
{
    m_pLabel = new QLabel( text, this );
    changeHasLabel( true );

    QPoint widgetCenter( width()/4, height()/4 );
    m_pLabel->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
    m_pLabel->move( widgetCenter );
    m_pLabel->show();
    m_pLabel->setAttribute( Qt::WA_DeleteOnClose );
}

int DragDropAreaWidget::value() const
{
    return m_pLabel->text().toInt();
}

bool DragDropAreaWidget::hasLabel() const
{
    return m_hasLabel;
}

void DragDropAreaWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if ( event->mimeData()->hasText() ) {
        if ( event->source() == this ) {
            event->setDropAction( Qt::MoveAction );
            event->accept();
        }
        else
            event->acceptProposedAction();
    }
    else {
        event->ignore();
    }
}

void DragDropAreaWidget::dropEvent(QDropEvent *event)
{
    if ( event->mimeData()->hasText() && !m_hasLabel ) {
        addLabel( event->mimeData()->text() );

        if ( event->source() == this ) {
            event->setDropAction( Qt::MoveAction );
            event->accept();
        }
        else
            event->acceptProposedAction();
    }
    else
        event->ignore();
}

void DragDropAreaWidget::mousePressEvent(QMouseEvent *event)
{
    QLabel *pLabel = qobject_cast<QLabel*>(childAt(event->pos()));

    if ( pLabel == nullptr )
        return;

    QMimeData *pMimeData = new QMimeData;
    pMimeData->setText( pLabel->text() );

    qreal dpr = window()->windowHandle()->devicePixelRatio();
    QPixmap pixmap( pLabel->size() * dpr );
    pixmap.setDevicePixelRatio( dpr );
    pLabel->render( &pixmap );

    QDrag *pDrag = new QDrag( this );
    pDrag->setMimeData( pMimeData );
    pDrag->setPixmap( pixmap );

    Qt::DropAction dropAction = pDrag->exec( Qt::MoveAction );

    if ( dropAction == Qt::MoveAction ) {
        pLabel->close();
        changeHasLabel( false );
    }
}

void DragDropAreaWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void DragDropAreaWidget::changeHasLabel(bool value)
{
    m_hasLabel = value;
    emit hasLabelChanged();
}
