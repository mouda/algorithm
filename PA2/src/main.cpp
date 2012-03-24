/**
 * 
 *
 * Copyright (C) 2010 - 2012 Yun-da Tsai (mouda) <dses5211@gmail.com>
 * All Rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <sys/time.h>
#include "knapsack.h"

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
  cout << "The input file is: " << inFileName << endl;  
  cout << "The output file is: " << outFileName << endl;  
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
  cout << "total number: " << totalNumber << endl;  
  cout << "pack size: " << packSpace << endl;  
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
  cout << "the value array: ";
  for ( int i = 0; i < totalNumber ; i++) cout << itemValue[i] << ' ';
  cout << endl; 

  cout << "the size array: ";
  for ( int i = 0; i < totalNumber ; i++) cout << itemSize[i] << ' ';
  cout << endl; 
#endif 


  // ----- initial the object ----- //
  Knapsack myKnapsack(itemValue, itemSize, packSpace);
  //myKnapsack.displayElements();
  bool* Result = myKnapsack.getResult();

  if ( argv[1][0] == '-' && argv[1][1] == 'B' && argv[1][2] == 'F') 
    myKnapsack.bruteForce();
  else if ( argv[1][0] == '-' && argv[1][1] == 'G' && argv[1][2] == 'D') 
    myKnapsack.greedy();
  else if ( argv[1][0] == '-' && argv[1][1] == 'D' && argv[1][2] == 'P') 
    myKnapsack.dynamicProgramming();
  else if ( argv[1][0] == '-' && argv[1][1] == 'R' && argv[1][2] == 'C') 
    myKnapsack.recursion();
  else {
    cout << "Missing the option "<< endl;
    return 1;
  }

#ifdef _DEBUG_ON_ 
  myKnapsack.displayElements();
  cout << "the result: " << endl;
  for ( int i = 0; i < totalNumber; i++) cout << Result[i] << ' ';
  cout << endl;
#endif 
  
}



