#include <chrono>
#include <thread>
#include <future>
#include <iostream>
#include <stop_token>
#include <condition_variable>
#define Debug(arg) std::cout <<"Tid[" \
    <<std::this_thread::get_id()<<"]" \
    <<" "<<arg <<std::endl;


int main() {
    std::stop_source stSrc;
    auto token = stSrc.get_token();

    std::thread t{[](int x,std::stop_token token) {
        std::promise<void> p;        
        auto fu = p.get_future();

        std::stop_callback cb{token,[&]() {
            // runs in the main thread context
            Debug("Promise Value Set");
            p.set_value();
        }};
        
        Debug("Sleeping...");
        /*this thread will wait on the future received from the promise
        * main function will call request_stop after 2 sec
        * therefore callback function will also run after 2 sec
        * promise will set the value
        * the wait on the future will end and this thread will wake up after 2 sec
        */
        fu.wait_for(std::chrono::seconds(10));
        Debug("Woke up...");
        if(token.stop_requested()) {
            Debug("Returning...");
            return;
        }

    },10,token};

    // lets ask to stop the thread t
    std::this_thread::sleep_for(std::chrono::seconds(2));
    stSrc.request_stop();

    Debug("Stop requested from main");
    
    t.join();
}