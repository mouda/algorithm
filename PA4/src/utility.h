// -------------------------------------------------------------------------- //
// @Description: the common utility
// @Provides: 
// -------------------------------------------------------------------------- //
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

bool writeMessage( ofstream & outFile, const vector<int> &value, 
    const vector< pair<unsigned, unsigned > > & result, 
    const string &graphName,
    const unsigned &verticesNum, 
    const double &runtime, 
    const double &memory
    );

bool readMessage();
