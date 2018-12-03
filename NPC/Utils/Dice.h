#ifndef DICE_H
#define DICE_H

#include <QRandomGenerator>

class Dice
{
public:
    Dice(const quint32 &value);
    ~Dice() =default;

    quint32 throwValue();

private:
    quint32 seed();

private:
    quint32 m_value{0};
    quint32 m_throws{1};
    QRandomGenerator m_generator;
};

#endif // DICE_H
