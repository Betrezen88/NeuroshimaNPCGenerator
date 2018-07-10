#include "SkillPackSection.h"

#include <QLabel>
#include <QSpinBox>
#include <QHBoxLayout>

#include <QDebug>

SkillPackSection::SkillPackSection(const QString &name, const QJsonDocument &json, QWidget *parent)
    : Section(json, parent)
{
    QHBoxLayout* all = new QHBoxLayout;
    all->addWidget( create(name) );

    setLayout( all );
}

QGroupBox *SkillPackSection::create(const QString &name)
{
    QGroupBox* groupBox = new QGroupBox( name );

    QGridLayout* skills = new QGridLayout;

    if ( !m_json.isEmpty() ) {
        QJsonArray skillsA = m_json.array();
        int row = 0;
        for( const QJsonValue& skill: skillsA ) {
            skills->addWidget( new QLabel(skill.toString()), row, 0 );
            skills->addWidget( new QSpinBox(this), row, 1 );
            ++row;
        }
    }

    groupBox->setLayout( skills );

    return groupBox;
}
