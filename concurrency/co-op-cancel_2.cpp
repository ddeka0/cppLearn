#include <mutex>
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>
#include <stop_token>
#include <condition_variable>
#include <future>
#define Debug(arg) std::cout <<"Tid["<<std::this_thread::get_id()<<"]"<<" "<<arg <<std::endl;


int main() {
    std::stop_source stSrc;
    auto token = stSrc.get_token();

    std::thread t{[](int x,std::stop_token token) {
        std::promise<void> p;        
        auto fu = p.get_future();

        std::stop_callback cb{token,[&]() {
            // either runs in the main's context or this thread context
            Debug("Promise Value Set");
            p.set_value();
        }};
        
        Debug("Sleeping...");

        fu.wait_for(std::chrono::seconds(10));
        
        if(token.stop_requested()) {
            return;
        }

    },10,token};

    // lets ask to stop the thread t
    std::this_thread::sleep_for(std::chrono::seconds(2));
    stSrc.request_stop();

    Debug("Stop requested from main");
    
    t.join();
}