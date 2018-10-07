#include "Dice.h"

#include <QTime>

Dice::Dice(const int &value)
    : m_value(value)
{}

int Dice::throwValue()
{
    m_generator.seed( static_cast<unsigned int>(QTime::currentTime().msec())+m_throws );
    ++m_throws;
    return m_generator.bounded(1, m_value);
}
