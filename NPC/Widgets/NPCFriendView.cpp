#include "NPCFriendView.h"
#include "NPCAttributeView.h"

#include <QLabel>
#include <QGridLayout>

NPCFriendView::NPCFriendView(const QJsonObject &pal, QWidget *parent)
    : NPCCustomWidget(parent),
      m_pName( new QLabel(this) ),
      m_pOrigin( new QLabel(this) ),
      m_pProfession( new QLabel(this) ),
      m_pSickness( new QLabel(this) )
{
    const QJsonObject &personal = pal.value("personal").toObject();
    setName( personal );
    m_pOrigin->setText( personal.value("origin").toString() );
    m_pSickness->setText( personal.value("sickness").toString() );
    m_pProfession->setText( personal.value("profession").toString() );

    QGridLayout *pLayout = new QGridLayout;
    setLayout( pLayout );

    pLayout->addWidget( new QLabel("Imię:", this), 0, 0 );
    pLayout->addWidget( m_pName, 0, 1 );
    pLayout->addWidget( new QLabel("Profesja:", this), 0, 2 );
    pLayout->addWidget( m_pProfession, 0, 3 );
    pLayout->addWidget( new QLabel("Pochodzenie:", this), 1, 0 );
    pLayout->addWidget( m_pOrigin, 1, 1 );
    pLayout->addWidget( new QLabel("Choroba:", this), 1, 2 );
    pLayout->addWidget( m_pSickness, 1, 3 );


}

QString NPCFriendView::name() const
{
    return m_pName->text();
}

void NPCFriendView::setName(const QJsonObject &personal)
{
    QString text = personal.value("name").toString();
    if ( !personal.value("nickname").toString().isEmpty() )
        text += " '" + personal.value("nickname").toString() + "'";
    text += " " + personal.value("surname").toString();

    m_pName->setText( text );
}
