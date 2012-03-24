#include <vector>
#include <utility>

using std::vector;
using std::pair;

class Knapsack {
  public:
    Knapsack( vector<int> value, vector<int> size);
    void displayElements() const;
    void displayResult() const;
    

  private:
    vector< pair<int,int> > data;
    bool* result;

};
