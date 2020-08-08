#include <bits/stdc++.h>
using namespace std;
typedef unsigned int u32;
#define PrintFunctionName	cout << __PRETTY_FUNCTION__ << endl;

#ifdef RVF
void func(int &&x) {
	PrintFunctionName;
	cout << x << endl;
}
#endif

void func(int &x) {
	PrintFunctionName;
	cout << x << endl;
}

int main() {
	u32 y = 3;
	func(y);
	int z = 4;
	func(z);
}