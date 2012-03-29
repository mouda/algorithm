#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include "knapsack.h"


using std::vector;
using std::pair;
using std::make_pair;
using std::find;


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
//  result = new bool[value.size()];
  for (int i = 0; i < value.size(); i++) result.push_back(0);

  dp = new int *[items.size()+1];
  for (int i = 0; i <= items.size(); i++) dp[i] = new int[packSize+1];
  for (int i = 0; i <= items.size(); i++) dp[i][0] = 0; 
  for (int i = 0; i <= packSize; i++) dp[0][i] = 0; 
  for (int i = 1; i <= items.size(); i++) 
    for (int j = 1; j <= packSize; j++) 
      dp[i][j] = -1;
  //traceBack = new int [value.size()];
      
}

Knapsack::~Knapsack()
{
  for (int i = 0; i <= items.size(); i++) delete[] dp[i];
  delete[] dp;
  //delete[] traceBack;
}

// -------------------------------------------------------------------------- //
// @Description: display the items member
// @Provides: mouda 
// -------------------------------------------------------------------------- //

void Knapsack::displayElements() const
{
  for (int i = 0; i < items.size(); i++)  
    std::cout << "items: "<< items[i].number <<' '<< items[i].value << ' '
      << items[i].size << std::endl;
}

// -------------------------------------------------------------------------- //
// @Description: bruteforce solve knapsack problem
// @Provides: mouda 
// -------------------------------------------------------------------------- //

void Knapsack::bruteForce()
{
  vector<Item> buffer;
  candidate(buffer);
  if (allCase.size()== 0 ) return; 
  else{
    int maxCase = 0;
    for (int i = 1; i < allCase.size() ; i++) 
      if ( totalValue(allCase[i]) > totalValue(allCase[maxCase])) 
        maxCase = i;
    for (int i = 0; i < allCase[maxCase].size(); i++) {
      result[allCase[maxCase][i].number] = 1;
    }  
    //std::cout<<totalValue(allCase[maxCase])<<std::endl;
  }
}

void Knapsack::candidate( vector<Item> buffer)
{
  bool addedSomething = false;
  for (int i = 0; i < items.size(); i++) {
    if ( find(buffer.begin(), buffer.end(), items[i]) == buffer.end()
        && (totalSize(buffer)+items[i].size <= packSize  )) {
      buffer.push_back(items[i]);
      candidate(buffer);
      buffer.pop_back();
      addedSomething = true;
    }
  }
  if (!addedSomething) {
    allCase.push_back(buffer); 
    return;
  }
}

int Knapsack::totalSize( vector<Item> x)
{
  int totalSize = 0;
  for (int i = 0; i < x.size(); i++) totalSize += x[i].size; 
  return totalSize;
}

int Knapsack::totalValue( vector<Item> x)
{
  int totalValue = 0;
  for (int i = 0; i < x.size(); i++) totalValue += x[i].value; 
  return totalValue;
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
  
  for (int i = 0; i < items.size(); i++) {
    if ( i < items.size() && items[i].value == items[i+1].value 
        && items[i].size > items[i+1].size) 
      continue;
    if (remainSpace >= items[i].size) {
      remainSpace = remainSpace - items[i].size; 
      result[items[i].number] = 1;
    }
  }
#ifdef _DEBUG_ON_ 
  std::cout << "remain space: " << remainSpace << std::endl;
#endif 
}

// -------------------------------------------------------------------------- //
// @Description: dynamic programming solve knapsack problem
// @Provides: mouda 
// -------------------------------------------------------------------------- //

void Knapsack::dynamicProgramming()
{

  traceBack = new int *[packSize+1];
  for (int i = 0; i <= items.size(); i++) traceBack[i] = new int[packSize+1];
  for (int i = 0; i <= items.size(); i++) 
    for (int j = 0; j <= packSize; j++) 
      traceBack[i][j] = 0;

  for (int i = 0; i <= items.size(); i++) {
    for (int j = 0; j <=  packSize; j++) {
      if ( i== 0 || j == 0) 
        dp[i][j] = 0;
      else if ( items[i-1].size <= j){
        if (items[i-1].value+dp[i-1][j-items[i-1].size] > dp[i-1][j] ) {
          dp[i][j] = items[i-1].value+dp[i-1][j-items[i-1].size];
          traceBack[i][j] = items[i-1].size;
        }
        else dp[i][j] = dp[i-1][j];
      }
      else 
        dp[i][j] = dp[i-1][j];
    }
  }
//  std::cout << dp[items.size()][packSize] << std::endl;

  /* find the combination */
  int remain = packSize;
  int x = items.size();
  while( dp[items.size()][remain] - dp[items.size()][remain-1] == 0) remain--;
  while( remain ) {
    while ( traceBack[x][remain] == 0) x--;
    result[x-1] = 1;
    remain = remain - traceBack[x][remain];
    x--;
  }

  for (int i = 0; i <= items.size(); i++) delete[] traceBack[i];
  delete[] traceBack;

}

// -------------------------------------------------------------------------- //
// @Description: recursion solve knapsack problem
//  Divide and conquer
//  Divide the original backpack into the sub-backpack, and assume the 
//  sub-backpack is the optimize result, and so on.
// @Provides: mouda 
// -------------------------------------------------------------------------- //

void Knapsack::recursion()
{
  
  RCFunction( items.size(),packSize);
  int remain = packSize;
  int x = items.size();
  while( remain  && x != 0) {
    while ( dp[x][remain] == dp[x-1][remain]) x--;
    result[x-1] = 1;
    remain = remain - items[x-1].size;
    x--;
  }
  
}

int Knapsack::RCFunction( int i, int pSize)
{
  if ( i == 0 || pSize == 0) { 
    dp[i][pSize] = 0;
    return 0;
  }
  if (dp[i][pSize] < 0) {
    if (pSize < items[i-1].size)
      dp[i][pSize] = RCFunction(i-1,pSize);
    else
      dp[i][pSize] = max(RCFunction(i-1,pSize), 
          items[i-1].value + RCFunction(i-1,pSize-items[i-1].size) );
  }
  return dp[i][pSize];

}

int Knapsack::max( int a, int b)
{
  if ( a > b ) return a;
  else  return b;
}

// -------------------------------------------------------------------------- //
// @Description: print the table for debugging
// @Provides: mouda 
// -------------------------------------------------------------------------- //

void Knapsack::printTable( int **table) 
{
  for (int i = 0; i <= items.size(); i++){ 
    for (int j = 0; j <= packSize; j++) 
      std::cout << table[i][j] << ' ' ;
    std::cout<< std::endl;
  }
}
