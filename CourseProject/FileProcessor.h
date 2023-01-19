#ifndef FILEPROCESSOR_H
#define FILEPROCESSOR_H
#include <vector>
#include <iostream>
#include <string>
#include "NFAState.h"
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
        Transition t;
        //convertNFA converter;
        vector<NFAState*> states;
        vector<string> alphabet;
        vector<string> transfer;
        ifstream inFS;
        string line;
        string inputState;
        string alphaChoice;

        void analyzeNFA(std::string fileName);

};




#endif

