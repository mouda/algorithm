#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "mySort.h"
#include "tm_usage.h" 
#include <sys/time.h>

using namespace std;

int main( int argc, char *argv[]){
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
  cout << "The input file is: " << inFileName << endl;  
  cout << "The output file is: " << outFileName << endl;  
#endif 

  char buffer[200]; 
  fstream inFile(inFileName);
  ofstream outFile(outFileName);
  inFile.getline(buffer,200);
  inFile.getline(buffer,200);
  int junk, num;
  vector<int> arr;
  vector<int> result;
  while(inFile >> junk >> num) arr.push_back(num);

#ifdef _TIME_ON_ 
  timeval tvS, tvE;
  CommonNs::TmUsage tmusg;
  CommonNs::TmStat stat;
  tmusg.periodStart();

  gettimeofday( &tvS, NULL);
#endif 

  MySort mySort(arr); 

  /* STL sort*/
  //sort(arr.begin(), arr.end());
  /* insertion sort */
  if ( argv[1][0] == '-' && argv[1][1] == 'I' && argv[1][2] == 'S') {
    mySort.insertion_sort();
  }
  /* merge sort */
  else if ( argv[1][0] == '-' && argv[1][1] == 'M' && argv[1][2] == 'S') {
    mySort.mergeSort();
  }
  else if ( argv[1][0] == '-' && argv[1][1] == 'H' && argv[1][2] == 'S') {
    mySort.heapSort();
  }
  /* heape sort */
  else {
    cout << "Missing the option "<< endl;
    return 1;

  }

#ifdef _TIME_ON_ 
  gettimeofday( &tvE, NULL);
  tmusg.getTotalUsage(stat);
// cout << "Time usage: "<< endl;
  cout << 1000000*(tvE.tv_sec-tvS.tv_sec)+tvE.tv_usec-tvS.tv_usec <<' ';
  cout << stat.vmPeak / 1024.0 << endl; 

#endif 

  result = mySort.getData();
  outFile << "# " << result.size() << " data points" <<endl;
  outFile << "# index number " << endl;  

  for (int i = 0; i < result.size(); i++) {
    outFile << i << ' ' << result[i] << endl;
  }
  outFile << endl; 
  outFile.close();
  inFile.close();
  return 0;
  
}


