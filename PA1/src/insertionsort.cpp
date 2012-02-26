/*
Program: Insertion Sort
Author Name:  Sanchit Karve A.K.A 
Usage: insertionsort 
Revision:   V.2010.2.1  
*/

#include <iostream>
#include <vector>
using namespace std;

vector<int> insertion_sort(vector<int> x)
{
  int key,i;
  int length = x.size();
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
  return x;
}

