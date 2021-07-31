I find myself in a situation with the following implementation.

I have a class that is exposed to users.

My intention is to expose a high-level abstraction to the user rather than
internal details of my library (or details they would need to handle without this library). 

A user can call some method named `do_this_for_me` and expects something
to happen for a service. But internally, my Adapter class needs to maintain some context for this user in a vector called `hidden_from_user`.

Now I want to consume this vector from my internal implementation of the core library.
But I can not because the `hidden_from_user` private in the Adapter class.




    #include <iostream>
    #include <vector>
    
    namespace adapter {
    
        class Adapter {
            public:
                void do_this_for_me(int flags) {
                    if(flags == 1) { // just as an example
                        int something_internal = 10;
                        hidden_from_user.push_back(something_internal);
                    }
                }
            private:
                // hidden_from_user vector is required store some changes
                // so that my internal implementation can use this vector
                std::vector<int> hidden_from_user;
        };
    }
    
    
    namespace core {
        class InternalThing {
            public:
                int internal_handling(UserApi* handle) {
                    // how to access hidden_from_user vector here using handle ?
    
                }
        };
    }
    
    
    
    
    int main() {
    
    }


Please suggest some design options which are considered as best practices.

Thanks!