#include <bits/stdc++.h>
#include <variant>
using namespace std;
struct A {
	int x;
	char y;
	A(int x,char y) {
		this->x = x;
		this->y = y;
	}
	// Add member funcitons if required
};
struct B {
	int x;
	char y;
	B(int x,char y) {
		this->x = x;
		this->y = y;
	}
	// Add member funcitons if required
};
using myType = std::variant<A, B>;
struct execute {
	void operator()(A & a) { 
		cout <<"Execute work of a" << endl;
	}
	void operator()(B& b) {
		cout <<"Execute work of b" << endl;
	}
};

int main() {
 	vector<myType> v;

 	{
	 	myType x = A(1,'a');
		v.push_back(x);
 	}

 	{
		myType x = B(2,'b');
		v.emplace_back(x);
 	}
	for(auto& x:v) {
		std::visit(execute(),x);
	}
}
