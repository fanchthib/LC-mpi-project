#include <iostream>
#include "Automaton.h"

using namespace std;

int main()
{
  //int choice;
    cout << "Computer Science Project" << endl;
    int fileNumber;
    std::cout <<" Enter the number of the file you want to use" << endl;
    std::cin  >> fileNumber;
    char fileName[20];
    sprintf(fileName, "INT5-%02d.txt",fileNumber);
    Automaton af (fileName);
    af.print();

    Automaton afdc;
    Automaton afd;

    if(af.isSynchronous()){

           if(af.isDeterminist())
           {
               cout << "The automaton is synchonous, and deterministic : " << endl;
        afd= af.determinize();
        afdc=afd;
           }

        else
        {


        cout << "The automaton is synchonous, we are going to determinize it : " << endl;
        afd= af.determinize();
         cout <<"Here is the determinized automaton"<< endl;
         cout<<endl;
         cout<<"************************************** "<<endl;
         cout<<"*********** DETERMINIZATION*********** "<<endl;
         cout<<"************************************** "<<endl;
         afd.print();
         afdc=afd;
    }


         if(!afd.isComplete())
         {


         cout <<"Here is the completed automaton"<< endl;
         cout<<endl;
        cout<<"************************************** "<<endl;
         cout<<"*********** Completion *************** "<<endl;
         cout<<"************************************** "<<endl;


         afdc.complete();
        afdc.print();
         }
         else{
            cout <<"The automaton is already completed"<< endl;
         }

    }
    else
    {
        cout << "The automaton is  not synchonous " << endl;
        af.determinize();
    }

    Automaton Acomp = afdc.complementary();
    cout<<endl;
        cout<<"*********************************************** "<<endl;
         cout<<"*********** Complementary automaton *********** "<<endl;
         cout<<"*********************************************** "<<endl;
    cout << "Here is the complementary automaton: "<< endl;
    Acomp.print();

    int choice;
    cout << "Do you want to test the function recognize a word ? (1: yes, 0:No) ?" << endl;
    cin >> choice;
    if(choice == 1)
    {
        //system("clear");
        cout << "Enter a word to see if it is readable with this automaton! (\"empty\" for the empty word, \"stop\" to stop)" << endl;
        std::string word;
        cin >> word ;

        while (word != "stop")
        {
            if(word == "empty" )
            {


                word = "";
            }


                if (afdc.recognize(word))
                    cout<< "The word is readable by the automaton AFDC"<< endl;
                else{
                    cout <<"The word is NOT readable by the automaton"<< endl;
                    cout <<"Is the word readable by the complementary automaton Acomp ? We are going to try : "<< endl;
                    if (Acomp.recognize(word))
                        cout <<"YES it works !!!!"<< endl;
                    else
                        cout <<"No because a character of your word is not a symbol from the alphabet! "<< endl;
                }
            //afdc.read(word);
            cout<<endl;
            cout<<"Enter a word to see if it is readable with this automaton! (\"empty\" for the empty word, \"stop\" to stop)" << endl;
            cin >> word;

        }
        //cin >> word;
    }



    if(!Acomp.isStanderdized())
    {
        cout << "The automaton is not standardized, we are going to standardize it : " << endl;
        cout<<endl;
         cout<<"************************************** "<<endl;
         cout<<"*********** STANDARDIZATION *********** "<<endl;
         cout<<"************************************** "<<endl;
        Acomp.standardize();
        Acomp.print();

    }
    else
    {
        cout << "\nThis automaton is already standardized !" << endl;
    }



    /*
    else if(!a.isDeterminist()){
         cout << "The automaton is not deterministic, we are going to determinize it : " << endl;
         a = a.determinize();
    }
    a.print();
    a.complete();
    cout << "Here is the automaton completed: " << endl;
    a.print();
    Automaton b = a.complementary();
    cout << "Here is the complementary automaton: "<< endl;
    b.print();

    cout << "Enter a word to see if it is readable with this automaton!" << endl;
    std::string word;
    cin >> word ;
    if (a.recognize(word))
        cout<< "The word is readable by the automaton"<< endl;
    else{
        cout <<"The word is NOT readable by the automaton"<< endl;
        cout <<"Is the word readable by the complementary automaton? We are going to try  "<< endl;
        if (b.recognize(word))
            cout <<"YES it works !!!!"<< endl;
        else
            cout <<"No because a character of your word is not a symbol from the alphabet! "<< endl;
    }*/
    return 0;
}
