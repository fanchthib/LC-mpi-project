#include "Automaton.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <deque>
#include <algorithm>

int comparedeques(std::deque<int> deque1,std::deque<int>deque2)
{
    unsigned int i = 0;
    for( i = 0 ;i< deque1.size(); i++)
    {
        if(std::find(deque2.begin(),deque2.end(),deque1[i]) == deque2.end())
        {
            return 0;
        }
    }
    return (i==deque2.size());
}

Automaton::Automaton()
{
    //ctor
}

Automaton::~Automaton()
{
    //dtor
}

Automaton::Automaton(std::string fileName)
{
    std::ifstream file(fileName.c_str());
    if(file) // if the file have been successfully opened
    {
        int nbStates = 0, nbTransitions = 0;
        std::cout << "The file: " << fileName << " opened." << std::endl;
        file >> nbSymbol;
        file >> nbStates;
        states.resize(nbStates); //give the nbState size to the state array
        for( int i = 0; i < nbStates; i++)
        {
          states[i].setNumber(i);
        }
        file >> nbInitialStates;
        for(int i = 0; i< nbInitialStates; i++)
        {
            int index = 0;
            file >> index;
            states[index].setInitial(true);

        }
        file >> nbFinalStates;
         for(int i = 0; i< nbFinalStates; i++)
        {
            int index = 0;
            file >> index;
            states[index].setFinal(true);
        }
        file >> nbTransitions;

        for(int i=0; i<nbTransitions; i++)
        {
                        std::string str;
            file >> str;
            std::stringstream ss; // enchainement de données peut importe le type
            ss << str;  // on va récuperer dans l'ordre un entier, un charactère, un entier
            int departure;
            ss >> departure;
            char symbol;
            ss >> symbol;
            int arrival;
            ss >> arrival;
            Transition t(states[departure],states[arrival],symbol); // we create the transition from the previous values found

            transitions.push_back(t); // we go for a next transition

        }
    }
    else
    {
        std::cout << "cannot open file: " << fileName << std::endl;
    }
}

void Automaton::print(){
    std::cout << "\nThis automaton has " << states.size() << " states." << std::endl;
    std::cout << "The initial states are: " << std::endl;
    for (unsigned int i=0; i<states.size();i++){
        if (states[i].getInitial())
            std::cout << " \t "<< i << "\n ";
    }
    std::cout << std::endl;
    std::cout << "The final states are: " << std::endl;
    for (unsigned int i=0; i<states.size();i++){
        if(states[i].getFinal())
           std::cout << " \t "<< i << "\n ";
    }
    std::cout << std::endl;
    std::cout << "And " << transitions.size() << " transitions : " << std::endl;
    for (unsigned int i=0; i<transitions.size(); i++){
        std::cout <<  " Transition " <<  i+1 <<" :  " <<transitions[i].getFromStateNumber()
         <<  transitions[i].getSymbol()  <<  transitions[i].getToStateNumber()<< std::endl;
    }
    if(!newToOriginStates.empty())
    {
        std::cout << "new to origin states" << std::endl;
        for(unsigned int i = 0;i<newToOriginStates.size(); i++)
        {
            std::cout << "new state : "<< i << std:: endl;
            for(unsigned int j = 0; j <newToOriginStates[i].size(); j++ )
            {
                std::cout << newToOriginStates[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
}

int Automaton::isSynchronous(){ // if there is an epsilon transition, the automaton is not synchronous: returns 1 if synchronous

    for (unsigned int i=0; i<transitions.size() ; i++){
        if(transitions[i].getSymbol() == '*')
            return 0;
    }
    return 1;
}

int Automaton::isDeterminist() // if not determinist returns 0
{  //if it is not synchronous or has more than one initial state it is not determinist
    if(isSynchronous()==0 || nbInitialStates>1)
        return 0;

    for(unsigned int i=0; i<transitions.size()-1;i++)
    { // if there is, for two different transitions, a same departure state AND a same transition symbol, it is not determinist
        for(unsigned int j=i+1; j<transitions.size();j++)
            if(transitions[i].getFromStateNumber()==transitions[j].getFromStateNumber() &&
               transitions[i].getSymbol()==transitions[j].getSymbol())
               return 0;
    }
    return 1;
}


int Automaton::isComplete()
{
    if (isDeterminist()==0) //if an automaton is not deter it is not complete
        return 0;
    if(transitions.size()== (states.size()* nbSymbol)) //for each couple of state and symbol we should have one and only transition because it is determinist
        return 1;
    return 0;
}

int Automaton::isStanderdized()//returns 0 if it is not standerdized
{
    if(nbInitialStates!=1)
        return 0;

    for(unsigned int i=0;i<states.size();i++)
    {
        if(states[i].getInitial())
        {
             for(unsigned int j=0;j<transitions.size();j++)
             {
                if(transitions[j].getToStateNumber()==i)
                    return 0;
             }
        }

    }
    return 1;
}



Automaton Automaton::determinize()
{
    if(isDeterminist()==1)
        return *this;
    Automaton a;
    std::deque <int> initial;
    a.nbSymbol= this->nbSymbol;
    bool isfinal=false;
    for (unsigned int i=0; i<states.size(); i++){// the set of ALL initial states of the automaton
        if(states[i].getInitial()){
            initial.push_back(i);
            if(states[i].getFinal())
               {
                   isfinal=true;
               }

        }

    }
    std::deque< std::deque<int> > newToOldStates; //each new state (a deque) have a deque for each element(old corresponding states)
    newToOldStates.push_back(initial);
    State initialState;
    initialState.setNumber(0);
    initialState.setFinal(isfinal);
    initialState.setInitial(true);
    a.states.push_back(initialState);
    a.nbInitialStates=1;
    calculateNextStates(0,initial, newToOldStates, a);
    a.newToOriginStates=newToOldStates;
    return a;

}


//to calculate to where a departure state is going with a certain transition symbol known, if an arrival exists then it goes as 'to'
void Automaton::calculateToState(std::deque<int> fromState, char c, std::deque<int> &to)
{
    for(unsigned int j = 0; j < fromState.size(); j++)
    for(unsigned int i = 0; i < transitions.size(); i++)
    {
        if(fromState[j] == transitions[i].getFromStateNumber())
        {
            if(transitions[i].getSymbol() =='*')
            {
                fromState.push_back(transitions[i].getToStateNumber());
            }
            if(transitions[i].getSymbol() == c)
            {
                if(std::find(to.begin(),to.end(),transitions[i].getToStateNumber()) == to.end()) //add element if not exists.
                to.push_back(transitions[i].getToStateNumber());
            }
        }
    }
}

//to calculate the new states of the determinized automaton
void Automaton::calculateNextStates(int number, std::deque<int> fromState, std::deque< std::deque<int> >& newToOldStates, Automaton &result)
{

    for(char c = 'a'; c < 'a'+nbSymbol; c++)// for all the symbols of the alphabet
    {
        std::deque<int> to;
        calculateToState(fromState,c,to);
        bool oldState = false;
        if (to.empty()) // no transition possible from this state with the current c symbol
            continue; // passe au Charactère (symbole) suivant
        for(unsigned int i = 0; i< newToOldStates.size(); i++)
        {
            if(comparedeques(to, newToOldStates[i]) != 0)// an old state
            {
                oldState = true;
                Transition t(result.states[number],result.states[i],c);
                result.transitions.push_back(t);
                break;// on passe au caractère suivant, we have already found the state
            }
        }
        if(!oldState) // it is a new state, not already manipulated
        {
            State newState;
            newState.setInitial(false);//we know it is not initial
            newState.setNumber(result.states.size());// the size of the array corresponds to the number index of the new state
            newState.setFinal(false);// we suppose it is not final and we verify it just after:
            for(unsigned int j=0;j<to.size();j++)//if just ONE of the corresponding state of the old automaton 'to' is final then the new state is also final
            {
                if(states[to[j]].getFinal())// states is the elements of the old automaton and 'result' is the elements of the new deterministic state (here we look on the old one)
                {
                     newState.setFinal(true);
                     break; // we stop the loop when we find ONE final state, if none is found the state remains not final
                }
            }
            result.states.push_back(newState);//we add the state in the new automaton array of states
            newToOldStates.push_back(to);
            Transition t(result.states[number],result.states[newState.getNumber()],c); // prend une référence donc doit resté existant, result state toujours vivant! La transition est crée à partir des éléments du tableau
            result.transitions.push_back(t);
            calculateNextStates(newState.getNumber(), to, newToOldStates, result);//appel à la récursivité, c'est un nv élément et on cherche les états qui succedront à ce nouvel été (si il y en a)
        }
    }
}

void Automaton::complete()
{

    if(!isDeterminist())
        *this=determinize();
    if(isComplete()==1)
        return;

    State p;
    p.setNumber(states.size());
    states.push_back(p);
    for(char c = 'a';c<'a'+nbSymbol;c++)// on a bien p qui transite vers lui meme pour tous les symboles de l'alphabet
    {
        Transition t(p,p,c);
        transitions.push_back(t);
    }
    for(unsigned int i=0; i<states.size();i++)//pour chaque state
    {
          for(char c = 'a';c<'a'+nbSymbol;c++)//pour chaque symbole de l'alphabet
          {
              bool found = false;
              for(unsigned int j=0; j<transitions.size();j++)
              {
                  if(transitions[j].getSymbol()==c && transitions[j].getFromStateNumber()==i){
                    found=true;
                    break;
                  }
              }
              if (!found)
              {
                  Transition t(states[i],p,c);
                  transitions.push_back(t);
              }

          }
    }
}

Automaton Automaton::complementary()
{

    if(!isComplete())
        complete();
   Automaton result(*this);
   for(unsigned int i = 0; i < states.size(); i++)
   {
        result.states[i].setFinal(!states[i].getFinal());
   }
   return result;
}

bool Automaton::recognize(std::string word)// return false if the word is not readable by the automaton
{
    int currentStateNb;

    for(unsigned int i=0;i<states.size();i++)
    {
        if(states[i].getInitial())//return true if initial and false if not
        {
            currentStateNb=i;
        }

    }

    for(unsigned int i=0;i<word.size();i++)
    {
        char c=word[i];
        if(c>='a'+nbSymbol)//if a character of the word is not in the alphabet of the automaton
            return false;

        for(unsigned int j=0;j<transitions.size();j++)
        {
            if(transitions[j].getFromStateNumber()==currentStateNb && transitions[j].getSymbol()==c)
            {
                currentStateNb=transitions[j].getToStateNumber();
                break;
            }
        }
    }
   return states[currentStateNb].getFinal();
}
void Automaton::standardize()
{
    if(isStanderdized()==1)
        return;

    State s;
    s.setInitial(true);//the state is an initial state
    s.setNumber(states.size());//we give to the state an index
    for(unsigned int i=0;i<states.size();i++)
    {
        if(states[i].getInitial())//return true if initial and false if not
        {
            for(unsigned int j=0;j<transitions.size();j++)
            {
                if(transitions[j].getFromStateNumber()==i)
                {
                    int x;
                    x=transitions[j].getToStateNumber();
                    Transition t(s,states[x],transitions[j].getSymbol());
                    transitions.push_back(t);
                }
            }
            if(states[i].getFinal())//if one of the initial states is a final state then the new state s is final
            {
                s.setFinal(true);
            }
            states[i].setInitial(false);
        }
    }
    states.push_back(s);//we add this state in the list of all the states
    nbInitialStates=1;
}


