/*
Filename: insertionsort.cpp
Author:  James Li
Description: this function implements the insertionsort 
Revision:   V.2010.2.1  
*/

#include <iostream>
using namespace std;
#define ELEMENTS 6

void replaceable_sorter(int x[],int length)
{
  int key,i;
  for(int j=1;j<length;j++)
  {
     key=x[j];
     i=j-1;
     while(x[i]>key && i>=0)
     {
               x[i+1]=x[i];
         i--;
     }
     x[i+1]=key;
  }
}
