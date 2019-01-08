#include <bits/stdc++.h>
using namespace std;
std::random_device rdevice;
std::mt19937 mt(rdevice());
std::uniform_int_distribution<uint8_t> dist(0.0, 255.0);
struct Node {
	int a[10];
	int x;
	Node() {
		memset(a,0,sizeof a);
		x = 0;
	}
};
int main() {
	Node node;
	node.x = 10;
	
	//	Node node2(node);

	shared_ptr<Node> node2(new Node(node));

	cout << node2->x << endl;
}


