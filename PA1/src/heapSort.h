
#include <vector>
using std::vector;

class HeapSort 
{
  public:
    HeapSort( vector<int> x);
    void sort();
    void displayElements() const; // display vector elements
    vector<int> getData() { return data; }
  private:
    vector< int > data; // vector of ints
    int size;
    int heap_size;

    int parent( int i ){ return i/2; }
    int left( int i ){ return 2*i; }
    int right( int i ) { return (2*i+1); } 
    
    void exchange( int  a, int  b);
    void maxHeaplify( int i);
    void buildMaxHeap();
};


