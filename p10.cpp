#include <atomic>
#include <bits/stdc++.h>
using namespace std;
template<class T>
struct node
{
    T data;
    node* next;
    node(const T& data) : data(data), next(nullptr) {}
};
 
template<class T>
class Stack
{
    std::atomic<node<T>*> head;
 public:
    void push(const T& data)
    {
        node<T>* new_node = new node<T>(data);
 
        // put the current value of head into new_node->next
        new_node->next = head.load(std::memory_order_relaxed);
        
        string ff = typeid(&head).name();
        system(("echo " + ff + " | c++filt -t").c_str());
        
        string ss = typeid(&new_node->next).name();
        system(("echo " + ss + " | c++filt -t").c_str());
        
        string tt = typeid(new_node).name();
        system(("echo " + tt + " | c++filt -t").c_str());
        // now make new_node the new head, but if the head
        // is no longer what's stored in new_node->next
        // (some other thread must have inserted a node just now)
        // then put that new head into new_node->next and try again
        

        while(!std::atomic_compare_exchange_weak_explicit(
                                &head,
                                &new_node->next,
                                new_node,
                                std::memory_order_release,
                                std::memory_order_relaxed))
                ; // the body of the loop is empty
// note: the above loop is not thread-safe in at least
// GCC prior to 4.8.3 (bug 60272), clang prior to 2014-05-05 (bug 18899)
// MSVC prior to 2014-03-17 (bug 819819). See member function version for workaround
    }
};
 
int main()
{

    Stack<int> s;
    s.push(1);
    std::cout << 1 << std::endl;
}