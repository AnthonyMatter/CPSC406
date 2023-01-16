#include "FileProcessor.h"

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
        NFA->setState(inputState);
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
            states[i]->startState = true;
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
                states[i]->acceptState = true;
            }
        }
    }
    ss.clear();

    //Lines 5 - 7
    getline(file, line);
    if(line == "BEGIN"){
        getline(file, line);
        while(line != "END"){
            transfer.push_back(line);
            getline(file, line);
        }
    }

    t.createDFAStates(transfer, states, alphabet, start, states);

}

void FileProcessor::writeDFA(){
    
}