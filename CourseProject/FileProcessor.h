#ifndef READNFA_H
#define READNFA_H
#include <vector>
#include <iostream>
#include <string>
#include "NFAState.h"
#include "DFAState.h"
#include "Transition.h"

using namespace std;

class FileProcessor{
    public:
        FileProcessor(){

        }
        ~FileProcessor(){

        }
        string filename;

        NFAState nfa;
        DFAState dfa;
        Transition t;
        vector<NFAState*> states;
        vector<string> alphabet;
        vector<string> transfer;
        ifstream inFS;
        string line;
        string inputState;
        string alphaChoice;

        void analyzeNFA(std::string fileName);
        void writeDFA();

};




#endif

