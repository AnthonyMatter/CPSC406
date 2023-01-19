#include "NFAState.h"

/* ************************************************************************************************
 * Member Function: setState
 * ------------------------------------------------------------------------------------------------
 * Functionality: Sets the state passed into the method as the current state for this particular
 * NFA state.
 * ------------------------------------------------------------------------------------------------
 * Parameters/Data Types: This method takes in a string representing the current state.
 * Return Value/Data Type: There is no return value since this is a void method.
 * ************************************************************************************************/
void NFAState::setState(std::string currentState){
    this->currentState = currentState;
}

/* ************************************************************************************************
 * Member Function: getState
 * ------------------------------------------------------------------------------------------------
 * Functionality: When called, the method returns this particular NFA state.
 * ------------------------------------------------------------------------------------------------
 * Parameters/Data Types: There was no parameters.
 * Return Value/Data Type: This method will return a string containing this particular NFA state.
 * ************************************************************************************************/
std::string NFAState::getState(){
    return this->currentState;
}

