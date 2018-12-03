#include "Dice.h"

#include <QTime>

Dice::Dice(const quint32 &value)
    : m_value(value)
{
    m_generator.seed( seed() );
}

quint32 Dice::throwValue()
{
    ++m_throws;
    return m_generator.bounded( m_value );
}

quint32 Dice::seed()
{
    QDate today = QDateTime::currentDateTime().date();
    QTime now = QTime::currentTime();

    quint32 tSeed = static_cast<quint32>(today.day() + today.month() + today.year()
            + now.hour() + now.minute() + now.second() + now.msec());

    if ( tSeed % m_throws )
        tSeed /= m_throws;
    else
        tSeed /= (m_throws+1);

    return static_cast<quint32>( tSeed );
}
