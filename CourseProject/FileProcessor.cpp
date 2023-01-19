#include "FileProcessor.h"


/* ************************************************************************************************
 * Member Function: analyzeNFA
 * ------------------------------------------------------------------------------------------------
 * Functionality: The input file is analyzed using string stream in order to isolate all states,
 * start state, accept states, and then the transition function for the NFA.
 * ------------------------------------------------------------------------------------------------
 * Parameters/Data Types: This method takes in the name of the file that is executed in the 
 * terminal.
 * Return Value/Data Type: There is no return value since this is a void method.
 * ************************************************************************************************/
void FileProcessor::analyzeNFA(string fileName){
    inFS.open(fileName);

    if(!inFS.is_open()){
        cout << "Cannont open: " << fileName << endl;
        throw std::runtime_error("Error opening file");
    }

    ifstream file(fileName);
    stringstream ss(line);

    //Line 1
    getline(file, line);
    ss.str(line);
    while(ss >> inputState){
        NFAState *NFA = new NFAState();
        NFA->setState(inputState); // Stores a particular NFAState object's state
        states.push_back(NFA);
    }
    ss.clear();

    //Line 2
    getline(file, line);
    ss.str(line);
    while(ss >> alphaChoice){
        alphabet.push_back(alphaChoice);
    }
    ss.clear();

    //Line 3
    getline(file, line);
    string start = line;
    for(int i = 0; i < states.size(); i++)
    {
        if(states[i]->getState() == start)
        {
            states[i]->startState = true; // Sets particular NFAState object's start state to true
        }
    }
    ss.clear();

    //Line 4
    getline(file, line);
    ss.str(line);
    while(ss >> inputState)
    {
        for(int i = 0; i < states.size(); i++)
        {
            if(states[i]->getState() == inputState)
            {
                states[i]->acceptState = true; // Sets particular NFAState object's accept state to true
            }
        }
    }
    ss.clear();

    //Lines 5 - 7
    getline(file, line);
    if(line == "BEGIN"){
        getline(file, line);
        while(line != "END"){
            transfer.push_back(line); // Pushes each line of the transition function on a vector<string>
            getline(file, line); // Used to iterate through transition function until line equals END
        }
    }

    t.createDFAStates(transfer, states, alphabet, start);

}
