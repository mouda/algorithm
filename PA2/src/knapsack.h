#include <vector>
#include <utility>

using std::vector;
using std::pair;

// -------------------------------------------------------------------------- //
// @Description: This data sturcture store the serial number, value and size
//  of every item.
// @Provides: mouda 
// -------------------------------------------------------------------------- //

typedef struct Item {
  int number;
  int value;
  int size;
  bool operator==( Item x) { return x.number == this->number;}
} Item;

// -------------------------------------------------------------------------- //
// @Description: Defince the object to manage all the solving methods 
// @Provides: mouda 
// -------------------------------------------------------------------------- //

class Knapsack {
  public:
    Knapsack( vector<int> value, vector<int> size, int pSize);
    ~Knapsack(){}
    void displayElements() const;
    vector<bool> getResult(){return result;}


    int bruteForce();
    void candidate( vector<Item> buffer );
    int totalSize( vector<Item> x);
    int totalValue( vector<Item> x);

    int greedy();
    int dynamicProgramming();

    int recursion();
    int RCFunction( int i, int pSize);
    int max(int a, int b);

    /* utility */
    void printTable( int **talbe);
    void constructDPTable();
    void destructDPTable();
    void constructTraceBackTable();
    void destructTraceBackTable();

  private:
    vector<Item> items; // number, value, size
    vector< vector<Item> > allCase; //for the BF
    int **dp;
    int **traceBack;
    int packSize;
    vector<bool> result;

};
