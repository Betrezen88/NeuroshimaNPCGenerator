#include "PersonalWidget.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>

#include <QDebug>

PersonalWidget::PersonalWidget(const QJsonArray &json, const QJsonArray &origins, QWidget *parent)
    : CardWidget("Postać", parent),
      m_pPortrait(new QLabel("", this))
{
    m_pPortrait->setFixedWidth( 235 );
    m_pPortrait->setMinimumHeight( 150 );
    m_pPortrait->setStyleSheet( m_portraitStyle );

    m_origins = origins;

    setLayout( createLayout(json) );

    connect( qobject_cast<QComboBox*>(m_widgets.value("Pochodzenie")),
             &QComboBox::currentTextChanged,
             this,
             &PersonalWidget::onOriginChanged);

    connect( qobject_cast<QComboBox*>(m_widgets.value("Cecha z pochodzenia")),
             &QComboBox::currentTextChanged,
             this,
             &PersonalWidget::onOriginFeatureChanged);

    loadData( origins );
}

void PersonalWidget::onOriginChanged(const QString &originName)
{
    m_features.clear();
    const QJsonObject originObj = origin(originName);
    const QJsonArray &features = originObj.value("features").toArray();

    for ( const QJsonValue &feature: features ) {
        const QJsonObject &obj = feature.toObject();
        const QString &name = obj.value("name").toString();
        const QString &description = obj.value("description").toString();
        m_features.insert( name, description );
    }

    qobject_cast<QComboBox*>(m_widgets.value("Cecha z pochodzenia"))->clear();
    qobject_cast<QComboBox*>(m_widgets.value("Cecha z pochodzenia"))->insertItems(0, m_features.keys());
}

void PersonalWidget::onOriginFeatureChanged(const QString &feature)
{
    qobject_cast<QComboBox*>(m_widgets.value("Cecha z pochodzenia"))->setToolTip( m_features.value(feature) );
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

void PersonalWidget::loadData(const QJsonArray &origins)
{
    QStringList originsNames;

    for ( const QJsonValue &origin: origins )
        originsNames << origin.toObject().value("name").toString();

    qobject_cast<QComboBox*>(m_widgets.value("Pochodzenie"))->insertItems(0, originsNames);
}

const QJsonObject PersonalWidget::origin(const QString &name) const
{
    QJsonObject objOrigin;
    for ( const QJsonValue &jOrigin: m_origins ) {
        const QJsonObject &obj = jOrigin.toObject();
        if ( name == obj.value("name").toString() ) {
            objOrigin = obj;
            break;
        }
    }
    return objOrigin;
}
