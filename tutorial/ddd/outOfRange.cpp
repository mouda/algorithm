#include<iostream>
#include<vector>

using namespace std;

class coordinate {
public:
    coordinate(int x,int y) { 
        x_ = x;
        y_ = y;
    }
    coordinate() {
        x_ = 0;
        y_ = 0;
    }

    int x_;
    int y_;
};

int main() {

    vector<coordinate *> coordinate_list;

    for (int i = 0; i < 5; ++i) {
        coordinate_list.push_back(new coordinate(i,i));
    }

    cout << coordinate_list[5]->x_ << endl;

    return 0;
}
