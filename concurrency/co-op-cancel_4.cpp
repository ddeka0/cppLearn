#include <mutex>
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>
#include <stop_token>
#include <condition_variable>
#include <future>
#include <coroutine>

#define Debug(arg) std::cout <<"Tid["<<std::this_thread::get_id()<<"]" \
    <<" "<<arg <<std::endl;


int main() {
    std::stop_source stSrc;
    auto token = stSrc.get_token();
    

    std::thread t{[](int x,std::stop_token token) {
        std::mutex m;
        std::condition_variable_any cv;
        std::unique_lock lock{m};

        Debug("Sleeping...");
        
        cv.wait_for(lock,token,std::chrono::seconds(10),[]{
            return false;
        });
        
        Debug("Returning ...");

    },10,token};

    // lets ask to stop the thread t
    std::this_thread::sleep_for(std::chrono::seconds(2));
    stSrc.request_stop();

    Debug("Stop requested from main");
    
    t.join();
}