#include "Transition.h"

void Transition::readIntoNFADictionary(vector<std::string> transfer){
    stringstream aa(entry);

    for(int i = 0; i < transfer.size(); i++){
        this->entry = transfer[i];
        aa.str(this->entry);
        getline(aa, this->input, ',');
        aa.ignore(1); // Ignores one space
        getline(aa, this->alphanum, ' ');
        aa.ignore(1);
        aa >> this->transition;
        nfaTransitions[this->input][this->alphanum].push_back(this->transition);
        aa.clear();

    }
}

void Transition::fillEmptyStates(vector<std::string> alphabet, vector<NFAState*> states){
    for(int i = 0; i < states.size(); i++){
        for(int y = 0; y < alphabet.size(); y++){
            if(nfaTransitions[states[i]->getState()][alphabet[y]].size() == 0){
                nfaTransitions[states[i]->getState()][alphabet[y]].push_back("EM");
            }
        }
    }
    
}

vector<string> Transition::removeEpsilonFromInput(vector<string> alphabet){
    vector<string> myvec;
    for(int i = 0; i < alphabet.size(); i++){
        if(alphabet[i] != "EPS"){
            myvec.push_back(alphabet[i]);
        }
    }

    return myvec;
}

// This allows 
void Transition::displayNFADictionary(std::map<std::string, std::unordered_map<std::string, vector<std::string>>> nfaTransitions){
    // Traverses through the dictionary //Only needed for testing
    for (auto outer_it = nfaTransitions.begin(); outer_it != nfaTransitions.end(); ++outer_it) {
        cout << outer_it->first << ": " << endl;
        for (auto inner_it = outer_it->second.begin(); inner_it != outer_it->second.end(); ++inner_it) {
            cout << " " << inner_it->first << ": ";
            for(auto i : inner_it->second){
                cout << i << " ";
            }
            cout << endl;
        }
    }
    cout << endl;

}

vector<string> Transition::computeNextState(vector<string> transitionState, vector<string> potentialNextState, string sigma, string newTransition){
    nextState = convertedState(transitionState, potentialNextState, sigma, newTransition); // {1, 3} a = {1, 3} or {1, 3} b = {2}
    nextState = elminiateEmptyWithinStates(nextState);
    nextState = checkDuplicates(nextState);
    sort(nextState.begin(), nextState.end());

    return nextState;
}



void Transition::createDFAStates(vector<std::string> transfer, vector<NFAState*> states, vector<std::string> alphabet, std::string start, vector<NFAState*> statesOfNFA){

    vector<string> transitionState;
    alphabet.push_back("EPS");
    readIntoNFADictionary(transfer);
    fillEmptyStates(alphabet, states);
    string newTransition = "";
    vector<string> sigma = removeEpsilonFromInput(alphabet);
    //displayNFADictionary(nfaTransitions);

    vector<string> currentState = getDFAStartState(nfaTransitions, start);
    sort(currentState.begin(), currentState.end()); // USE THIS SORT FOR EACH OF YOUR FINAL DFA STATES
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
                nextState =  computeNextState(transitionState, potentialDFAStates[y], sigma[i], newTransition);
                

                potentialDFAStates.push_back(nextState);
                if(dfaTransitions[potentialDFAStates[y]][sigma[i]].empty()){
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

void Transition::writeToDFA(DFATransitionMap& dfaTransitions, vector<NFAState*> statesOfNFA, vector<string> sigma, std::string start){
    ofstream DFAFile;
    //stringstream write;
    DFAFile.open("output.DFA");
    int number;
    string output;
    //Specification that will be written into a .DFA File

    //Line 1 - States
    for(int i = 0; i < DFAStates.size(); i++){
        DFAFile << "{";
        for(int j = 0; j < DFAStates[i].size(); j++){
            stringstream write(DFAStates[i][j]);
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
        DFAFile << " ";
    }

    DFAFile << endl;
    int startCount = 0;

    //Line 2 - Input Alphabet
    for(int i = 0; i < sigma.size(); i++){
        DFAFile << sigma[i] << " ";
    }
    DFAFile << endl;

    //Line 3 - Start State

    vector<string> DFAStartStateFinal = getDFAStartState(nfaTransitions, start);
    checkDuplicates(DFAStartStateFinal);
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
                    DFAFile << " ";
                    
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
            DFAFile << "}";
            DFAFile << ", ";
            DFAFile << sigma[j];
            DFAFile << " = ";
            if(DFAStates[i] == emptyState){
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

    DFAFile << "END" << endl;
}

vector<string> Transition::convertedState(vector<string> transition, vector<string> currentState, string sigma, string transitionState){ // Get this to return a vector<string> later
    vector<std::string> possibleStates;

    nextState.clear(); // THIS IS TO ENSURE THAT NO VALUES ARE CARRIED OVER IN THE NEXT ITERATION
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

bool Transition::stateExists(vector<vector<string>>& finalDFAStates, vector<string>& nextState){
    for(auto &state: finalDFAStates){
        if(state.size() == nextState.size()){
            if(equal(state.begin(), state.end(), nextState.begin())){
                return true;
            }
        }
    }
    return false;
}

bool Transition::containsEmptyState(vector<string> myvec){
    for(int i = 0; i < myvec.size(); i++){
        if(myvec[i] == "EM"){
            return true;
        }
    }
    return false;
}

bool Transition::containsRealState(vector<string> myvec){
    for(int i = 0; i < myvec.size(); i++){
        if(myvec[i] != "EM"){
            return true;
        }
    }
    return false;
}

vector<string> Transition::elminiateEmptyWithinStates(vector<string> myvec){
    vector<string> withoutEmpty;
    if(containsRealState(myvec)){
        for(int i = 0; i < myvec.size(); i++){
            if(myvec[i] != "EM"){
                withoutEmpty.push_back(myvec[i]);
            }
        }
        return withoutEmpty;
    }
    return myvec;
}

vector<string> Transition::checkEpsilon(TransitionMap& nfaTransitions, string transitionState, int index, vector<std::string> possibleStates){
    if(nfaTransitions[transitionState]["EPS"][index] != "EM"){
        string epsilonClosure = nfaTransitions[transitionState]["EPS"][index];
        possibleStates.push_back(epsilonClosure);
    }


    return possibleStates;
    
}

vector<string> Transition::checkDuplicates(vector<string> myvec){
    vector<string> newVec;
    unordered_map<std::string, bool> unique;
    if(myvec.size() == 1){
        return myvec;
    }

    for(const auto& state: myvec){
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
 * Functionality: Iterates through a vector<string> and prints each state within vector.
 * ------------------------------------------------------------------------------------------------
 * Parameters/Data Types: A vector<string> is passed as a parameter
 * Return Value/Data Type: There is no return value since this is a void function.
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
 * Return Value/Data Type: There is no return value since this is a void function.
 * ************************************************************************************************/

void Transition::printVector(vector<string> states){
    for(int i = 0; i < states.size(); i++){
        cout << states[i] << " ";
    }
}

