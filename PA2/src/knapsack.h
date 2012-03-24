#include <vector>
#include <utility>

using std::vector;
using std::pair;

typedef struct {
  int number;
  int value;
  int size;
} Item;

class Knapsack {
  public:
    Knapsack( vector<int> value, vector<int> size, int pSize);
    ~Knapsack(){ delete result; }
    void displayElements() const;
    bool* getResult(){return result;}


    void bruteForce();
    void greedy();
    void dynamicProgramming();
    void recursion();

  private:
    vector<Item> items; // number, value, size
    int packSize;
    bool* result;

};
