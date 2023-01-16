#ifndef DFASTATE_H
#define DFASTATE_H
#include <iostream>
#include <vector>

using namespace std;


class DFAState{
    public:
        DFAState(){

        }
        ~DFAState(){

        }
        void setDFAStartState(string state);
        
        vector<string> state;
        string fullState;
        string alpha;
        string transition;
        string input;
        bool acceptState = false;
        bool startState = false;
        

        void setalpha(string alpha){
            this->alpha = alpha;
        }

        void setTransition(string transition){
            this->transition = transition;
        }

        string getTransition(){
            return this->transition;
        }

        string getAlpha(){
            return this->alpha;
        }

        void setDFAState(vector<string> state){
            this->state = state;
        }

        void setInput(string input){
            this->input = input;
        }

        string getInput(){
            return this->input;
        }

        string getDFAState(){
            this->fullState = "";
            for(int i = 0; i < state.size(); i++){
                this->fullState += state[i];
                if(i != state.size() - 1){
                    this->fullState += ',';
                }
            }
            return this->fullState;
        }




    private:

};





#endif