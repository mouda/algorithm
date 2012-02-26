/*
Program: main.cpp
Author:  JamesLi
Description: this is a main program 
Revision:   V.2010.2.1  
*/

#include <iostream>
using namespace std;
#include "replaceable_sorter.h"  // add this line
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
  replaceable_sorter(A,ELEMENTS);  // change this line
  cout<<endl<<"SORTED LIST"<<endl;
  for(x=0;x<ELEMENTS;x++)
  {
       cout<<A[x]<<endl;
  }
  return 0;
}
