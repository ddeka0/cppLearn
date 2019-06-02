#include <unistd.h>
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

int main() {

	std::cout << BOLDCYAN << "pplx thread test A" << std::endl << RESET<< std::endl;
	// This is one way of submitting task to threadPool
	
	std::string x("Hello");
	
	auto t1 = pplx::task<std::string/*return type of the immediate lambda function */>([&x]()
	{
		std::cout << GREEN <<"Entry1 with TID : "<<std::this_thread::get_id()
			<< RESET << std::endl;
		
		int cnt = 5;	// some dummy work
		while(cnt >= 0) {
			std::cout << RED <<"loop .... "<< cnt-- << RESET << std::endl;
		}
		
		return (x + std::string(" World !"));	// return a std::string
	})
	.then([](string x)	// catch the return value of the prevTask or prevLambda function
	{	
		sleep(1);	// some dummy work
		std::cout << GREEN << "Entry2 with TID : " <<std::this_thread::get_id()
			<<" : "<< x << RESET << std::endl;
	});
	
	t1.wait();


	std::cout << std::endl << BOLDCYAN <<"pplx thread test B"<< std::endl << RESET << std::endl;
	// This is one more way of submitting tasks to thread pool

	pplx::task_from_result()
	.then([]()
	{	
		std::cout << GREEN <<"Entry3 with TID : "<<std::this_thread::get_id() << RESET << std::endl;
		return pplx::task_from_result<std::string>("Hello");	// return as task		
	})
	.then([](string x)	// capture the return value from prevTask (as simple string)
	{	
		std::cout <<GREEN<<"Entry4 with TID : "<<std::this_thread::get_id()<< RESET <<std::endl;
		
		std::vector<string> ret = {x,"World !"};
		return ret;
	})
	.then([](pplx::task<std::vector<string>> prevTask)	// capturing as task
	{
		std::vector<string> v_strings;	
		try {
			v_strings = prevTask.get(); 
		}
		catch (const std::exception& e) { 
			std::cout << e.what() << std::endl;
		}

		for(auto &str:v_strings)
			std::cout << GREEN << str <<" ";
		
		std::cout << RESET << std::endl;

	}).wait(); // please wait main thread, for these chain to complete

}