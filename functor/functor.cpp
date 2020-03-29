#include <bits/stdc++.h>
using namespace std;

// template <class T>
// constexpr std::string_view type_name() {
//     using namespace std;
// #ifdef __clang__
//     string_view p = __PRETTY_FUNCTION__;
//     return string_view(p.data() + 34, p.size() - 34 - 1);
// #elif defined(__GNUC__)
//     string_view p = __PRETTY_FUNCTION__;
// #  if __cplusplus < 201402
//     return string_view(p.data() + 36, p.size() - 36 - 1);
// #  else
//     return string_view(p.data() + 49, p.find(';', 49) - 49);
// #  endif
// #elif defined(_MSC_VER)
//     string_view p = __FUNCSIG__;
//     return string_view(p.data() + 84, p.size() - 84 - 7);
// #endif
// }
// template<typename RetType,class... Params>
// class FunctorImpl {
//     public:
//     virtual RetType operator()(Params...) = 0;
// };

// template<typename ParentFunctor,typename Fun>
// class FunctorHandler : public FunctorImpl<typename ParentFunctor::RetType,typename ParentFunctor::Params>
// {
// public:
//     using ReturnType = typename ParentFunctor::RetType;
//     FunctorHandler(const Func& fun): _fun(fun) {}
//     ReturnType operator()(typename ParentFunctor::Params...) {
//         _fun(Params...);
//     }
// private:
//     Fun _fun;
// };

// template<typename RetType,class... Params>
// struct Functor {
//     public:
//     template<class Fun>
//     Functor(const Fun& fun) : spImpl(new FunctorHandler<Functor,Fun>(fun)) {}
//     using FuncSig = RetType (ObjType::*)(Params...);
//     FuncSig funcptr;
//     ObjType *obj;

//     RetType operator()(Params... params) {
//         return (*spImpl)(params...);
//     }
//     private:
//     using Impl = FunctorImpl<RetType,Params...>;
//     std::auto_ptr<Impl> spImpl;
// };
// class command {
//     int x;
//     char *ch;
//     public:
//     void operator()(int a,char *x) {
//         // some task
//         std::cout << "task1 done!" << std::endl;
//     }
// };


// int main() {
//     Functor<void,command,int,char *> f;
//     command c;
//     f.funcptr = &command::operator();
//     f.obj = &c;
//     char x[] = {'a','b'};
//     f(100,x);
// }


template<class RetType,class... Params>
struct Functor {
    std::function<RetType(Params...)> func;
    RetType operator()(Params... params) {
        return func(params...);
    }
    template<typename T>
    Functor(T&& _func) {
        std::cout <<"ok" << std::endl;
        func = std::forward<T>(_func);
    }
};
class command {
    int x;
    char *ch;
    public:
    void operator()(int a,char *x) {
        // some task
        std::cout << "task1 done!" << std::endl;
    }
};

int main() {
    command c;
    std::function<void(int,char*)> f(c);
    Functor<void,int,char*> ff(f);
    char x[] = {'a','b'};
    // f(100,x);
}