#include "Transition.h"

Transition::Transition(State const& _fromState, State const& _toState, char _symbol):
    fromStateNumber(_fromState.getNumber()), toStateNumber(_toState.getNumber()), symbol(_symbol)
{}

Transition::~Transition()
{}

char Transition::getSymbol()
{
    return symbol;
}


int Transition::getFromStateNumber()
{
    return fromStateNumber;
}

int Transition::getToStateNumber()
{
    return toStateNumber;
}
