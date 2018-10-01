#include "SkillSpinBox.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

SkillSpinBox::SkillSpinBox(QWidget *parent)
    : QWidget(parent),
      m_pValue(new QLineEdit(this)),
      m_pStepUp(new QPushButton("+", this)),
      m_pStepDown(new QPushButton("-", this))
{
    setMaximumHeight( 30 );

    m_pValue->setText( QString::number(0) );
    m_pValue->setReadOnly( true );

    m_pStepUp->setMaximumSize( 20, 20 );
    m_pStepDown->setMaximumSize( 20, 20 );

    connect( m_pStepUp, &QPushButton::clicked, this, &SkillSpinBox::skillUp );
    connect( m_pStepDown, &QPushButton::clicked, this, &SkillSpinBox::skillDown );

    QHBoxLayout *pAll = new QHBoxLayout;
    pAll->addWidget( m_pValue );
    pAll->addWidget( m_pStepUp );
    pAll->addWidget( m_pStepDown );
    pAll->setSpacing( 0 );
    pAll->setMargin( 0 );
    setLayout( pAll );
}

void SkillSpinBox::setMinimum(const int &minValue)
{
    m_minValue = minValue;
}

void SkillSpinBox::setValue(const int &value)
{
    if ( value >= m_minValue )
        m_pValue->setText( QString::number(value) );
}

void SkillSpinBox::onAvailableSkillPointsChanged(const int value)
{
    const bool disable = (0 == value) || (value < m_pValue->text().toInt());
    m_pStepUp->setDisabled( disable );
}

void SkillSpinBox::skillUp()
{
    int value = m_pValue->text().toInt();
    m_pValue->setText( QString::number(++value) );
    emit skillValueChanged( m_pValue->text().toInt(), true );
}

void SkillSpinBox::skillDown()
{
    int value = m_pValue->text().toInt();
    if ( value > m_minValue ) {
        m_pValue->setText( QString::number(--value) );
        emit skillValueChanged( m_pValue->text().toInt(), false );
    }
}

void SkillSpinBox::setDisabled(const bool &disable)
{
    m_pStepUp->setDisabled( disable );
    m_pStepDown->setDisabled( disable );
}
