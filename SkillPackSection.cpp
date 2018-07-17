#include "SkillPackSection.h"

#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QHBoxLayout>

#include <QDebug>

SkillPackSection::SkillPackSection(const QString &name, const QJsonDocument &json, QWidget *parent)
    : Section(json, parent)
{
    m_packBought = new QCheckBox( this );

    QHBoxLayout* all = new QHBoxLayout;
    all->addWidget( create(name) );

    connect( m_packBought, &QCheckBox::clicked, this, &SkillPackSection::packBought );

    setLayout( all );
}

void SkillPackSection::packBought(const bool &bougth)
{
    const int value = bougth ? 2 : -2;
    changeSkillsValue( value );
}

QGroupBox *SkillPackSection::create(const QString &name)
{
    QGroupBox* groupBox = new QGroupBox( name );

    QGridLayout* skills = new QGridLayout;

    skills->addWidget( new QLabel("Pakiet", this), 0, 0 );
    skills->addWidget( m_packBought, 0, 1 );

    if ( !m_json.isEmpty() ) {
        QJsonArray skillsA = m_json.array();
        int row = 1;
        for( const QJsonValue& skill: skillsA ) {
            skills->addWidget( new QLabel(skill.toString()), row, 0 );
            QSpinBox* spinBox = new QSpinBox( this );
            skills->addWidget( spinBox, row, 1 );
            m_skills.insert( skill.toString(), spinBox );
            ++row;
        }
    }

    groupBox->setLayout( skills );

    return groupBox;
}

void SkillPackSection::changeSkillsValue(const int &value)
{
    for( QSpinBox* skill: m_skills )
        skill->setValue( skill->value() + value );
}
