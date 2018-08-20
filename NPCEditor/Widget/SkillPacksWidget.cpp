#include "SkillPacksWidget.h"

#include <QJsonObject>
#include <QJsonValue>
#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>

SkillPacksWidget::SkillPacksWidget(const QString &name, const QJsonArray &skills, QWidget *parent)
    : QWidget(parent),
      m_pTitle(new QLabel(name, this))
{
    setObjectName("A");
    m_pTitle->setStyleSheet( m_titleStyle );
    m_pTitle->setFixedHeight( 30 );
    setStyleSheet( m_widgetStyle );
    setFixedHeight( 110 );

    QGridLayout* all = new QGridLayout;

    all->addWidget( m_pTitle, 0, 0, 1, 0 );

    int row = 1;
    for ( const QJsonValue &pack: skills ) {
        QLabel *pLabel = new QLabel(pack.toString(), this);
        QSpinBox *pSpinBox = new QSpinBox(this);
        m_skills.insert( pack.toString(), pSpinBox );
        all->addWidget( pLabel, row, 0 );
        all->addWidget( pSpinBox, row, 1 );
        ++row;
    }

    setLayout( all );
}

void SkillPacksWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
