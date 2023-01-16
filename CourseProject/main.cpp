#include "FileProcessor.h"
using namespace std;

int main(int argc, char** argv){
    string fileName = argv[1];
    FileProcessor rnfa;
    //cout <<  << endl;

    rnfa.analyzeNFA(fileName);

}


