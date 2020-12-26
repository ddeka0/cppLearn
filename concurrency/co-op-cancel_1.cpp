#include <mutex>
#include <chrono>
#include <thread>
#include <iostream>
#include <stop_token>

int main() {
    std::stop_source stSrc;
    auto token = stSrc.get_token();
    auto token2 = std::move(token);
    
    std::thread t{[](int x,std::stop_token token) {
        bool done = false;
        while(!done) {
            std::cout <<"["<<x<<"] Hello from inside of the std::thread "<<std::endl;
            if(token.stop_requested()) {
                std::cout <<"This thread work is done!" << std::endl;
                done = true;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

    },10,token};

    // lets ask to stop the thread t
    std::this_thread::sleep_for(std::chrono::seconds(2));
    stSrc.request_stop();
    std::cout <<"stop requested from main" << std::endl;
    
    
    t.join();
}