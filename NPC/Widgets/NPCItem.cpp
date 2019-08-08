#include "NPCItem.h"

#include <QAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QPushButton>

NPCItem::NPCItem(const QJsonObject &item, Type type, QWidget *parent)
    : NPCCustomWidget(parent),
      m_item( item ),
      m_type( type ),
      m_pName(new QLabel(m_item.value("name").toString(), this)),
      m_pQuantity(new QLabel("1", this)),
      m_pButton(new QPushButton("...", this))
{
    createActions();

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->setSpacing( 1 );
    setLayout( pLayout );
    pLayout->addWidget( m_pName, 0, Qt::AlignLeft );
    pLayout->addSpacerItem( new QSpacerItem(1, 1, QSizePolicy::Maximum, QSizePolicy::MinimumExpanding) );
    pLayout->addWidget( m_pQuantity, 0, Qt::AlignRight );
    pLayout->addWidget( m_pButton, 0, Qt::AlignRight );
}

QString NPCItem::name() const
{
    return m_item.value("name").toString();
}

NPCItem::Type NPCItem::type() const
{
    return m_type;
}

QJsonObject NPCItem::object() const
{
    return m_item;
}

int NPCItem::quantity() const
{
    return m_pQuantity->text().toInt();
}

void NPCItem::increaseQuantity()
{
    m_pQuantity->setText( QString::number(m_pQuantity->text().toInt()+1) );
}

void NPCItem::decreaseQuantity()
{
    m_pQuantity->setText( QString::number(m_pQuantity->text().toInt()-1) );
}

void NPCItem::setQuantity(const int quantity)
{
    m_pQuantity->setText( QString::number(quantity) );
}

void NPCItem::throwOut()
{
    decreaseQuantity();
    if ( 0 == m_pQuantity->text().toInt() )
        emit destroyItem( this );
}

void NPCItem::onEquipTrigger()
{
    emit equip( m_item );
    decreaseQuantity();
    if ( 0 == m_pQuantity->text().toInt() )
        emit destroyItem( this );
}

void NPCItem::setType(const NPCItem::Type &type)
{
    m_type = type;
    delete m_pButton->menu();
    createActions();
}

void NPCItem::createActions()
{
    QMenu *pMenu = new QMenu(this);

    if ( Type::SHOP == m_type ) {
        m_pReturn = new QAction("Zwróć", this);
        pMenu->addAction( m_pReturn );
        connect( m_pReturn, &QAction::triggered,
                 [this](){
            emit this->returnItem(m_item.value("name").toString(), m_item.value("price").toInt());
            this->decreaseQuantity();
            if ( 0 == m_pQuantity->text().toInt() )
                emit this->destroyItem( this );
        });
    }
    else {
        m_pThrowOut = new QAction("Wyrzuć", this);
        pMenu->addAction( m_pThrowOut );
        connect( m_pThrowOut, &QAction::triggered,
                 this, &NPCItem::throwOut );

        if ( "Broń biała" == m_item.value("type").toString()
             || "Broń miotana" == m_item.value("type").toString()
             || "Pistolet" == m_item.value("type").toString()
             || "Rewolwer" == m_item.value("type").toString()
             || "Pistolet maszynowy" == m_item.value("type").toString()
             || "Karabin powtarzalny" == m_item.value("type").toString()
             || "Karabin samopowtarzalny" == m_item.value("type").toString()
             || "Karabin automatyczny" == m_item.value("type").toString()
             || "Strzelba" == m_item.value("type").toString()
             || "Karabin maszynowy" == m_item.value("type").toString()
             || "Karabin snajperski" == m_item.value("type").toString()
             || "Granatnik" == m_item.value("type").toString()
             || "Broń przeciwpancerna" == m_item.value("type").toString()
             || "Miotacz ognia" == m_item.value("type").toString()
             || "Zbroja" == m_item.value("type").toString()
             || "Kamizelka" == m_item.value("type").toString()
             || "Hełm" == m_item.value("type").toString() ) {
            m_pEquip = new QAction("Wyekwipuj", this);
            pMenu->addAction(m_pEquip);
            connect( m_pEquip, &QAction::triggered,
                     this, &NPCItem::onEquipTrigger );
        }
        else {
            m_pUse = new QAction("Użyj", this);
            pMenu->addAction(m_pUse);
        }
    }
    m_pButton->setMenu(pMenu);
}
