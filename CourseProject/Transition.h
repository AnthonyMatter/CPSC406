#ifndef TRANSITION_H
#define TRANSITION_H
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "NFAState.h"
#include <map>
#include <unordered_map>
using namespace std;

class Transition{
    public:
        Transition(){

        }
        ~Transition(){

        }

        string input;
        string alphanum;
        string transition;
        string entry;
        vector<string> testState;
        int count = 0;
        vector<string> currentState;
        vector<string> nextState;
        vector<string> emptyState = {"EM"};
        vector<vector<string>> DFAStates;
        vector<vector<string>> potentialDFAStates;
        std::map<std::string, std::unordered_map<std::string, vector<std::string>>> nfaTransitions;
        std::map<vector<std::string>, std::map<std::string, vector<vector<std::string>>>> dfaTransitions;
        using DFATransitionMap = std::map<vector<std::string>, std::map<std::string, vector<vector<std::string>>>>;
        using TransitionMap = std::map<std::string, std::unordered_map<std::string, std::vector<std::string>>>;


        void displayNFADictionary(std::map<std::string, std::unordered_map<std::string, vector<std::string>>> nfaTransitions);
        
        vector<string> convertedState(vector<string> startState, string alpha, string transitionState);

        void readIntoNFADictionary(vector<std::string> transfer);

        void fillEmptyStates(vector<string> alphabet, vector<NFAState*> states);

        vector<string> checkEpsilon(TransitionMap& nfaTransitions, string currentState, int index, vector<std::string> possibleStates);
    
        void createDFAStates(vector<std::string> transfer, vector<NFAState*> statesOfNFA, vector<std::string> alphabet, std::string start);

        vector<string> getDFAStartState(TransitionMap& nfaTransitions, std::string start);

        vector<string> elminiateEmptyWithinStates(vector<string> myvec);

        vector<string> computeNextState(vector<string> potentialNextState, string sigma, string newTransition);

        bool containsRealState(vector<string> myvec);

        bool stateExists(vector<vector<string>>& DFAStates, vector<string>& nextState);

        void writeToDFA(DFATransitionMap& dfaTransitions, vector<NFAState*> statesOfNFA, vector<string> sigma, std::string start);

        vector<string> checkDuplicates(vector<string> myvec);

        void printVector(vector<string> states);

        vector<string> removeEpsilonFromInput(vector<string> alphabet);

    private:

};




#endif