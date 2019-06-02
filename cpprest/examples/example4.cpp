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


/*This file implements use case of thread pool using calablanca library */
void TaskA() {
	sleep(rand()%5);
	cout << RED <<"taskA executing .." << RESET << endl;
}
void TaskB() {
	sleep(rand()%5);
	cout << GREEN <<"taskB executing .." << RESET << endl;
}
std::map<int,std::function<void(void)>> taskIdMap = {
	{0,		TaskA},
	{1,		TaskB}
};
int main() {
	srand(time(NULL));
	int cnt = 100;	// 100 task to dispatch 
	while(cnt--) {
		int taskId = rand()%2;	// simulates an receipt of an event or task
		auto func = taskIdMap[taskId];
		pplx::task<int>([func,taskId]()	// check int return type
		{
			func();
			return taskId;

		})
		.then([](pplx::task<int> prevTask)	// capture return value here from prevTask 
		{
			auto id = prevTask.get();
			cout << BOLDYELLOW <<(id == 0?"TaskA is done !":"TaskB is done !") << RESET <<endl;
		});
	}
	cout<< BOLDWHITE <<"No more task to dispatch. Main thread is free now"<< RESET <<endl;
	getchar();	// to wait main thread for all worker thread to finish their tasks
}
