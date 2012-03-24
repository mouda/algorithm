#include <iostream>
#include <vector>
#include <utility>
#include "knapsack.h"

using std::vector;
using std::pair;
using std::make_pair;


// -------------------------------------------------------------------------- //
// @Description: class constructer 
// @Provides: 
// -------------------------------------------------------------------------- //

Knapsack::Knapsack( vector<int> value, vector<int> size ) 
{
  for (int i = 0; i < value.size(); i++) 
    data.push_back(make_pair(value[i], size[i])); 
  
  result = new bool[value.size()];
  for (int i = 0; i < value.size(); i++) result[i] = 0;
}

// -------------------------------------------------------------------------- //
// @Description: display 
// @Provides: 
// -------------------------------------------------------------------------- //

void Knapsack::displayElements() const
{
  for (int i = 0; i < data.size(); i++) { 
    std::cout << "data: "<< data[i].first << ' ' << data[i].second << std::endl;
  }
}
