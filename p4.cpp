#include <iostream>
#include <map>
#include <algorithm>
#include <mutex>
using namespace std;
struct C {
	int r;
	char s;
};
struct B {
	int p;
	char q;
	C c;
};
struct A {
	int x;
	int y;
	B b;
	char z;
};
/*
	A
	L__ x
	|
	L__ y
	|
	L__ b
		L__p
		|
		L__q
		|
		L__c
			L__r
			|
			L__s

*/
/*  the follwing is the
	data structure to store objects of of type A 
	corresponding to an ID (int)
*/
map<int,A> Map;
mutex m;

/*  Follwing are the desired 
	api's to set and get values from the Map 
	Locks are also required to handle concurrency
*/
void getValueFromMap(int id,/* what to pass ?? */) {
	m.lock();

	/* code to get the value of the specified varible */

	m.unlock();
}
void setValueInMap(int id,/* what to pass ?? */) {
	m.lock();

	/* code to set the new value to the specified variable */

	m.unlock();	
}
int main() {
	/* lets suppose Map has some A type objects already */
	int id = 4;
	/* assume I want to change the information (value of p) of  id = 4*/
	/* instead of doing the following */
	m.lock();
	Map[id].b.p = 12; /*update to some value */
	m.unlock();
	/* what I need the follwing */
	setValueInMap(4,/* what_to_change , what_is_the_new_value etc .. */);

	/*similarly */

	/*if I want get the value of s */
	int s;
	m.lock();
	getValueFromMap(id,/* what_to_get,placeholder (for example &s) */);
	m.unlock();
}

The first idea came to me, is to have function pointer table of all the `getter` and `setter` 
function (needs to be defined inside the structs) for all the member variables and pass those 
function pointers as callbacks to the `api`'s along with new value or `placeholders`. 
If this approach correct, can you please show me, how to implement it?