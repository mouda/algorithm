#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;

#include "mySort.h"

MySort::MySort( vector<int> x)
{
 size = x.size(); 
 heap_size = x.size();
 data = x;

}

// -------------------------------------------------------------------------- //
// @Description: insertion sort 
// @Provides: 
// -------------------------------------------------------------------------- //

void MySort::insertion_sort()
{
  int key,i;
  for(int j=1;j<size;j++)
  {
     key=data[j];
     i=j-1;
     while(data[i]>key && i>=0) {
      data[i+1]=data[i];
      i--;
     }
     data[i+1]=key;
  }
}




// -------------------------------------------------------------------------- //
// @Description: merge sort
// @Provides: 
// -------------------------------------------------------------------------- //

void MySort::mergeSort()
{
  sortSubVector( 0, size - 1 ); 
} 

void MySort::sortSubVector( int low, int high )
{                                                 
  if ( ( high - low ) >= 1 ) {
     int middle1 = ( low + high ) / 2;    // calculate middle of vector
     int middle2 = middle1 + 1;           // calculate next element over
     
     sortSubVector( low, middle1 );       // first half of vector      
     sortSubVector( middle2, high );      // second half of vector    
     merge( low, middle1, middle2, high );
  } 
} 

void MySort::merge( int left, int middle1, int middle2, int right ) 
{
  int leftIndex = left; // index into left subvector              
  int rightIndex = middle2; // index into right subvector         
  int combinedIndex = left; // index into temporary working vector
  vector< int > combined( size ); // working vector              

  while ( leftIndex <= middle1 && rightIndex <= right ) {
      // place smaller of two current elements into result
      // and move to next space in vector
      if ( data[ leftIndex ] <= data[ rightIndex ] )
	combined[ combinedIndex++ ] = data[ leftIndex++ ]; 
      else 
	combined[ combinedIndex++ ] = data[ rightIndex++ ];
  }
  
  if ( leftIndex == middle2 ) {                                                       while ( rightIndex <= right ) // copy in rest of right vector
	combined[ combinedIndex++ ] = data[ rightIndex++ ];       
  }                                                      
  else {                                                               
      while ( leftIndex <= middle1 ) // copy in rest of left vector
	combined[ combinedIndex++ ] = data[ leftIndex++ ];        
  } 
  
   // copy values back into original vector
  for ( int i = left; i <= right; i++ )   
    data[ i ] = combined[ i ];           
  
}

void MySort::displayElements() const
{
  displaySubVector( 0, size - 1 );
} 

void MySort::displaySubVector( int low, int high ) const
{
  for ( int i = 0; i < low; i++ )
    cout << "   ";
  
  for ( int i = low; i <= high; i++ )
    cout << " " << data[ i ];
} 


// -------------------------------------------------------------------------- //
// @Description: HeapSort
// @Provides: 
// -------------------------------------------------------------------------- //

void MySort::heapSort()
{
  buildMaxHeap();
   for ( int i = size; i >= 2; i--){
    exchange(1, i);
    heap_size = heap_size - 1;
    maxHeaplify(1);
  }
}

void MySort::maxHeaplify( int i )
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

void MySort::buildMaxHeap()
{
  heap_size = size;
  for( int i = (size) / 2; i >= 1; i--)
    maxHeaplify( i );
}

void MySort::exchange(int a, int b)
{
  int temp = data[a-1];
  data[a-1] = data[b-1];
  data[b-1] = temp;
}
