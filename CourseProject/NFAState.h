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
        //std::string startState;
        
        bool acceptState = false;
        bool startState = false;

        
        bool isNFAAccepted(std::string currentState);
        bool isNFAStartState(std::string currentState);

        void setState(std::string currentState){
            this->currentState = currentState;
        }
    
        std::string getState(){
            return this->currentState;
        }

    private:

};







#endif