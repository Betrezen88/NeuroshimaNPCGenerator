#include "Personal.h"

#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>

#include <QDebug>

Personal::Personal(const QJsonArray &json, QWidget *parent)
    : Section(parent),
      m_pPortrait( new QLabel("", this) ),
      m_pTitle( new QLabel("Postać", this) )
{
    m_pPortrait->setFixedWidth( 235 );
    m_pPortrait->setMinimumHeight( 150 );
    m_pPortrait->setStyleSheet( m_portraitStyle );
    m_pTitle->setStyleSheet( Section::m_titleStyle );
    m_pTitle->setFixedHeight( 50 );

    setLayout( createLayout(json) );
}

QVBoxLayout *Personal::createLayout(const QJsonArray &json)
{
    QVBoxLayout* pLayout = new QVBoxLayout;

    pLayout->addWidget( m_pPortrait );
    pLayout->addWidget( m_pTitle );

    for ( const QJsonValue &value: json ) {
        const QJsonObject &obj = value.toObject();
        pLayout->addLayout( createElement(obj) );
    }

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

QLayout *Personal::createElement(const QJsonObject &obj)
{
    const QString &key = obj.value("label").toString();
    const QString &type = obj.value("Type").toString();
    m_widgets.insert( key, createWidget(type) );
    QLayout *layout{nullptr};

    if ( "QComboBox" == type )
        layout = new QVBoxLayout;
    else
        layout = new QHBoxLayout;

    layout->addWidget( new QLabel(key) );
    layout->addWidget( m_widgets.value(key) );

    return layout;
}
