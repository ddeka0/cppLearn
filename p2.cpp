#include <bits/stdc++.h>
using namespace std;
struct Node {
public:
	int x;
	int y;
	int z;
	Node(){}
	Node(int a,int b,int c) {
		x = a;
		y = b;
		z = c;
	}
};
map<int,Node> Map;
void func(int id,Node &node) {
	Map[id] = node;
}
int main() {
	{
		Node node1;
		node1.x = 10;
		node1.y = 20;
		node1.z = 30;
		func(1,node1);
	}
	cout << Map[1].z << endl;
}