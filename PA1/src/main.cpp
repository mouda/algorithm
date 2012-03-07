#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "mergeSort.h"
#include "heapSort.h"
#include "insertionsort.h"

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
  cout << "The input file is: " << inFileName << endl;  
  cout << "The output file is: " << outFileName << endl;  

  char buffer[200]; 
	fstream inFile(inFileName);
	ofstream outFile(outFileName);
  inFile.getline(buffer,200);
  inFile.getline(buffer,200);
  int junk, num;
  vector<int> arr;
  vector<int> result;
  while(inFile >> junk >> num) arr.push_back(num);

  
  

  /* STL sort*/
  //sort(arr.begin(), arr.end());
  /* insertion sort */
  if ( argv[1][0] == '-' && argv[1][1] == 'I' && argv[1][2] == 'S') {
    result = insertion_sort(arr);
  }
  /* merge sort */
  else if ( argv[1][0] == '-' && argv[1][1] == 'M' && argv[1][2] == 'S') {
    MergeSort mergeSort(arr);
    mergeSort.sort();
    result = mergeSort.getData();
  }
  else if ( argv[1][0] == '-' && argv[1][1] == 'H' && argv[1][2] == 'S') {
    HeapSort heapSort(arr);
    heapSort.sort();
    result = heapSort.getData();
  }
  /* heape sort */
  else {
    cout << "Missing the option "<< endl;
    return 1;

  }
  outFile << "# " << result.size() << " data points" <<endl;
  outFile << "# index number" << endl;  

  for (int i = 0; i < result.size(); i++) {
    outFile << i << ' ' << result[i] << endl;
  }
  outFile << endl; 
	outFile.close();
	inFile.close();
	return 0;
  
}


