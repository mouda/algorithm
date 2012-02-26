// **************************************************************************
// File       [main.cpp]
// Author     [Deitel How to program C++, 5ed. Ch 20, Fig. 20.07]
// Synopsis   [The main program of this demo]
// Modify     [2010/02/21 CM Li]
// **************************************************************************

#include <iostream>
using std::cout;
using std::endl;

#include "global.h"  // glabl variables
#include "mergeSort.h" // class MergeSort definition
#include "tm_usage.h" // the tm_usage library

int main()
{

	CommonNs::TmUsage tmusg;
    CommonNs::TmStat stat;
    tmusg.periodStart();

   // create object to perform merge sort
   MergeSort sortVector( VECTOR_SIZE );
      
   cout << "Unsorted vector:" << endl;
   sortVector.displayElements(); // print unsorted vector
   cout << endl << endl;
 
   sortVector.sort(); // sort vector

   cout << "Sorted vector:" << endl;
   sortVector.displayElements(); // print sorted vector
   cout << endl;

  
   tmusg.getPeriodUsage(stat);
   cout <<"user time:" << stat.uTime / 1000000.0 << "s" << endl; // print period user time in seconds
   cout <<"system time:" << stat.sTime / 1000000.0 << "s" << endl; // print period systemtime in seconds
   cout <<"user+system time:" << (stat.uTime + stat.sTime) / 1000000.0 << "s" << endl; 
   return 0;
} // end main

