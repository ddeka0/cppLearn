#pragma once
#include <experimental/coroutine>

class rtask {
public:
    struct promise_type;
    using coro_handle = std::experimental::coroutine_handle<promise_type>;
    rtask(rtask&) = delete;
    // task(task&&) = delete;
    rtask(rtask&& other)
    : m_handle(other.m_handle) {
        other.m_handle = nullptr;
    }
    rtask& operator=(rtask&& other) {
        if (&other != this) {
            m_handle = other.m_handle;
            other.m_handle = nullptr;
        }
        return *this;
    }
    rtask(coro_handle handle) : m_handle(handle){
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    bool resume() {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        if(!m_handle.done()) {
            m_handle.resume();
        }
        return not m_handle.done();
    }
    int get();
    ~rtask() {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        m_handle.destroy();
    }

private:
    coro_handle m_handle;
};

struct rtask::promise_type {
    int m_value;
    auto get_return_object() {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        return coro_handle::from_promise(*this);
    }
    auto initial_suspend() {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        return std::experimental::suspend_always();
    }
    auto final_suspend() {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        return std::experimental::suspend_always();
    }
    void yield_value(int v) { m_value = v; }
    // void return_void() {}
    void return_value(int v) {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        m_value = v;
    }
    void unhandled_exception() noexcept {}
};

int rtask::get() {
    // std::cout << __PRETTY_FUNCTION__ << std::endl;
    return m_handle.promise().m_value;
}