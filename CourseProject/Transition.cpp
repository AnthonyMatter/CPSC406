#include "Transition.h"


/* ************************************************************************************************
 * Member Function: readIntoNFADictionary
 * ------------------------------------------------------------------------------------------------
 * Functionality: Takes each line of the NFA transition function and stores the state, 
 * sigma input, and transition state into a nested dictionary representing the NFA. This process
 * continues until the end of the NFA transition function.
 * ------------------------------------------------------------------------------------------------
 * Parameters/Data Types: This method takes in a vector<string>, with each string in the vector
 * being a line of the NFA transition function.
 * Return Value/Data Type: There is no return value since this is a void method.
 * ************************************************************************************************/
void Transition::readIntoNFADictionary(vector<std::string> transfer){
    stringstream aa(entry);

    for(int i = 0; i < transfer.size(); i++){
        this->entry = transfer[i];
        aa.str(this->entry);
        getline(aa, this->input, ','); // Isolates the input state
        //aa.ignore(1); // Ignores one space
        getline(aa, this->alphanum, '='); // Isolates the sigma input
        //aa.ignore(1); // Ignores one space
        aa >> this->transition; // Isolates the transition state
        this->input.erase(std::remove_if(this->input.begin(), this->input.end(), ::isspace), this->input.end());
        this->alphanum.erase(std::remove_if(this->alphanum.begin(), this->alphanum.end(), ::isspace), this->alphanum.end());
        this->transition.erase(std::remove_if(this->transition.begin(), this->transition.end(), ::isspace), this->transition.end());
        //These following three functions will erase any whitespace that is contained within each of these strings

        nfaTransitions[this->input][this->alphanum].push_back(this->transition); // Stores into NFA nested dictionary
        aa.clear();

    }
}


/* ************************************************************************************************
 * Member Function: fillEmptyStates
 * ------------------------------------------------------------------------------------------------
 * Functionality: Iterates through the NFA nested dictionary and if an state with a particular 
 * sigma input leads to nowhere, then we fill that tranistion with an empty state, represented as
 * EM.
 * ------------------------------------------------------------------------------------------------
 * Parameters/Data Types: This method takes in a vector<string> representing the sigma alphabet
 * and a vector of NFA states.
 * Return Value/Data Type: There is no return value since this is a void method.
 * ************************************************************************************************/
void Transition::fillEmptyStates(vector<std::string> alphabet, vector<NFAState*> states){
    for(int i = 0; i < states.size(); i++){
        for(int y = 0; y < alphabet.size(); y++){
            if(nfaTransitions[states[i]->getState()][alphabet[y]].size() == 0){ // This checks if a state with an sigma input leads to nowhere in the NFA
                nfaTransitions[states[i]->getState()][alphabet[y]].push_back("EM");
            }
        }
    }
    
}


/* ************************************************************************************************
 * Member Function: removeEpsilonFromInput
 * ------------------------------------------------------------------------------------------------
 * Functionality: Iterates through the sigma alphabet and removes EPS from the alphabet. I 
 * originally had EPS in the alphabet so that I could find if certain states had an epsilon
 * transition that actually led to another state.
 * ------------------------------------------------------------------------------------------------
 * Parameters/Data Types: This method takes in a vector<string> representing the sigma alphabet.
 * Return Value/Data Type: The sigma alpha is returned as a vector<string> without EPS.
 * ************************************************************************************************/
vector<string> Transition::removeEpsilonFromInput(vector<string> alphabet){
    vector<string> myvec;
    for(int i = 0; i < alphabet.size(); i++){
        if(alphabet[i] != "EPS"){
            myvec.push_back(alphabet[i]);
        }
    }

    return myvec;
}



/* ************************************************************************************************
 * Member Function: displayNFADictionary
 * ------------------------------------------------------------------------------------------------
 * Functionality: Iterates through the NFA nested dictionary and displays the transitions of the
 * NFA states in a condensed form. This was particularly used for testing purposes for most of 
 * the process implementing the program.
 * ------------------------------------------------------------------------------------------------
 * Parameters/Data Types: This takes in a nested dictionary representing the transitions of the 
 * NFA.
 * Return Value/Data Type: There is no return value since this is a void method.
 * ************************************************************************************************/
void Transition::displayNFADictionary(std::map<std::string, std::unordered_map<std::string, vector<std::string>>> nfaTransitions){
    // Traverses through the dictionary //Only needed for testing
    for (auto outer_it = nfaTransitions.begin(); outer_it != nfaTransitions.end(); ++outer_it) { // These are the original states of the NFA.
        cout << outer_it->first << ": " << endl;
        for (auto inner_it = outer_it->second.begin(); inner_it != outer_it->second.end(); ++inner_it) { // This is the sigma inputs
            cout << " " << inner_it->first << ": ";
            for(auto i : inner_it->second){ // These are the transitions given each sigma input for one of the original states
                cout << i << " ";
            }
            cout << endl;
        }
    }
    cout << endl;

}


/* ************************************************************************************************
 * Member Function: computeNextState
 * ------------------------------------------------------------------------------------------------
 * Functionality: Ensure that the next state does not contain any duplicates and is ordered 
 * correctly when the full specification is displyed in a file. Also, if there is an empty state, 
 * but there is also other states, then we remove the empty state since it only transitions to
 * itself.
 * ------------------------------------------------------------------------------------------------
 * Parameters/Data Types: This method takes in a vector with the potential next state, a sigma 
 * input as well as the transition state.
 * Return Value/Data Type: This method returns the next state as a vector<string>.
 * ************************************************************************************************/
vector<string> Transition::computeNextState(vector<string> potentialNextState, string sigma, string newTransition){
    nextState = convertedState(potentialNextState, sigma, newTransition); 
    nextState = elminiateEmptyWithinStates(nextState);
    nextState = checkDuplicates(nextState);
    sort(nextState.begin(), nextState.end());

    return nextState;
}


/* ************************************************************************************************
 * Member Function: createDFAStates
 * ------------------------------------------------------------------------------------------------
 * Functionality: Utilizes a while to compute the next state and then make that next state your
 * input state. This process continues until all reachable states are reached. Simultaneously, the 
 * nested dictionary to represent the DFA transitions are being filled with an input state, the 
 * sigma input, and the transition state, after each iteration. Also, each state is pushed onto a
 * vector containing DFA states and we run helper methods to ensure that duplicate states are not 
 * being pushed onto the vector of DFA states. This runs until there are no new states left to 
 * check.
 * ------------------------------------------------------------------------------------------------
 * Parameters/Data Types: This method takes in a vector of strings, where each string is a line of
 * the NFA transition function. Also, this method takes in a vector of NFA States, a vector with
 * the sigma alphabet, and a string containing the NFA start state.
 * Return Value/Data Type: There is no return value since this is a void method.
 * ************************************************************************************************/
void Transition::createDFAStates(vector<std::string> transfer, vector<NFAState*> statesOfNFA, vector<std::string> alphabet, std::string start){

    alphabet.push_back("EPS");
    readIntoNFADictionary(transfer);
    fillEmptyStates(alphabet, statesOfNFA);
    string newTransition = "";
    vector<string> sigma = removeEpsilonFromInput(alphabet);
    //displayNFADictionary(nfaTransitions);

    vector<string> currentState = getDFAStartState(nfaTransitions, start);
    currentState = elminiateEmptyWithinStates(currentState);
    sort(currentState.begin(), currentState.end()); // This ensures that the start state is ordered correctly before being used to create the DFA transition function
    DFAStates.push_back(currentState);
    potentialDFAStates.push_back(currentState);
    vector<vector<string>> possibleTemp;

    while(potentialDFAStates.size() != 0){
        for(int i = 0; i < sigma.size(); i++){
            int n = potentialDFAStates.size();
            for(int y = 0; y < n; y++){
                if(potentialDFAStates[y] == emptyState){
                    continue;
                }
                nextState =  computeNextState(potentialDFAStates[y], sigma[i], newTransition);
                

                potentialDFAStates.push_back(nextState);
                if(dfaTransitions[potentialDFAStates[y]][sigma[i]].empty()){ // This prohibits the possibility of multiple of the same transition state being pushed into that particular vector
                    dfaTransitions[potentialDFAStates[y]][sigma[i]].push_back(nextState);
                }
            }
        }

        vector<vector<string>> temp;
        temp.clear();

        for(int i = 0; i < potentialDFAStates.size(); i++){
            vector<string> nState = potentialDFAStates[i];
            auto it = std::find(DFAStates.begin(), DFAStates.end(), nState);
            if (it != DFAStates.end()) {

            } else {
                temp.push_back(nState);
                DFAStates.push_back(nState);
            }
        }

        potentialDFAStates = temp;
    }

    writeToDFA(dfaTransitions, statesOfNFA, sigma, start);

}


/* ************************************************************************************************
 * Member Function: writeToDFA
 * ------------------------------------------------------------------------------------------------
 * Functionality: We utilize output stream to write the specifications of a DFA onto a file named 
 * output.DFA. Stringstream is utilized to format sets of states to ensure they are separated by 
 * commas and also enclosed within braces. 
 * ------------------------------------------------------------------------------------------------
 * Parameters/Data Types: The method takes in a nested dictionary representing the DFA
 * transitions, vector of NFA states, vector containing the sigma inputs and a string for the NFA
 * start state.
 * Return Value/Data Type: There is no return type since this is a void method.
 * ************************************************************************************************/
void Transition::writeToDFA(DFATransitionMap& dfaTransitions, vector<NFAState*> statesOfNFA, vector<string> sigma, std::string start){
    ofstream DFAFile;
    DFAFile.open("output.DFA");
    int number; // Used so that we can isolate only the number in each DFA State


    //Specification that will be written into a .DFA File

    //Line 1 - States
    for(int i = 0; i < DFAStates.size(); i++){
        DFAFile << "{"; // Within the loop, we isolate the numbers and separate by commas, outside of the loop they are enclosed in braces
        for(int j = 0; j < DFAStates[i].size(); j++){
            stringstream write(DFAStates[i][j]); // Allocate a string stream onto each DFA state within a vector
            if(DFAStates[i][j] == "EM"){
                DFAFile << "EM";
            }
            else{
            write.ignore();
            write >> number;
            DFAFile << number;
            if(j != DFAStates[i].size() - 1){
                DFAFile << ",";
                }
            }
        }
        DFAFile << "}";
        DFAFile << "    ";
    }

    DFAFile << endl;
    int startCount = 0;

    //Line 2 - Input Alphabet
    for(int i = 0; i < sigma.size(); i++){
        DFAFile << sigma[i] << "    ";
    }
    DFAFile << endl;

    //Line 3 - Start State

    vector<string> DFAStartStateFinal = getDFAStartState(nfaTransitions, start);
    checkDuplicates(DFAStartStateFinal);
    DFAStartStateFinal = elminiateEmptyWithinStates(DFAStartStateFinal);
    DFAFile << "{"; 
    for(int i = 0; i < DFAStartStateFinal.size(); i++){
        if(DFAStartStateFinal[i] == "EM"){
                DFAFile << "EM";
        }
        else
        {
        stringstream write(DFAStartStateFinal[i]);
        write.ignore();
        write >> number;
        DFAFile << number;
        if(i != DFAStartStateFinal.size() - 1){
            DFAFile << ",";
            }
        }
    }
    DFAFile << "}";

    DFAFile << endl;

    //Line 4 - Accept States
    for(int i = 0; i < statesOfNFA.size(); i++){
        for(int j = 0; j < DFAStates.size(); j++){
            for(int z = 0; z < DFAStates[j].size(); z++){
                if(statesOfNFA[i]->acceptState == true && statesOfNFA[i]->getState() == DFAStates[j][z]){
                    DFAFile << "{";
                    for(int k = 0; k < DFAStates[j].size(); k++){
                        if(DFAStates[j][k] == "EM"){
                            DFAFile << "EM";
                        }
                        else
                        {
                        stringstream write(DFAStates[j][k]);
                        write.ignore();
                        write >> number;
                        DFAFile << number;
                        if(k != DFAStates[j].size() - 1){
                            DFAFile << ",";
                            }
                        }
                    }
                    DFAFile << "}";
                    DFAFile << "    ";
                    
                }
            }
        }
    }
    DFAFile << endl;

    //Lines 5 - 7
    DFAFile << "BEGIN" << endl;
    for(int i = 0; i < DFAStates.size(); i++){
        for(int j = 0; j < sigma.size(); j++){
            DFAFile << "{";
            for(int m = 0; m < DFAStates[i].size(); m++){
                if(DFAStates[i][m] == "EM"){
                    DFAFile << "EM";
                }
                else
                {
                stringstream write(DFAStates[i][m]);
                write.ignore();
                write >> number; 
                DFAFile << number;
                if(m != DFAStates[i].size() - 1){
                    DFAFile << ",";
                    }
                }
            }
            DFAFile << "}"; // Above we are isolating the input states and enclosing them within braces
            DFAFile << ", ";
            DFAFile << sigma[j]; // We then write the sigma input after the input states separated by a comma
            DFAFile << " = ";
            if(DFAStates[i] == emptyState){ // If we have reached just the empty state, then write EM to the file and go to next iteration. If not, the following portion below writes the transition states to the file.
                DFAFile << "{";
                DFAFile << "EM";
                DFAFile << "}";
                DFAFile << endl;
                    continue;
                }
            
            DFAFile << "{";
            for(int k = 0; k < dfaTransitions[DFAStates[i]][sigma[j]].size(); k++){
                for(int z = 0; z < dfaTransitions[DFAStates[i]][sigma[j]][k].size(); z++){
                        if(dfaTransitions[DFAStates[i]][sigma[j]][k][z] == "EM"){
                            DFAFile << "EM";
                        }
                        else
                        {
                        stringstream write(dfaTransitions[DFAStates[i]][sigma[j]][k][z]);
                        write.ignore();
                        write >> number;
                        DFAFile << number;
                        if(z != dfaTransitions[DFAStates[i]][sigma[j]][k].size() - 1){
                            DFAFile << ",";
                        }
                    }
                }
            }
            DFAFile << "}";
            DFAFile << endl;
        }
    }

    DFAFile << "END" << endl; // After the full DFA transition function has been written to the file, we write END to indicate the end of the specification of the DFA
}


/* ************************************************************************************************
 * Member Function: convertedState
 * ------------------------------------------------------------------------------------------------
 * Functionality: This method is responsible for converting an state given a sigma input, into a 
 * transition state.
 * ------------------------------------------------------------------------------------------------
 * Parameters/Data Types: This method takes in a vector<string> for the current state, a string
 * for a sigma input, and a string for the transition state.
 * Return Value/Data Type: This will return a vector<string> representing the transition state.
 * ************************************************************************************************/
vector<string> Transition::convertedState(vector<string> currentState, string sigma, string transitionState){ // Get this to return a vector<string> later
    vector<std::string> possibleStates;

    nextState.clear(); // This ensures that no values are carried over to the next iteration
        for(int i = 0; i < currentState.size(); i++){
            for(int y = 0; y < nfaTransitions[currentState[i]][sigma].size(); y++){
                if(nfaTransitions[currentState[i]][sigma][y] != "EM"){
                    transitionState = nfaTransitions[currentState[i]][sigma][y];
                    possibleStates.push_back(transitionState);
                    nextState = checkEpsilon(nfaTransitions, transitionState, 0, possibleStates);
                }
                else{
                    nextState.push_back("EM");
                }
            }
        }

        return nextState;

}


/* ************************************************************************************************
 * Member Function: stateExists
 * ------------------------------------------------------------------------------------------------
 * Functionality: Iterates through a vector of vectors and checks if a vector in the nested
 * dictionary equals nextState.
 * ------------------------------------------------------------------------------------------------
 * Parameters/Data Types: This method takes in a vector<vector<string>> and a vector<string>.
 * Return Value/Data Type: This will return true if the state already exists in the dictionary
 * and false if not.
 * ************************************************************************************************/
bool Transition::stateExists(vector<vector<string>>& finalDFAStates, vector<string>& nextState){
    for(auto &state: finalDFAStates){
        if(state.size() == nextState.size()){
            if(equal(state.begin(), state.end(), nextState.begin())){ // Checks if vector nextState is equal to one of the vectors in finalDFAStates
                return true;
            }
        }
    }
    return false;
}


/* ************************************************************************************************
 * Member Function: containsRealState
 * ------------------------------------------------------------------------------------------------
 * Functionality: Iterates through vector and if there exists a real state, return true. Else, 
 * return false.
 * ------------------------------------------------------------------------------------------------
 * Parameters/Data Types: This method takes in a vector<string>.
 * Return Value/Data Type: This will return true if there is a real state and false if not.
 * ************************************************************************************************/
bool Transition::containsRealState(vector<string> myvec){
    for(int i = 0; i < myvec.size(); i++){
        if(myvec[i] != "EM"){
            return true;
        }
    }
    return false;
}

/* ************************************************************************************************
 * Member Function: elminiateEmptyWithinStates
 * ------------------------------------------------------------------------------------------------
 * Functionality: Through my implementation, all the states will contain inputs that lead to empty
 * states. To ensure that we only display the reachable states, we first check if the vector 
 * contains real states. If so, then we iterate through vector and only push non empty states onto
 * new vector.
 * ------------------------------------------------------------------------------------------------
 * Parameters/Data Types: This method takes in a vector<string>.
 * Return Value/Data Type: This will return a vector<string> without empty states.
 * ************************************************************************************************/
vector<string> Transition::elminiateEmptyWithinStates(vector<string> myvec){
    vector<string> withoutEmpty;
    if(containsRealState(myvec)){ // Check to make sure there is a real state in the vector
        for(int i = 0; i < myvec.size(); i++){
            if(myvec[i] != "EM"){
                withoutEmpty.push_back(myvec[i]);
            }
        }
        return withoutEmpty;
    }
    return myvec;
}

/* ************************************************************************************************
 * Member Function: checkEpsilon
 * ------------------------------------------------------------------------------------------------
 * Functionality: With our new transition state, we check to see if that transition state has an
 * epsilon transition. If it does then we epsilon close the transition state with it's transition
 * state by pushing it onto a vector.
 * ------------------------------------------------------------------------------------------------
 * Parameters/Data Types: This method takes in a nested dictionary representing the NFA, a string
 * as the transition state, the index of the vector within the nested dictionary and a 
 * vector<string> containing all possible states.
 * Return Value/Data Type: This will return a vector<string> with the possible states reached by 
 * one transition.
 * ************************************************************************************************/
vector<string> Transition::checkEpsilon(TransitionMap& nfaTransitions, string transitionState, int index, vector<std::string> possibleStates){
    if(nfaTransitions[transitionState]["EPS"][index] != "EM"){
        string epsilonClosure = nfaTransitions[transitionState]["EPS"][index];
        possibleStates.push_back(epsilonClosure);
    }


    return possibleStates;
    
}

/* ************************************************************************************************
 * Member Function: checkDuplicates
 * ------------------------------------------------------------------------------------------------
 * Functionality: Utilizes a map to store duplicate strings and then iterates through the map
 * to find the unique states.
 * ------------------------------------------------------------------------------------------------
 * Parameters/Data Types: This method takes in a vector<string> of potential duplicate states.
 * Return Value/Data Type: This will return a vector<string> with only unique states.
 * ************************************************************************************************/
vector<string> Transition::checkDuplicates(vector<string> myvec){
    vector<string> newVec;
    unordered_map<std::string, bool> unique;
    if(myvec.size() == 1){
        return myvec;
    }

    for(const auto& state: myvec){ // Iteration through the map
        if(!unique[state]){
            newVec.push_back(state);
            unique[state] = true;
        }
    }
    return newVec;
}

/* ************************************************************************************************
 * Member Function: getDFAStartState
 * ------------------------------------------------------------------------------------------------
 * Functionality: Iterates through nfaTransitions dictionary and checks each state against NFA 
 * start state. If state matches start state then it is pushed onto a vector. If the start also has 
 * an epsilon transition, then we push the destination state as well.
 * ------------------------------------------------------------------------------------------------
 * Parameters/Data Types: This method takes in a nested dictionary and a string for the nfa start 
 * state.
 * Return Value/Data Type: This will return a vector<string>.
 * ************************************************************************************************/
vector<string> Transition::getDFAStartState(TransitionMap& nfaTransitions, string start){
    currentState.clear();
    for (auto outer = nfaTransitions.begin(); outer != nfaTransitions.end(); ++outer){
        for (auto inner = outer->second.begin(); inner != outer->second.end(); ++inner){
            for(auto value : inner->second){
                    if(inner->first == "EPS" && outer->first == start){
                        currentState.push_back(outer->first);
                        currentState.push_back(value);
                    
                    }

            }
        }
    }

    return currentState;
}

/* ************************************************************************************************
 * Member Function: printVector
 * ------------------------------------------------------------------------------------------------
 * Functionality: Iterates through a vector<string> and prints each state within vector.
 * ------------------------------------------------------------------------------------------------
 * Parameters/Data Types: A vector<string> is passed as a parameter
 * Return Value/Data Type: There is no return value since this is a void method.
 * ************************************************************************************************/
void Transition::printVector(vector<string> states){
    for(int i = 0; i < states.size(); i++){
        cout << states[i] << " ";
    }
}