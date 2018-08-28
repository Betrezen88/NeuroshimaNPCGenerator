#include "SkillPacksWidget.h"

#include <QJsonObject>
#include <QJsonValue>

SkillPacksWidget::SkillPacksWidget(const QString &name,
                                   const QJsonArray &specialization,
                                   const QJsonArray &skills,
                                   QWidget *parent)
    : CardWidget("", parent)
{
    setObjectName("SkillPackWidget");
    setStyleSheet( m_widgetStyle );
    setFixedHeight( 115 );
    m_pTitlePack = new QLabel(name + specLabel(specialization), this);
    m_pTitlePack->setObjectName("SkillPackTitle");
    m_pTitlePack->setStyleSheet( m_skillPackLabelStyle );
    m_pTitlePack->setFixedHeight( 30 );

    QVBoxLayout* pAll = new QVBoxLayout;
    pAll->addWidget( m_pTitlePack );
    pAll->addLayout( createSkillsLayout(skills) );

    setLayout( pAll );
}

QGridLayout *SkillPacksWidget::createSkillsLayout(const QJsonArray &skills)
{
    QGridLayout *pLayout = new QGridLayout;

    int row = 1;
    for ( const QJsonValue &pack: skills ) {
        QLabel *pLabel = new QLabel(pack.toString(), this);
        QSpinBox *pSpinBox = new QSpinBox(this);
        m_skills.insert( pack.toString(), pSpinBox );
        pLayout->addWidget( pLabel, row, 0 );
        pLayout->addWidget( pSpinBox, row, 1 );
        ++row;
    }
    pLayout->setMargin( 0 );
    return pLayout;
}

QString SkillPacksWidget::specLabel(const QJsonArray &specialization)
{
    QString label = " (";
    for (int i=0; i<specialization.size(); ++i) {
        label += specialization.at(i).toString().at(0);
        if ( i != specialization.size()-1 )
            label += ",";
    }
    label += ")";
    return label;
}
