#include <iostream>
#include <fstream>
#include <vector>
#include <sys/time.h>
#include "knapsack.h"
#include "tm_usage.h" 

using namespace std;

int main( int argc, char *argv[]) {
  /* parsing the input arguments */

  char *inFileName;
  char *outFileName;
  if(argc != 4) {
    cout << "Incorrect input argument number: " <<argc <<endl;
    return 1;
  }

  inFileName = argv[2];
  outFileName = argv[3];

#ifdef _DEBUG_ON_ 
  //cout << "The input file is: " << inFileName << endl;  
  //cout << "The output file is: " << outFileName << endl;  
#endif 

  char buffer[200]; 
  fstream inFile(inFileName);
  ofstream outFile(outFileName);
  
  int totalNumber, packSpace, num;
  char junk;
  vector<int> itemValue; 
  vector<int> itemSize;
  inFile >> junk >> junk >> totalNumber;
  inFile >> junk >> junk >> packSpace;
  
#ifdef _DEBUG_ON_ 
  //cout << "total number: " << totalNumber << endl;  
  //cout << "pack size: " << packSpace << endl;  
#endif 

  inFile >> junk >> junk >> junk;
  for ( int i = 0;  i < totalNumber; i++) {
    inFile >> num;
    itemValue.push_back(num);
  }

  inFile >> junk >> junk >> junk;
  for ( int i = 0;  i < totalNumber; i++) {
    inFile >> num;
    itemSize.push_back(num);
  }

#ifdef _DEBUG_ON_ 
//  cout << "the value array: ";
//  for ( int i = 0; i < totalNumber ; i++) cout << itemValue[i] << ' ';
//  cout << endl; 

//  cout << "the size array: ";
//  for ( int i = 0; i < totalNumber ; i++) cout << itemSize[i] << ' ';
//  cout << endl; 
#endif 

#ifdef _TIME_ON_ 
  timeval tvS, tvE;
  CommonNs::TmUsage tmusg;
  CommonNs::TmStat stat;
  tmusg.periodStart();
  gettimeofday( &tvS, NULL);
#endif 

  // ----- initial the object ----- //
  Knapsack myKnapsack(itemValue, itemSize, packSpace);
  //myKnapsack.displayElements();

  if ( argv[1][0] == '-' && argv[1][1] == 'B' && argv[1][2] == 'F') 
    outFile << "max = " << myKnapsack.bruteForce() << endl;
  else if ( argv[1][0] == '-' && argv[1][1] == 'G' && argv[1][2] == 'D') 
    outFile << "max = " << myKnapsack.greedy() << endl;
  else if ( argv[1][0] == '-' && argv[1][1] == 'D' && argv[1][2] == 'P') 
    outFile << "max = " << myKnapsack.dynamicProgramming() << endl;
  else if ( argv[1][0] == '-' && argv[1][1] == 'R' && argv[1][2] == 'C') 
    outFile << "max = " << myKnapsack.recursion() << endl;
  else {
    cout << "Missing the option "<< endl;
    return 1;
  }

  vector<bool> Result = myKnapsack.getResult();
  outFile << "xi =";
  for ( int i = 0; i < totalNumber; i++) outFile  << ' '<< Result[i];
  outFile << endl;
#ifdef _TIME_ON_ 
  gettimeofday( &tvE, NULL);
  tmusg.getTotalUsage(stat);
  outFile << "runtime = ";
  outFile <<
    (double)(1000000*(tvE.tv_sec-tvS.tv_sec)+tvE.tv_usec-tvS.tv_usec)/1000000
    <<" sec"<<endl;
  outFile << "memory = ";
  outFile << stat.vmPeak / 1024.0 << " MB" <<endl; 
#endif 
  outFile.close();
  inFile.close();
  return 0;
  
}



