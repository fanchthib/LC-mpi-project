#ifndef STATE_H
#define STATE_H


class State
{
    public:
        State();
        virtual ~State();
        void setInitial(bool value);
        void setFinal(bool value);
        bool getFinal() const;
        bool getInitial() const;
        void setSink(bool value);
        bool getSink()const;
        int getNumber()const;
        void setNumber(int nb);

    private:
        bool isInitial;
        bool isFinal;
        bool isSink;//P
        int  number;
};

#endif // STATE_H
