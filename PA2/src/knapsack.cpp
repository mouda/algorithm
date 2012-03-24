#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include "knapsack.h"


using std::vector;
using std::pair;
using std::make_pair;


bool compareFunction( Item a, Item b) {
  return ( a.value > b.value );
}

// -------------------------------------------------------------------------- //
// @Description: class constructer 
// @Provides: mouda 
// -------------------------------------------------------------------------- //

Knapsack::Knapsack( vector<int> value, vector<int> size, int pSize ) 
{
  Item *pItem;
  for (int i = 0; i < value.size(); i++){
    pItem = new Item;
    pItem->number = i;
    pItem->value = value[i];
    pItem->size = size[i];
    items.push_back(*pItem); 
  } 
  packSize = pSize; 
  result = new bool[value.size()];
  for (int i = 0; i < value.size(); i++) result[i] = 0;
}

// -------------------------------------------------------------------------- //
// @Description: display the items member
// @Provides: mouda 
// -------------------------------------------------------------------------- //

void Knapsack::displayElements() const
{
  for (int i = 0; i < items.size(); i++) { 
    std::cout << "items: "<< items[i].number <<' '<< items[i].value << ' ' << items[i].size
      << std::endl;
  }
}

// -------------------------------------------------------------------------- //
// @Description: bruteforce solve knapsack problem
// @Provides: mouda 
// -------------------------------------------------------------------------- //

void Knapsack::bruteForce()
{

}



// -------------------------------------------------------------------------- //
// @Description: greedy solve knapsack problem
// @Provides: mouda 
// -------------------------------------------------------------------------- //


void Knapsack::greedy()
{
  int remainSpace = packSize;
  int maxIndex = 0;
  sort(items.begin(),items.end(),compareFunction);
  while(remainSpace > 0) {
    remainSpace = remainSpace - items[maxIndex].size;
    result[items[maxIndex].number] = 1;
    maxIndex++;
  }
}

// -------------------------------------------------------------------------- //
// @Description: dynamic programming solve knapsack problem
// @Provides: mouda 
// -------------------------------------------------------------------------- //

void Knapsack::dynamicProgramming()
{

}

// -------------------------------------------------------------------------- //
// @Description: recursion solve knapsack problem
// @Provides: mouda 
// -------------------------------------------------------------------------- //

void Knapsack::recursion()
{

}
