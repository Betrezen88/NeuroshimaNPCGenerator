#include "PersonalWidget.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>

PersonalWidget::PersonalWidget(const QJsonArray &json, QWidget *parent)
    : CardWidget("Postać", parent),
      m_pPortrait(new QLabel("", this))
{
    m_pPortrait->setFixedWidth( 235 );
    m_pPortrait->setMinimumHeight( 150 );
    m_pPortrait->setStyleSheet( m_portraitStyle );

    setLayout( createLayout(json) );
}

QVBoxLayout *PersonalWidget::createLayout(const QJsonArray &json)
{
    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addWidget( m_pPortrait );
    pLayout->addWidget( m_pTitle );

    for ( const QJsonValue &value: json ) {
        const QJsonObject &obj = value.toObject();
        pLayout->addWidget( createElement(obj) );
    }

    pLayout->setSpacing( 2 );

    return pLayout;
}

QWidget *PersonalWidget::createWidget(const QString &type)
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

QWidget *PersonalWidget::createElement(const QJsonObject &obj)
{
    QWidget *pWidget = new QWidget(this);

    const QString &key = obj.value("label").toString();
    const QString &type = obj.value("Type").toString();
    m_widgets.insert( key, createWidget(type) );
    QLayout *pLayout{nullptr};

    if ( "QComboBox" == type )
        pLayout = new QVBoxLayout;
    else
        pLayout = new QHBoxLayout;

    pLayout->addWidget( new QLabel(key) );
    pLayout->addWidget( m_widgets.value(key) );

    pWidget->setObjectName( "PersonalItem" );
    pWidget->setStyleSheet( m_personalWidgetStyle );
    pWidget->setLayout( pLayout );

    return pWidget;
}
