#include <iostream>
#include <fstream>

using namespace std;

int Max (int a, int b)
{
	if (a> b) return a;
	else return b;
}

int main () 
{
	ifstream inFile("test.in");
	ofstream outFile("test.out");
	int a,b;
	inFile >> a;
	inFile >> b;	
	outFile << Max(a, b) << endl;
	outFile.close();
	inFile.close();
	return 0;
}		

