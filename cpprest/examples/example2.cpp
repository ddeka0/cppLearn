
#include <bits/stdc++.h>
#include "cpprest/asyncrt_utils.h"

using namespace std;
using namespace utility;

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */


/*Actual code starts from this point ......................... */


#define NUM_ELEMENTS 20
/*This function takes a vector as input and process each element
of the vector independently using the thread pool 
This function returns a vector of all the task handles (or threads) 
This vector will be used later to merge all the thread (using .wait())
*/
pplx::task<std::vector<pplx::task<void>>> Process(std::vector<int> &v) {
	std::vector<pplx::task<void>> tids;
	for(auto &x:v) {
		auto t = pplx::task<void>([&x]()
		{	
			x = rand()%100;	// dummy processing
			int cnt = 1000000;
			while(cnt-- > 0) {
				rand();
			}
		});
		tids.push_back(t);
	}
	cout << YELLOW << "return from Process()" << RESET << endl;
	return pplx::task_from_result<std::vector<pplx::task<void>>>(tids);
}

int main() {
	
	srand(time(NULL));
	std::vector<int> v;
	for(int i = 0;i<NUM_ELEMENTS;i++) {
		v.push_back(i);
	}
	
	Process(v)
	.then([](pplx::task<std::vector<pplx::task<void>>> prevTask) // see line number 50, here we capture the return value
	{
		std::vector<pplx::task<void>> tids;
		try {
			tids = prevTask.get();
		}
		catch(const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
		cout << YELLOW <<"waiting for worker threads to finish"<< RESET << endl;
		for(auto &t : tids) {
			t.wait();
		}
	})
	.wait();
	
	cout << YELLOW << "All worker threads finished task" << RESET << endl;
	for(auto &x:v) {
		std::cout << BOLDCYAN << x <<" ";
	}
	cout <<RESET<<endl;
}
