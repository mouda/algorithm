#include <vector>
using std::vector;

class MySort
{

 public:
  MySort( vector<int> x );               // constructor initializes vector
  void displayElements() const;          // display vector elements
  vector<int> getData(){return data;}
  
  void mergeSort();                      // sort vector using merge sort
  void insertion_sort();
  void heapSort();

 private:

  // ----- common data ----- //
  int size; // vector size
  vector< int > data;                       // vector of ints

  // ----- for the merge sort ----- //
  void sortSubVector( int, int );           // sort subvector
  void merge( int, int, int, int );         // merge two sorted vectors
  void displaySubVector( int, int ) const;  // display subvector


  // ----- for the heap sort ----- //
  int heap_size;

  int parent( int i ){ return i/2; }
  int left( int i ){ return 2*i; }
  int right( int i ) { return (2*i+1); } 
  
  void exchange( int  a, int  b);
  void maxHeaplify( int i);
  void buildMaxHeap();

}; // end class SelectionSort
