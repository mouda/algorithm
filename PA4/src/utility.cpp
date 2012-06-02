// -------------------------------------------------------------------------- //
// @Description: 
// @Provides: 
// -------------------------------------------------------------------------- //

#include "utility.h"

bool writeMessage( ofstream & outFile, 
    const vector<int> &value, 
    const vector< pair<unsigned, unsigned > > &result, 
    const string &graphName,
    const unsigned &verticesNum, 
    const double &runtime, 
    const double &memory){

    outFile <<"digraph "  << graphName <<"_mf {" <<endl; 
    for (size_t i = 0; i < result.size(); i++) {
      outFile <<'v'<<result[i].first << " -> " <<'v' <<result[i].second;
      outFile << " [label = " << '"' << value[i] << '"' << "];" << endl;
    }

    unsigned sum = 0;
    for (size_t i = 0; i < value.size(); i++) 
      sum += value[i]; 

    outFile << '}' << endl;
    outFile << "// vertices = " << verticesNum<< endl;
    outFile << "// edges = " << result.size()<<endl;
    outFile << "// max flow = " << sum << endl;
    outFile << "// runtime = " << runtime << " sec" << endl;
    outFile << "// memory = " << memory << " MB" ;
    
    return true;
}

bool readMessage(){

}
