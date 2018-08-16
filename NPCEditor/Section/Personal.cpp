#include "Personal.h"

#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>

Personal::Personal(const QJsonArray &json, QWidget *parent)
    : Section(parent),
      m_pPortrait( new QLabel("", this) ),
      m_pTitle( new QLabel("Postać", this) )
{
    m_pPortrait->setFixedWidth( 235 );
    m_pPortrait->setMinimumHeight( 150 );
    m_pPortrait->setStyleSheet( m_portraitStyle );
    m_pTitle->setStyleSheet( Section::m_titleStyle );

    setLayout( createLayout(json) );
}

QVBoxLayout *Personal::createLayout(const QJsonArray &json)
{
    QVBoxLayout* pLayout = new QVBoxLayout;

    pLayout->addWidget( m_pPortrait );
    pLayout->addWidget( m_pTitle );

    return pLayout;
}

QWidget *Personal::createWidget(const QString &type)
{
    QWidget *widget{nullptr};

    if ( "QLineEdit" == type )
        widget = new QLineEdit(this);
    else if ( "QComboBox" == type )
        widget = new QComboBox(this);
    else if ( "QSpinBox" == type )
        widget = new QSpinBox(this);

    return widget;
}
