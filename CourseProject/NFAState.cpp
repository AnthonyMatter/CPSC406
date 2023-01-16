#include "NFAState.h"


bool NFAState::isNFAAccepted(std::string currentState){
    return this->acceptState;
}

bool NFAState::isNFAStartState(std::string currentState){
    return this->startState;
}

