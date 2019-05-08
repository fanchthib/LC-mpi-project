#include "State.h"


State::State():isInitial(false),isFinal(false),isSink(false),number(0)
{
}

State::~State()
{
}

void State::setInitial(bool value)
{
    isInitial = value;
}
void State::setFinal(bool value)
{
    isFinal = value;
}
bool State::getFinal() const
{
    return isFinal;
}
bool State::getInitial() const
{
    return isInitial;
}
void State::setSink(bool value)
{
    isSink = value;
}
bool State::getSink() const
{
    return isSink;
}

void State::setNumber(int nb)
{
    number = nb;
}

int  State::getNumber() const
{
    return number;
}
