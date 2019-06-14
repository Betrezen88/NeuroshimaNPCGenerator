#include "NPCItem.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

NPCItem::NPCItem(const QJsonObject &item, QWidget *parent)
    : NPCCustomWidget(parent),
      m_item( item ),
      m_pName(new QLabel(m_item.value("name").toString(), this)),
      m_pQuantity(new QLabel("1", this)),
      m_pReturnBtn(new QPushButton("Zwróć", this))
{
    connect( m_pReturnBtn, &QPushButton::clicked,
             [this](){
        emit this->returnItem(m_item.value("name").toString(), m_item.value("price").toInt());
        m_pQuantity->setText( QString::number(m_pQuantity->text().toInt()-1) );
        if ( m_pQuantity->text().toInt() == 0 )
            emit this->destroyItem( this );
    });

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->setSpacing( 1 );
    setLayout( pLayout );
    pLayout->addWidget( m_pName, 0, Qt::AlignLeft );
    pLayout->addSpacerItem( new QSpacerItem(1, 1, QSizePolicy::Maximum, QSizePolicy::MinimumExpanding) );
    pLayout->addWidget( m_pQuantity, 0, Qt::AlignRight );
    pLayout->addWidget( m_pReturnBtn, 0, Qt::AlignRight );
}

QString NPCItem::name() const
{
    return m_item.value("name").toString();
}

void NPCItem::increaseQuantity()
{
    m_pQuantity->setText( QString::number(m_pQuantity->text().toInt()+1) );
}
