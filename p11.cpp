#include <bits/stdc++.h>
using namespace std;
struct Node {
	int x;
	char y;
	Node() {
		x = 4;
		y = 'A';
	}
};
enum Type {
	T_array = 0,
	T_node
};
int main() {
	auto func = [](Type field,void * b) {
		switch(field) {
			case T_array: {
				cout << ((int *)(b))[0] << endl;
			}
			break;
			case T_node: {
				cout << ((Node *)(b))->x << endl;
			}
			break;
		}
	};

	int a[3];
	a[0] = 7;
	func(T_array,static_cast<void*>(a));
	Node node;
	func(T_node,static_cast<void*>(&node));
}