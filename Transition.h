#ifndef TRANSITION_H
#define TRANSITION_H

#include "State.h"

class Transition
{
    public:

        Transition(State const& _fromState, State const& _toState, char _symbol);
        virtual ~Transition();
        char getSymbol();
        int getFromStateNumber();
        int getToStateNumber();

    private:
        int fromStateNumber;
        int toStateNumber;
        char symbol;
};

#endif // TRANSITION_H
