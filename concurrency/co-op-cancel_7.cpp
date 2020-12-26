#include <chrono>
#include <thread>
#include <iostream>
#include <stop_token>

int main() {
    /*An co-operative interruptible thread*/
    std::jthread t{[](std::stop_token token,int x) {
        bool done = false;
        while(!done) {
            std::cout <<"["<<x<<"] Hello from inside of the std::thread "<<std::endl;
            
            if(token.stop_requested()) {
                std::cout <<"This thread work is done!" << std::endl;
                done = true;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

    },10};
 
   /* we dont need to pass the token, jthread constructor
    * will take care of it.
    * We can omit the token params also, if we dont want the thread function
    * to be interruptible
    */
    
    /*In the following thread function, token argument is skipped 
    * Reason being, t2 need not be interrupted from the application point of view
    */
    
    std::jthread t2{[](int x) {
        bool done = false;
        while(!done) {
            std::cout <<"["<<x<<"] Hello from inside of the std::thread2 "<<std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

    },10};

    t.request_stop();
}