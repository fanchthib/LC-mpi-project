#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <stdio.h>
#include <string>
#include <vector>
#include <deque>
#include "Transition.h"
#include "Automaton.h"


class Automaton
{
    public:
        Automaton();
        Automaton(std::string fileName);
        void print();
        int isSynchronous();
        int isDeterminist();
        int isComplete();
        int isStanderdized();
        Automaton determinize();
        void complete();
        Automaton complementary();
        bool recognize(std::string word);
        void standardize();//Automaton standardize()
        virtual ~Automaton();

    private:
        int nbSymbol;//number of symbols in the alphabet
        int nbInitialStates;
        int nbFinalStates;
        std::deque< std::deque<int> > newToOriginStates;
        std::vector<State> states; //set of states
        std::vector<Transition> transitions; //set of transition
        void calculateNextStates(int number,std::deque<int> fromState,std::deque< std::deque<int> >& newToOldStates, Automaton &result);
        void calculateToState(std::deque<int> fromState, char c, std::deque<int> &to);

};

#endif // AUTOMATON_H
