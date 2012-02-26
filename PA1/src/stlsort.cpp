/*
Program: Sort
Author Name:  
Usage: sort 
Revision:   V.2010.2.1  
*/

#include <iostream>
#include <algorithm>
using namespace std;
#define ELEMENTS 6


int main()
{
  int A[ELEMENTS]={5,2,4,6,1,3};
  int x;
  cout<<"NON SORTED LIST:"<<endl;
  for(x=0;x<ELEMENTS;x++)
  {
       cout<<A[x]<<endl;
  }
  sort (A, A+ELEMENTS);
  cout<<endl<<"SORTED LIST"<<endl;
  for(x=0;x<ELEMENTS;x++)
  {
       cout<<A[x]<<endl;
  }
  return 0;
}
