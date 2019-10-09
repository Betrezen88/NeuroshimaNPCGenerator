#include "NPCFriendObverse.h"
#include "NPCAttributeView.h"

#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>

NPCFriendObverse::NPCFriendObverse(const QJsonObject &personal,
                                   const QJsonArray &attributes,
                                   const QJsonArray &features,
                                   QWidget *parent)
    : NPCCustomWidget( parent ),
      m_pName( new QLabel(this) ),
      m_pOrigin( new QLabel(this) ),
      m_pProfession( new QLabel(this) ),
      m_pSickness( new QLabel(this) ),
      m_pConnection( new QLabel(this) ),
      m_personal( personal ),
      m_features( features )
{
    m_pName->setText( fullname() );
    m_pOrigin->setText( m_personal.value("origin").toString() );
    m_pSickness->setText( m_personal.value("sickness").toString() );
    m_pConnection->setText( m_personal.value("connection").toString() );
    m_pProfession->setText( m_personal.value("profession").toString() );

    setAttributes( attributes );

    setLayout( createLayout() );
}

QString NPCFriendObverse::fullname() const
{
    QString name = m_personal.value("name").toString();
    if ( !m_personal.value("nickname").toString().isEmpty() )
        name += " '"+m_personal.value("nickname").toString()+"'";
    if ( !m_personal.value("surname").toString().isEmpty() )
        name += " "+m_personal.value("surname").toString();
    return name;
}

void NPCFriendObverse::setAttributes(const QJsonArray &attributes)
{
    for ( const QJsonValue attribute: attributes ) {
        const QJsonObject &tAttribute = attribute.toObject();
        const QString &name = tAttribute.value("name").toString();
        m_attributes.insert( name,
                             new NPCAttributeView(name,
                                                  tAttribute.value("value").toInt(),
                                                  0,
                                                  tAttribute.value("skillpacks").toArray(),
                                                  this) );
    }
}

QHBoxLayout *NPCFriendObverse::createLayout()
{
    QHBoxLayout *pLayout = new QHBoxLayout;

    QVBoxLayout *pColumn1 = new QVBoxLayout;
    pColumn1->addWidget( personal() );
    pColumn1->addWidget( features() );
    pColumn1->addSpacerItem( new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding) );

    QVBoxLayout *pColumn2 = new QVBoxLayout;
    pColumn2->addWidget( m_attributes.value("Budowa") );
    pColumn2->addWidget( m_attributes.value("Zręczność") );
    pColumn2->addSpacerItem( new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding) );

    QVBoxLayout *pColumn3 = new QVBoxLayout;
    pColumn3->addWidget( m_attributes.value("Charakter") );
    pColumn3->addWidget( m_attributes.value("Percepcja") );
    pColumn3->addSpacerItem( new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding) );

    QVBoxLayout *pColumn4 = new QVBoxLayout;
    pColumn4->addWidget( m_attributes.value("Spryt") );
    pColumn4->addSpacerItem( new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding) );

    pLayout->addLayout( pColumn1 );
    pLayout->addLayout( pColumn2 );
    pLayout->addLayout( pColumn3 );
    pLayout->addLayout( pColumn4 );

    return pLayout;
}

QGroupBox *NPCFriendObverse::personal()
{
    QGroupBox *pGroupBox = new QGroupBox( "Personalia", this );

    QVBoxLayout *pLayout = new QVBoxLayout;
    pGroupBox->setLayout( pLayout );
    pLayout->addWidget( m_pName );
    pLayout->addWidget( m_pOrigin );
    pLayout->addWidget( m_pProfession );
    pLayout->addWidget( m_pSickness );
    pLayout->addWidget( m_pConnection );

    return pGroupBox;
}

QGroupBox *NPCFriendObverse::features()
{
    QGroupBox *pGroupBox = new QGroupBox( "Cechy", this );

    QVBoxLayout *pLayout = new QVBoxLayout;
    pGroupBox->setLayout( pLayout );

    for ( const QJsonValue feature: m_features ) {
        const QJsonObject &tFeature = feature.toObject();
        QLabel *pLabel = new QLabel(tFeature.value("name").toString());
        pLabel->setToolTip( tFeature.value("description").toString() );
        pLayout->addWidget( pLabel );
    }

    return pGroupBox;
}
