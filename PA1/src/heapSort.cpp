#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;

#include "heapSort.h"

HeapSort::HeapSort( vector<int> x)
{
 size = x.size(); 
 heap_size = x.size();
 data = x;

}

void HeapSort::sort()
{
  buildMaxHeap();
   for ( int i = size; i >= 2; i--){
    exchange(1, i);
    heap_size = heap_size - 1;
    maxHeaplify(1);
  }
}

void HeapSort::maxHeaplify( int i )
{
  int l = left( i ); 
  int r = right( i );
  int largest = 0;
  int temp;
  if( (l <= heap_size) && (data[l-1] > data[i-1]) )
   largest = l;
  else
   largest = i; 
 
  if( (r <= heap_size) && (data[r-1] > data[largest-1]))
    largest = r;

  if( largest != i ) {
    exchange(i,largest); 
    maxHeaplify(largest);
  }
}

void HeapSort::buildMaxHeap()
{
  heap_size = size;
  for( int i = (size) / 2; i >= 1; i--)
    maxHeaplify( i );
}

void HeapSort::exchange(int a, int b)
{
  int temp = data[a-1];
  data[a-1] = data[b-1];
  data[b-1] = temp;
}

