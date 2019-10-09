#include "NPCAbstractSkillpackView.h"

#include <QJsonValue>
#include <QJsonObject>
#include <QGridLayout>
#include <QLabel>

NPCAbstractSkillpackView::NPCAbstractSkillpackView(const QString &name, const QStringList &specs, QWidget *parent)
    : NPCCustomWidget( parent ),
      m_pName( new QLabel(name, this) ),
      m_pSpecs( new QLabel(this) ),
      m_pLayout( new QGridLayout ),
      m_specializations( specs )
{
    init();
}

NPCAbstractSkillpackView::NPCAbstractSkillpackView(const QString &name, const QJsonArray &specs, QWidget *parent)
    : NPCCustomWidget( parent ),
      m_pName( new QLabel(name, this) ),
      m_pSpecs( new QLabel(this) ),
      m_pLayout( new QGridLayout )
{
    for ( const QJsonValue spec: specs )
        m_specializations << spec.toString();

    init();
}

QString NPCAbstractSkillpackView::name() const
{
    return m_pName->text();
}

QStringList NPCAbstractSkillpackView::specializations() const
{
    return m_specializations;
}

void NPCAbstractSkillpackView::init()
{
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );
    setLayout( m_pLayout );
    setSpecializationsLabel( m_specializations );
    m_pName->setObjectName( "Title" );
    m_pSpecs->setObjectName( "Title" );
    m_pName->setStyleSheet( m_titleStyle );
    m_pSpecs->setStyleSheet( m_titleStyle );

    QHBoxLayout *pTitleRow = new QHBoxLayout;
    pTitleRow->addWidget( m_pName );
    pTitleRow->addWidget( m_pSpecs, 0, Qt::AlignRight );

    m_pLayout->addLayout( pTitleRow, 0, 0 );
}

void NPCAbstractSkillpackView::setSpecializationsLabel(const QStringList &specs)
{
    QString text{"("};
    for ( const QString &spec: specs ) {
        text += spec.at(0);
        if ( spec != specs.last() )
            text += ", ";
    }
    text += ")";
    m_pSpecs->setText( text );
}
