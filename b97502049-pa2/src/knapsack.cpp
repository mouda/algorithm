#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include "knapsack.h"
#include <string>

using std::vector;
using std::pair;
using std::make_pair;
using std::find;
using std::string;

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
  for (int i = 0; i < value.size(); i++) result.push_back(0);

      
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

int Knapsack::bruteForce()
{
  string buffer;
  candidate(buffer, items.size());
  if (allCase.size()== 0 ) return 0; 
  else{
    int maxCase = allCase.size()-1;
    for (int i = 1; i < allCase.size() ; i++) 
      if ( totalValue(allCase[i]) > totalValue(allCase[maxCase]) && 
          totalSize(allCase[i]) <= packSize){ 
        maxCase = i;
      }
    for (int i = 0; i < items.size(); i++) {
      if ( allCase[maxCase][i] == '1' ) result[i] = 1;
    }  
    return totalValue(allCase[maxCase]);
  }
}

void Knapsack::candidate( string buffer, int n)
{
  if ( n < 0 ) {
    allCase.push_back(buffer);
    return;
  }
  candidate( buffer+'1', n-1);
  candidate( buffer+'0', n-1);
}

int Knapsack::totalSize( string x)
{
  int totalSize = 0;
  for (int i = 0; i < x.size(); i++)
    if ( x[i] == '1') totalSize += items[i].size; 
  return totalSize;
}

int Knapsack::totalValue( string x)
{
  int totalValue = 0;
  for (int i = 0; i < x.size(); i++) 
    if ( x[i] == '1') totalValue += items[i].value; 
  return totalValue;
}



// -------------------------------------------------------------------------- //
// @Description: greedy solve knapsack problem
// @Provides: mouda 
// -------------------------------------------------------------------------- //


int Knapsack::greedy()
{
  int remainSpace = packSize;
  int maxValue = 0;
  sort(items.begin(),items.end(),compareFunction);
  
  for (int i = 0; i < items.size(); i++) {
    if (items[i].value == items[i+1].value && items[i].size > items[i+1].size) 
      continue;
    if (remainSpace >= items[i].size) {
      remainSpace = remainSpace - items[i].size; 
      result[items[i].number] = 1;
      maxValue += items[i].value;
    }
  }
  return maxValue;
#ifdef _DEBUG_ON_ 
  std::cout << "remain space: " << remainSpace << std::endl;
#endif 
}

// -------------------------------------------------------------------------- //
// @Description: dynamic programming solve knapsack problem
// @Provides: mouda 
// -------------------------------------------------------------------------- //

int Knapsack::dynamicProgramming()
{

  int maxValue;
  constructDPTable();
  constructTraceBackTable();

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
  maxValue = dp[items.size()][packSize];
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
  destructTraceBackTable();
  destructDPTable();
  return maxValue; 

}

// -------------------------------------------------------------------------- //
// @Description: recursion solve knapsack problem
//  Divide and conquer
//  Divide the original backpack into the sub-backpack, and assume the 
//  sub-backpack is the optimize result, and so on.
// @Provides: mouda 
// -------------------------------------------------------------------------- //

int Knapsack::recursion()
{
  int maxValue;
  int remain = packSize;
  int x = items.size();
  constructDPTable();

  maxValue = RCFunction( items.size(),packSize);
  while( remain  && x != 0) {
    while ( dp[x][remain] == dp[x-1][remain]) x--;
    result[x-1] = 1;
    remain = remain - items[x-1].size;
    x--;
  }
  destructDPTable();
  return maxValue;
  
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

// -------------------------------------------------------------------------- //
// @Description: construct the dynamic programming table
// @Provides: mouda 
// -------------------------------------------------------------------------- //

void Knapsack::constructDPTable()
{
  dp = new int *[items.size()+1];
  for (int i = 0; i <= items.size(); i++) dp[i] = new int[packSize+1];
  for (int i = 0; i <= items.size(); i++) dp[i][0] = 0; 
  for (int i = 0; i <= packSize; i++) dp[0][i] = 0; 
  for (int i = 1; i <= items.size(); i++) 
    for (int j = 1; j <= packSize; j++) 
      dp[i][j] = -1;

}

// -------------------------------------------------------------------------- //
// @Description: destruct the dynamin programming table
// @Provides: mouda
// -------------------------------------------------------------------------- //

void Knapsack::destructDPTable()
{
  for (int i = 0; i <= items.size(); i++) delete[] dp[i];
  delete[] dp;
}

// -------------------------------------------------------------------------- //
// @Description: construct the trace back table
// @Provides: mouda
// -------------------------------------------------------------------------- //

void Knapsack::constructTraceBackTable()
{
  traceBack = new int *[packSize+1];
  for (int i = 0; i <= items.size(); i++) traceBack[i] = new int[packSize+1];
  for (int i = 0; i <= items.size(); i++) 
    for (int j = 0; j <= packSize; j++) 
      traceBack[i][j] = 0;
}

// -------------------------------------------------------------------------- //
// @Description: destruct the trace back table
// @Provides: mouda 
// -------------------------------------------------------------------------- //

void Knapsack::destructTraceBackTable()
{
  for (int i = 0; i <= items.size(); i++) delete[] traceBack[i];
  delete[] traceBack;
}

