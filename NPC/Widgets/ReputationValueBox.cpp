#include "ReputationValueBox.h"

#include <QHBoxLayout>

ReputationValueBox::ReputationValueBox(QWidget *parent)
    : QWidget(parent),
      m_pValue(new QLineEdit("0", this)),
      m_pUpBtn(new QPushButton("+", this)),
      m_pDownBtn(new QPushButton("-", this))
{
    m_pValue->setReadOnly( true );
    m_pValue->setMinimumWidth( 50 );
    m_pUpBtn->setFixedWidth( 35 );
    m_pDownBtn->setFixedWidth( 35 );

    connect( m_pUpBtn, &QPushButton::clicked,
             this, &ReputationValueBox::upBtnClick );
    connect( m_pDownBtn, &QPushButton::clicked,
             this, &ReputationValueBox::downBtnClick );

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->setSpacing( 2 );
    setLayout( pLayout );

    pLayout->addWidget( m_pValue );
    pLayout->addWidget( m_pUpBtn );
    pLayout->addWidget( m_pDownBtn );
}

int ReputationValueBox::value() const
{
    return m_pValue->text().toInt();
}

void ReputationValueBox::setValue(const int &value)
{
    m_pValue->setText( QString::number(value) );
}

void ReputationValueBox::setUpDisable(const bool &disable)
{
    m_pUpBtn->setDisabled( disable );
}

void ReputationValueBox::setDownDisable(const bool &disable)
{
    m_pDownBtn->setDisabled( disable );
}

void ReputationValueBox::setMinimumValue(const int &min)
{
    m_minimum = min;
}

void ReputationValueBox::setMaximumValue(const int &max)
{
    m_maximum = max;
}

void ReputationValueBox::upBtnClick()
{
    const int tValue = m_pValue->text().toInt()+1;
    if ( m_maximum >= tValue ) {
        m_pValue->setText( QString::number(tValue) );
        emit valueUp();
    }
}

void ReputationValueBox::downBtnClick()
{
    const int tValue = m_pValue->text().toInt()-1;
    if ( m_minimum <= tValue ) {
        m_pValue->setText( QString::number(tValue) );
        emit valueDown();
    }
}
