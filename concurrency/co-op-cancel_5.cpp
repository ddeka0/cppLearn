#include <thread>
#include <iostream>
#include <stop_token>

#define Debug(arg) std::cout <<"Tid[" \
    <<std::this_thread::get_id()<<"]" \
    <<" "<<arg <<std::endl;

int main() {
    std::stop_source stSrc;
    auto token = stSrc.get_token();
    
    std::stop_callback cb1{token,[&]() {
        Debug("Callback-1");
    }};

    std::stop_callback cb2{token,[&]() {
        Debug("Callback-2");
    }};

    stSrc.request_stop();

    Debug("Stop requested from main");
}