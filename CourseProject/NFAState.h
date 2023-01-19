#ifndef NFASTATE_H
#define NFASTATE_H
#include <iostream>

class NFAState{
    public:
        NFAState(){
            this->currentState = "";
        }
        ~NFAState(){

        }
        std::string currentState;
        
        bool acceptState = false;
        bool startState = false;

        void setState(std::string currentState);
    
        std::string getState();

    private:

};







#endif