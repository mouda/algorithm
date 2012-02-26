// **************************************************************************
// File       [mergeSort.h]
// Author     [Deitel How to program C++, 5ed. Ch 20, Fig. 20.05]
// Synopsis   [The header file for a MergeSort Class]
// Modify     [2010/02/21 CM Li]
// **************************************************************************

#ifndef _MERGESORT_H_
#define _MERGESORT_H_


#include <vector>
using std::vector;

// MergeSort class definition
class MergeSort
{
 public:
  MergeSort( int ); // constructor initializes vector
  void sort(); // sort vector using merge sort
  void displayElements() const; // display vector elements
 private:
  int size; // vector size
  vector< int > data; // vector of ints
  void sortSubVector( int, int ); // sort subvector
  void merge( int, int, int, int ); // merge two sorted vectors
  void displaySubVector( int, int ) const; // display subvector
}; // end class SelectionSort

#endif

