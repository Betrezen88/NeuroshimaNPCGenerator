#include "Section.h"

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>

Section::Section(const QJsonDocument &json, QWidget *parent)
    : m_json(json), QWidget(parent)
{

}

QWidget *Section::createWidget(const QString &type)
{
    if ( type == "QLineEdit" )
        return new QLineEdit( this );
    else if ( type == "QComboBox" )
        return new QComboBox( this );
    else if ( type == "QSpinBox" )
        return new QSpinBox( this );
    return new QWidget;
}
