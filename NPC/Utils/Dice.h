#ifndef DICE_H
#define DICE_H

#include <QRandomGenerator>

class Dice
{
public:
    Dice(const int &value);
    ~Dice() =default;

    int throwValue();

private:
    int m_value{0};
    unsigned int m_throws{0};
    QRandomGenerator m_generator;
};

#endif // DICE_H
